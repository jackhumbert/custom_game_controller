#include <RED4ext/InstanceType.hpp>
#include <RED4ext/RED4ext.hpp>
#include <RED4ext/RTTITypes.hpp>
#include <RED4ext/RED4ext.hpp>
#include <spdlog/spdlog.h>
#include <RED4ext/Scripting/Natives/Generated/EInputKey.hpp>
#include "Utils.hpp"
#include <stdio.h>
#include <Windows.h>
#include <Dbt.h>
#include <hidsdi.h>
#include <hidpi.h>
#include <concrt.h>
#include <winrt/Windows.Gaming.Input.h>
#include <winrt/Windows.Foundation.h>
#include <deque>
using namespace winrt;
using namespace Windows::Gaming::Input;

struct XINPUT_GAMEPAD {
  WORD wButtons;
  BYTE bLeftTrigger;
  BYTE bRightTrigger;
  SHORT sThumbLX;
  SHORT sThumbLY;
  SHORT sThumbRX;
  SHORT sThumbRY;
};

struct XINPUT_STATE {
  DWORD dwPacketNumber;
  XINPUT_GAMEPAD Gamepad;
};

enum EInputAction {
  IACT_None = 0x0,
  IACT_Press = 0x1,
  IACT_Release = 0x2,
  IACT_Axis = 0x3,
};

enum EInputDevice {
  INVALID = 0x0,
  KBD_MOUSE = 0x1,
  ORBIS = 0x2,
  DURANGO = 0x3,
  STEAM = 0x4,
  XINPUT_PAD = 0x5,
  STADIA = 0x6,
  NINTENDO_SWITCH = 0x7,
  EID_COUNT = 0xC,
};

struct Input {
  RED4ext::EInputKey key;
  EInputAction action;
  float value;
  uint32_t unk0C;
  uint32_t unk10;
  uint32_t unk14;
  uint32_t unk18; // source, maybe? 2 disables processing in some places
  uint32_t unk1C;
  HWND hwnd;
  uint64_t index;
  uint64_t unk30;
  uint8_t unk38;
  uint8_t unk39;
  uint8_t unk3A;
  uint8_t unk3B;
  uint8_t unk3C;
  uint8_t unk3D;
  uint8_t unk3E;
  uint8_t unk3F;
};

Input *__fastcall UpdateInput(Input *input, RED4ext::EInputKey key, EInputAction action, float value, int a5, int a6,
                              HWND hwnd, uint32_t index) {
  input->hwnd = hwnd;
  input->index = index;
  input->unk0C = a5;
  input->unk10 = a6;
  input->value = value;
  input->unk18 = 1;
  input->unk30 = 0i64;
  input->unk38 = 0;
  input->key = key;
  input->action = action;
  return input;
}

class BaseGamepad {
public:
  virtual BaseGamepad* Release(byte a1) { return this; }                                          // 00
  virtual Input* GetInputs(RED4ext::DynArray<Input>* inputs, HWND hwnd) { return inputs->end(); } // 08

  virtual Input* ResetInputs(RED4ext::DynArray<Input>* inputs, HWND hwnd) {                       // 10
    return inputs->end();
  }

  virtual RED4ext::CName *__fastcall GetCName(RED4ext::CName *cname) {                            // 18
    *cname = RED4ext::CName("xpad");
    return cname;
  }

  virtual EInputDevice __fastcall GetType() { return EInputDevice::XINPUT_PAD; }                  // 20
  virtual void sub_28() {}                                                                        // 28
  virtual void sub_30() {}                                                                        // 30
  virtual uint32_t __fastcall GetIndex() { return this->userIndex; }                              // 38
  virtual bool IsEnabled(uint32_t *a1) { return false; }                                          // 40

  // not a part of the base class VFT
  virtual void __fastcall Initialize(uint32_t gamepadIndex) { userIndex = gamepadIndex; }

protected:
  uint32_t userIndex;
  XINPUT_STATE inputState;
};

concurrency::critical_section controllerListLock;

struct ICustomGameController : RED4ext::IScriptable {
  RED4ext::CClass *GetNativeType();

  RED4ext::DynArray<bool> buttons;
  RED4ext::DynArray<uint32_t> switches;
  RED4ext::DynArray<float> axes;

