#include <ICustomGameController.hpp>
#include <RED4ext/RED4ext.hpp>
#include <RED4ext/Scripting/Stack.hpp>

void ICustomGameController::Setup(int32_t index) {
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
    RED4ext::ExecuteFunction(this, onInit, nullptr);
  }
}

void ICustomGameController::Update() {
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
      RED4ext::ExecuteFunction(this, onUpdate, nullptr);
    }
  }
}

void ICustomGameController::UpdateValues() {
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

float ICustomGameController::GetAxisValue(uint32_t index) { 
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

RTTI_DEFINE_CLASS(ICustomGameController, "ICustomGameController", {
  RTTI_METHOD(SetButton);
  RTTI_METHOD(SetAxis);
  RTTI_PROPERTY(id);
  RTTI_PROPERTY(vid);
  RTTI_PROPERTY(pid);
  RTTI_PROPERTY(buttons);
  RTTI_PROPERTY(switches);
  RTTI_PROPERTY(axes);
  RTTI_PROPERTY(buttonKeys);
  RTTI_PROPERTY(axisKeys);
  RTTI_PROPERTY(axisInversions);
  RTTI_PROPERTY(axisDeadzones);
  RTTI_PROPERTY(axisCenters);
});