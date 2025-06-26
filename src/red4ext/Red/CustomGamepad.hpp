#pragma once

#include <Red/BaseGamepad.hpp>
#include <ICustomGameController.hpp>
#include <concrt.h>
#include <winrt/Windows.Gaming.Input.h>
#include <winrt/Windows.Foundation.h>
#include <hidsdi.h>
#include <hidpi.h>
#include <Red/Input.hpp>

using namespace winrt;
using namespace Windows::Gaming::Input;

// void __fastcall GamepadDestructor(void *a1) {
//   RED4ext::UniversalRelocFunc<decltype(&GamepadDestructor)> call(2647855289);
//   return call(a1);
// }

class CustomGamepad : public BaseGamepad {
public:
  virtual ~CustomGamepad() override {
    
  }
  
  using AllocatorType = RED4ext::Memory::Allocator<RED4ext::Memory::PoolEngine>;

  RED4ext::DynArray<ICustomGameController> controllers;
  concurrency::critical_section controllerListLock;

  CustomGamepad(uint32_t & gamepadIndex) : BaseGamepad(gamepadIndex) {
    RawGameController::RawGameControllerAdded([this](winrt::Windows::Foundation::IInspectable const &, RawGameController const &addedController) {
      concurrency::critical_section::scoped_lock{this->controllerListLock};
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
        auto controller = reinterpret_cast<ICustomGameController *>(controllerCls->CreateInstance(true));
        controllerCls->ConstructCls(controller);

        auto handle = RED4ext::Handle<ICustomGameController>(controller);
        controller->ref = RED4ext::WeakHandle(*reinterpret_cast<RED4ext::Handle<RED4ext::ISerializable> *>(&handle));
        controller->nativeType = controllerCls;

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
          concurrency::critical_section::scoped_lock{this->controllerListLock};
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

  virtual void Update(RED4ext::DynArray<Input> & inputs, HWND hwnd) override {
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
            inputs.EmplaceBack(*input);
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
            inputs.EmplaceBack(*input);
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
  }

  virtual void Reset(RED4ext::DynArray<Input> & inputs, HWND hwnd) override {
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
          inputs.EmplaceBack(*input);
          delete input;
        }
      }
      auto axisCount = controller.rawGameController.AxisCount();
      for (int i = 0; i < axisCount; ++i) {
        auto key = controller.axisKeys[i];
        if (key != RED4ext::EInputKey::IK_None) {
          auto input = new Input();
          UpdateInput(input, key, EInputAction::IACT_Axis, 0.0, 0, 0, hwnd, userIndex);
          inputs.EmplaceBack(*input);
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
  }

  virtual bool IsConnected() const override { 
    return true;
  }
};