  uint16_t pid;
  uint16_t vid;
  int32_t id;

  bool buttonsNew[0x100];
  std::vector<GameControllerSwitchPosition> switchesNew;
  std::vector<double> axesNew;

  RED4ext::DynArray<RED4ext::EInputKey> buttonKeys;
  RED4ext::DynArray<RED4ext::EInputKey> axisKeys;
  RED4ext::DynArray<bool> axisInversions;
  RED4ext::DynArray<float> axisCenters;
  RED4ext::DynArray<float> axisDeadzones;

  bool connected;
  bool calibrated;
  RawGameController rawGameController = RawGameController(nullptr);

  void Setup(int32_t index) {
    connected = true;
    
    //auto nrid = winrt::to_string(rawGameController.NonRoamableId());
    //id = (uint64_t)RED4ext::CName(nrid.c_str());
    id = index;
    pid = rawGameController.HardwareProductId();
    vid = rawGameController.HardwareVendorId();

    auto numButtons = rawGameController.ButtonCount();
    for (int i = 0; i < numButtons; ++i) {
      buttons.EmplaceBack(false);
      buttonKeys.EmplaceBack(RED4ext::EInputKey::IK_None);
    }

    auto numAxes = rawGameController.AxisCount();
    for (int i = 0; i < numAxes; ++i) {
      axes.EmplaceBack(false);
      axisKeys.EmplaceBack(RED4ext::EInputKey::IK_None);
      axisInversions.EmplaceBack(false);
      axisCenters.EmplaceBack(0.5);
      axisDeadzones.EmplaceBack(0.01);
    }
    axesNew.resize(numAxes);

    auto numSwitches = rawGameController.SwitchCount();
    for (int i = 0; i < numSwitches; ++i) {
      switches.EmplaceBack(0);
    }
    switchesNew.resize(numSwitches);

    rawGameController.GetCurrentReading(buttonsNew, switchesNew, axesNew);
    UpdateValues();

    auto onInit = GetType()->GetFunction("OnSetup");
    if (onInit) {
      auto stack = RED4ext::CStack(this, nullptr, 0, nullptr, 0);
      onInit->Execute(&stack);
    }
  };

  void Update() {
    if (rawGameController) {
      rawGameController.GetCurrentReading(buttonsNew, switchesNew, axesNew);

      if (!calibrated) {
        auto axisCount = rawGameController.AxisCount();
        auto allZero = true;
        for (int i = 0; i < axisCount; i++) {
          allZero &= (axesNew[i] == 0.0);
        }
        if (!allZero) {
          spdlog::info("Calibrating controller: {}", id);
          for (int i = 0; i < axisCount; i++) {
            axisCenters[i] = axesNew[i];
            spdlog::info("Center for axis {}: {}", i, axisCenters[i]);
          }
          calibrated = true;
        }
      }

      auto onUpdate = GetType()->GetFunction("OnUpdate");
      if (onUpdate) {
        auto stack = RED4ext::CStack(this, nullptr, 0, nullptr, 0);
        onUpdate->Execute(&stack);
      }
    }
  };

  void UpdateValues() {
    if (!connected)
      return;

    auto buttonCount = rawGameController.ButtonCount();
    for (int i = 0; i < buttonCount; ++i) {
      if (buttons[i] != buttonsNew[i]) {
        buttons[i] = buttonsNew[i];
      }
    }
    auto axisCount = rawGameController.AxisCount();
    for (int i = 0; i < axisCount; ++i) {
      if (axes[i] != axesNew[i]) {
        axes[i] = axesNew[i];
      }
    }
    auto switchCount = rawGameController.SwitchCount();
    for (int i = 0; i < switchCount; ++i) {
      if (switches[i] != (uint32_t)switchesNew[i]) {
        switches[i] = (uint32_t)switchesNew[i];
      }
    }
  }

  float GetAxisValue(uint32_t index) { 
    float value = axesNew[index] - axisCenters[index];
    if (value > 0.0) {
      if (axisCenters[index] != 1.0) {
        value /= (1.0 - axisCenters[index]);
      }
    } else {
      if (axisCenters[index] != 0.0) {
        value /= axisCenters[index];
      }
    }
    value *= (axisInversions[index] ? -1.0 : 1.0);
    if (abs(value) < axisDeadzones[index]) {
      value = 0.0;
    } else {
      if (value > 0.0) {
        value -= axisDeadzones[index];
      } else {
        value += axisDeadzones[index];
      }
      if (axisDeadzones[index] != 1.0) {
        value /= (1.0 - axisDeadzones[index]);
      }
    }

    auto onUpdate = GetType()->GetFunction("GetAxisValue");
    if (onUpdate) {
      auto rtti = RED4ext::CRTTISystem::Get();
      RED4ext::CStackType args[2];
      args[0] = RED4ext::CStackType(rtti->GetType("Uint32"), &index);
      args[1] = RED4ext::CStackType(rtti->GetType("Float"), &value);
      float newValue = value;
      auto result = RED4ext::CStackType(rtti->GetType("Float"), &newValue);
      auto stack = RED4ext::CStack(this, args, 2, &result);
      onUpdate->Execute(&stack);
      value = *(float*)result.value;
    }

    return value;
  }
};

RED4ext::TTypedClass<ICustomGameController> cls("ICustomGameController");

RED4ext::CClass *ICustomGameController::GetNativeType() { return &cls; }

void SetButtonScripts(RED4ext::IScriptable *aContext, RED4ext::CStackFrame *aFrame, void *aOut, int64_t a4) {
  uint32_t button;
  RED4ext::EInputKey key;
  RED4ext::GetParameter(aFrame, &button);
  RED4ext::GetParameter(aFrame, &key);

  aFrame->code++;

  auto icgc = reinterpret_cast<ICustomGameController *>(aContext);

  if (button < icgc->buttonKeys.size) {
    icgc->buttonKeys[button] = key;
  }
}

void SetAxisScripts(RED4ext::IScriptable *aContext, RED4ext::CStackFrame *aFrame, void *aOut, int64_t a4) {
  uint32_t axis;
  RED4ext::EInputKey key;
  bool inverted;
  float center;
  float deadzone;
  RED4ext::GetParameter(aFrame, &axis);
  RED4ext::GetParameter(aFrame, &key);
  RED4ext::GetParameter(aFrame, &inverted);
  RED4ext::GetParameter(aFrame, &center);
  RED4ext::GetParameter(aFrame, &deadzone);

  aFrame->code++;

  auto icgc = reinterpret_cast<ICustomGameController *>(aContext);

  if (axis < icgc->axisKeys.size) {
    icgc->axisKeys[axis] = key;
    icgc->axisInversions[axis] = inverted;
    icgc->axisCenters[axis] = center;
    icgc->axisDeadzones[axis] = deadzone;
  }
}

class CustomGamepad : public BaseGamepad {
public:
  RED4ext::DynArray<ICustomGameController> controllers;

  void __fastcall Initialize(uint32_t gamepadIndex) {
    userIndex = gamepadIndex;
    RawGameController::RawGameControllerAdded([this](winrt::Windows::Foundation::IInspectable const &,
                                                     RawGameController const &addedController) {
      concurrency::critical_section::scoped_lock{controllerListLock};
      for (auto &controller : controllers) {
        if (!controller.connected) {
          spdlog::info("{:04X}:{:04X} reconnected to class: {}", addedController.HardwareVendorId(),
                       addedController.HardwareProductId(), RED4ext::CNamePool::Get(controller.GetType()->GetName()));
          controller.rawGameController = addedController;
          controller.connected = true;
          return;
        }
      }
      auto name = winrt::to_string(addedController.DisplayName());
      auto vid = addedController.HardwareVendorId();
      auto pid = addedController.HardwareProductId();
      spdlog::info("{:04X}:{:04X} {} connected", vid,
                   pid, name);
      auto id = winrt::to_string(addedController.NonRoamableId());
      // spdlog::info("          id:       {}", (uint64_t)RED4ext::CName(id.c_str()));
      spdlog::info("          id:       {}", (uint64_t)controllers.size);
      spdlog::info("          buttons:  {}", addedController.ButtonCount());
      spdlog::info("          axes:     {}", addedController.AxisCount());
      spdlog::info("          switches: {}", addedController.SwitchCount());

      auto rtti = RED4ext::CRTTISystem::Get();
      char className[100];
      sprintf(className, "CustomGameController_%04X_%04X", vid, pid);
      auto controllerCls = rtti->GetClassByScriptName(className);
      if (!controllerCls) {
        sprintf(className, "CustomGameController_%04x_%04x", vid, pid);
        controllerCls = rtti->GetClassByScriptName(className);
      }
      if (!controllerCls) {
        sprintf(className, "CustomGameController_%04X_XXXX", vid);
        controllerCls = rtti->GetClassByScriptName(className);
      }
      if (!controllerCls) {
        sprintf(className, "CustomGameController_%04x_xxxx", vid);
        controllerCls = rtti->GetClassByScriptName(className);
      }
      if (!controllerCls) {
        strcpy(className, "CustomGameController");
        controllerCls = rtti->GetClassByScriptName(className);
      }
      if (controllerCls) {
        spdlog::info("          class: {}", className);
        auto controller = reinterpret_cast<ICustomGameController *>(controllerCls->AllocInstance(true));
        controllerCls->ConstructCls(controller);

        auto handle = RED4ext::Handle<ICustomGameController>(controller);
        controller->ref = RED4ext::WeakHandle(*reinterpret_cast<RED4ext::Handle<RED4ext::ISerializable> *>(&handle));
        controller->unk30 = controllerCls;

        controller->rawGameController = addedController;
        controller->Setup(controllers.size);
        //auto numButtons = controller->rawGameController.ButtonCount();
        //for (auto i = 0; i < numButtons; ++i) {
        //  spdlog::info("          button {} ({}) value: {}", i, (uint32_t)controller->rawGameController.GetButtonLabel(i),
        //               controller->buttons[i]);
        //}
        //auto numSwitches = controller->rawGameController.SwitchCount();
        //for (auto i = 0; i < numSwitches; ++i) {
        //  spdlog::info("          switch {} value: {}", i, (uint32_t)controller->switches[i]);
        //}
        controllers.EmplaceBack(*controller);
      }
    });

    RawGameController::RawGameControllerRemoved(
        [this](winrt::Windows::Foundation::IInspectable const &, RawGameController const &removedController) {
          concurrency::critical_section::scoped_lock{controllerListLock};
          for (auto &controller : controllers) {
            if (controller.rawGameController == removedController) {
              auto name = winrt::to_string(removedController.DisplayName());
              spdlog::info("{:04X}:{:04X} ({}) {} disconnected from: {}", removedController.HardwareVendorId(),
                           removedController.HardwareProductId(), controller.id, name,
                           RED4ext::CNamePool::Get(controller.GetType()->GetName()));
              controller.connected = false;
              controller.calibrated = false;
              return;
            }
          }
        });
  }

  Input *__fastcall GetInputs(RED4ext::DynArray<Input> *inputs, HWND hwnd) {
    for (auto &controller : controllers) {
      if (!controller.connected)
        continue;

      controller.Update();

      auto buttonCount = controller.rawGameController.ButtonCount();
      for (int i = 0; i < buttonCount; ++i) {
        if (controller.buttons[i] != controller.buttonsNew[i]) {
          controller.buttons[i] = controller.buttonsNew[i];
          auto key = controller.buttonKeys[i];
          if (key != RED4ext::EInputKey::IK_None) {
            auto input = new Input();
            UpdateInput(input, key, controller.buttons[i] ? EInputAction::IACT_Press : EInputAction::IACT_Release, 1.0,
                        0, 0, hwnd, userIndex);
            inputs->EmplaceBack(*input);
            delete input;
          }
        }
      }
      auto axisCount = controller.rawGameController.AxisCount();
      for (int i = 0; i < axisCount; ++i) {
        float newValue = controller.GetAxisValue(i);
        if (controller.axes[i] != newValue) {
          controller.axes[i] = newValue;
          auto key = controller.axisKeys[i];
          if (key != RED4ext::EInputKey::IK_None) {
            auto input = new Input();
            UpdateInput(input, key, EInputAction::IACT_Axis, newValue, 0, 0, hwnd, userIndex);
            inputs->EmplaceBack(*input);
            delete input;
          }
        }
      }
      auto switchCount = controller.rawGameController.SwitchCount();
      for (int i = 0; i < switchCount; ++i) {
        if (controller.switches[i] != (uint32_t)controller.switchesNew[i]) {
          controller.switches[i] = (uint32_t)controller.switchesNew[i];
        }
      }
    }
    return inputs->end();
  };

  Input *__fastcall ResetInputs(RED4ext::DynArray<Input> *inputs, HWND hwnd) {
    for (auto &controller : controllers) {
      if (!controller.connected)
        continue;

      controller.Update();

      auto buttonCount = controller.rawGameController.ButtonCount();
      for (int i = 0; i < buttonCount; ++i) {
        auto key = controller.buttonKeys[i];
        if (key != RED4ext::EInputKey::IK_None) {
          auto input = new Input();
          UpdateInput(input, key, EInputAction::IACT_Release, 1.0, 0, 0, hwnd, userIndex);
          inputs->EmplaceBack(*input);
          delete input;
        }
      }
      auto axisCount = controller.rawGameController.AxisCount();
      for (int i = 0; i < axisCount; ++i) {
        auto key = controller.axisKeys[i];
        if (key != RED4ext::EInputKey::IK_None) {
          auto input = new Input();
          UpdateInput(input, key, EInputAction::IACT_Axis, 0.0, 0, 0, hwnd, userIndex);
          inputs->EmplaceBack(*input);
          delete input;
        }
      }
      auto switchCount = controller.rawGameController.SwitchCount();
      for (int i = 0; i < switchCount; ++i) {
        if (controller.switches[i] != (uint32_t)controller.switchesNew[i]) {
          controller.switches[i] = (uint32_t)controller.switchesNew[i];
        }
      }
    }
    return inputs->end();
  };

  bool __fastcall IsEnabled(uint32_t *a1) { return true; }
};

// Replaces XPads with our custom gamepad class
// 48 8D 05 A9 3C 9B 02 89 51 08 48 89 01 0F 57 C0
BaseGamepad *__fastcall InitializeXPad(BaseGamepad *, uint32_t);
constexpr uintptr_t InitializeXPadAddr = 0x794760 + 0xC00;
decltype(&InitializeXPad) InitializeXPad_Original;

BaseGamepad *__fastcall InitializeXPad(BaseGamepad *gamepad, uint32_t gamepadIndex) {
  if (gamepadIndex == 3) {
    gamepad = new CustomGamepad();
    gamepad->Initialize(gamepadIndex);
  } else {
    gamepad = new BaseGamepad();
    gamepad->Initialize(gamepadIndex);
  }
  //} else {
    //InitializeXPad_Original(gamepad, gamepadIndex);
  //}
  return gamepad;
}

// Allows joystick keys to be treated as axes
bool __fastcall IsJoystickAxis(uint16_t key) {
  auto result = false;
  result |= ((key >= (uint16_t)RED4ext::EInputKey::IK_JoyX) && (key <= (uint16_t)RED4ext::EInputKey::IK_JoyR));
  result |= ((key >= (uint16_t)RED4ext::EInputKey::IK_JoyU) && (key <= (uint16_t)RED4ext::EInputKey::IK_JoySlider2));
  return result;
}

bool __fastcall IsJoystick(uint16_t key) {
  auto result = IsJoystickAxis(key);
  result |= ((key >= (uint16_t)RED4ext::EInputKey::IK_Joy1) && (key <= (uint16_t)RED4ext::EInputKey::IK_Joy16));
  return result;
}

// takes a 16b version of RED4ext::EInputKey
// B8 96 00 00 00 66 2B C8 66 83 F9 05 0F 96 C0 C3
bool __fastcall IsAxis(uint16_t key);
constexpr uintptr_t IsAxisAddr = 0x2D146A0 + 0xC00;
decltype(&IsAxis) IsAxis_Original;

bool __fastcall IsAxis(uint16_t key) {
  return IsAxis_Original(key) || IsJoystickAxis(key);
}

// BA 96 00 00 00 0F B7 C1 66 2B C2 66 83 F8 05 76
bool __fastcall IsButtonToAxis(uint16_t key);
constexpr uintptr_t IsButtonToAxisAddr = 0x2D146B0 + 0xC00;
decltype(&IsButtonToAxis) IsButtonToAxis_Original;

bool __fastcall IsButtonToAxis(uint16_t key) { 
  return IsButtonToAxis_Original(key) && !IsJoystickAxis(key);
}

// BA 88 00 00 00 0F B7 C1 66 2B C2 66 83 F8 13 76
bool __fastcall IsGamepad(uint16_t key);
constexpr uintptr_t IsGamepadAddr = 0x2D146F0 + 0xC00;
decltype(&IsGamepad) IsGamepad_Original;

bool __fastcall IsGamepad(uint16_t key) { 
  return IsGamepad_Original(key) || IsJoystick(key);
}

RED4EXT_C_EXPORT void RED4EXT_CALL RegisterTypes() {
  spdlog::info("Registering classes & types");
  auto rtti = RED4ext::CRTTISystem::Get();
  //cls.flags = {.isNative = true};
   cls.flags = {.isAbstract = true, .isNative = true};
  cls.parent = rtti->GetClass("IScriptable");
  rtti->RegisterType(&cls);

  // RED4ext::CNamePool::Add("GameControllerSwitchPosition");
  // auto gcsp = RED4ext::CEnum::CEnum("GameControllerSwitchPosition", 4, {.isScripted = true});
  // gcsp.hashList.EmplaceBack(RED4ext::CName("Center"));
  // gcsp.hashList.EmplaceBack(RED4ext::CName("Up"));
  // gcsp.hashList.EmplaceBack(RED4ext::CName("UpRight"));
  // gcsp.hashList.EmplaceBack(RED4ext::CName("Right"));
  // gcsp.hashList.EmplaceBack(RED4ext::CName("DownRight"));
  // gcsp.hashList.EmplaceBack(RED4ext::CName("Down"));
  // gcsp.hashList.EmplaceBack(RED4ext::CName("DownLeft"));
  // gcsp.hashList.EmplaceBack(RED4ext::CName("Left"));
  // gcsp.hashList.EmplaceBack(RED4ext::CName("UpLeft"));
  // gcsp.valueList.EmplaceBack(0);
  // gcsp.valueList.EmplaceBack(1);
  // gcsp.valueList.EmplaceBack(2);
  // gcsp.valueList.EmplaceBack(3);
  // gcsp.valueList.EmplaceBack(4);
  // gcsp.valueList.EmplaceBack(5);
  // gcsp.valueList.EmplaceBack(6);
  // gcsp.valueList.EmplaceBack(7);
  // gcsp.valueList.EmplaceBack(8);
  // rtti->RegisterType(&gcsp);
}

RED4EXT_C_EXPORT void RED4EXT_CALL PostRegisterTypes() {
  spdlog::info("Registering members & functions");
  RED4ext::CNamePool::Add("array:EInputKey");
  // RED4ext::CNamePool::Add("array:GameControllerSwitchPosition");
  auto rtti = RED4ext::CRTTISystem::Get();

  //RED4ext::CProperty::Flags flags = {
      //.b21 = true,
      //.b29 = true,
      //.b31 = true,
      //.b35 = true,
      //.b36 = true,
  //};

  cls.props.PushBack(RED4ext::CProperty::Create(rtti->GetType("Int32"), "id", nullptr,
                                                offsetof(ICustomGameController, id)));
  cls.props.PushBack(RED4ext::CProperty::Create(rtti->GetType("Uint16"), "vid", nullptr,
                                                offsetof(ICustomGameController, vid)));
  cls.props.PushBack(RED4ext::CProperty::Create(rtti->GetType("Uint16"), "pid", nullptr,
                                                offsetof(ICustomGameController, pid)));
  cls.props.PushBack(RED4ext::CProperty::Create(rtti->GetType("array:Bool"), "buttons", nullptr,
                                                offsetof(ICustomGameController, buttons)));
  // cls.props.PushBack(RED4ext::CProperty::Create(rtti->GetType("array:GameControllerSwitchPosition"), "switches",
  // .ullptr, offsetof(ICustomGameController, switches)));
  cls.props.PushBack(RED4ext::CProperty::Create(rtti->GetType("array:Uint32"), "switches",
                                                nullptr, offsetof(ICustomGameController, switches)));
  cls.props.PushBack(RED4ext::CProperty::Create(rtti->GetType("array:Float"), "axes", nullptr,
                                                offsetof(ICustomGameController, axes)));
   cls.props.PushBack(RED4ext::CProperty::Create(rtti->GetType("array:EInputKey"), "buttonKeys", nullptr,
                                                 offsetof(ICustomGameController, buttonKeys)));
   cls.props.PushBack(RED4ext::CProperty::Create(rtti->GetType("array:EInputKey"), "axisKeys", nullptr,
                                                 offsetof(ICustomGameController, axisKeys)));
   cls.props.PushBack(RED4ext::CProperty::Create(rtti->GetType("array:Bool"), "axisInversions", nullptr,
                                                 offsetof(ICustomGameController, axisInversions)));
   cls.props.PushBack(RED4ext::CProperty::Create(rtti->GetType("array:Float"), "axisDeadzones", nullptr,
                                                 offsetof(ICustomGameController, axisDeadzones)));
   cls.props.PushBack(RED4ext::CProperty::Create(rtti->GetType("array:Float"), "axisCenters", nullptr,
                                                 offsetof(ICustomGameController, axisCenters)));

  auto setButton =
      RED4ext::CClassFunction::Create(&cls, "SetButton", "SetButton", &SetButtonScripts, {.isNative = true});
  cls.RegisterFunction(setButton);
  auto setAxis = RED4ext::CClassFunction::Create(&cls, "SetAxis", "SetAxis", &SetAxisScripts, {.isNative = true});
  cls.RegisterFunction(setAxis);

}

RED4EXT_C_EXPORT bool RED4EXT_CALL Main(RED4ext::PluginHandle aHandle, RED4ext::EMainReason aReason,
                                        const RED4ext::Sdk *aSdk) {
  switch (aReason) {
  case RED4ext::EMainReason::Load: {
    // Attach hooks, register RTTI types, add custom states or initalize your
    // application. DO NOT try to access the game's memory at this point, it
    // is not initalized yet.

    Utils::CreateLogger();
    spdlog::info("Starting up");

    RED4ext::RTTIRegistrator::Add(RegisterTypes, PostRegisterTypes);

    while (!aSdk->hooking->Attach(aHandle, RED4EXT_OFFSET_TO_ADDR(InitializeXPadAddr), &InitializeXPad,
                                  reinterpret_cast<void **>(&InitializeXPad_Original)))
      ;
    while (!aSdk->hooking->Attach(aHandle, RED4EXT_OFFSET_TO_ADDR(IsAxisAddr), &IsAxis,
                                  reinterpret_cast<void **>(&IsAxis_Original)))
      ;
    while (!aSdk->hooking->Attach(aHandle, RED4EXT_OFFSET_TO_ADDR(IsButtonToAxisAddr), &IsButtonToAxis,
                                  reinterpret_cast<void **>(&IsButtonToAxis_Original)))
      ;
    while (!aSdk->hooking->Attach(aHandle, RED4EXT_OFFSET_TO_ADDR(IsGamepadAddr), &IsGamepad,
                                  reinterpret_cast<void **>(&IsGamepad_Original)))
      ;

    break;
  }
  case RED4ext::EMainReason::Unload: {
    // Free memory, detach hooks.
    // The game's memory is already freed, to not try to do anything with it.

    spdlog::info("Shutting down");
    aSdk->hooking->Detach(aHandle, RED4EXT_OFFSET_TO_ADDR(InitializeXPadAddr));
    aSdk->hooking->Detach(aHandle, RED4EXT_OFFSET_TO_ADDR(IsAxisAddr));
    aSdk->hooking->Detach(aHandle, RED4EXT_OFFSET_TO_ADDR(IsButtonToAxisAddr));
    aSdk->hooking->Detach(aHandle, RED4EXT_OFFSET_TO_ADDR(IsGamepadAddr));
    spdlog::shutdown();
    break;
  }
  }

  return true;
}

RED4EXT_C_EXPORT void RED4EXT_CALL Query(RED4ext::PluginInfo *aInfo) {
  aInfo->name = L"Custom Game Controller";
  aInfo->author = L"Jack Humbert";
  aInfo->version = RED4EXT_SEMVER(0, 0, 2);
  aInfo->runtime = RED4EXT_RUNTIME_LATEST;
  aInfo->sdk = RED4EXT_SDK_LATEST;
}

RED4EXT_C_EXPORT uint32_t RED4EXT_CALL Supports() { return RED4EXT_API_VERSION_LATEST; }
