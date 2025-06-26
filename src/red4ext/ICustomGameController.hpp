#pragma once

#include <RED4ext/RED4ext.hpp>
#include <RED4ext/Scripting/IScriptable.hpp>
#include <RED4ext/Scripting/Natives/Generated/EInputKey.hpp>
#include <spdlog/spdlog.h>
#include <concrt.h>
#include <winrt/Windows.Gaming.Input.h>
#include <winrt/Windows.Foundation.h>
#include <hidsdi.h>
#include <hidpi.h>
#include <RedLib.hpp>

using namespace winrt;
using namespace Windows::Gaming::Input;

struct ICustomGameController : RED4ext::IScriptable {
  // virtual RED4ext::CClass *GetNativeType() override;
  virtual ~ICustomGameController() override = default;

  RTTI_IMPL_TYPEINFO(ICustomGameController);
  RTTI_IMPL_ALLOCATOR();

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

  void Setup(int32_t index);
  void Update();
  void UpdateValues();
  float GetAxisValue(uint32_t index);

  void SetButton(uint32_t button, RED4ext::EInputKey key) {
    this->buttonKeys[button] = key;
  }
  
  void SetAxis(uint32_t axis, RED4ext::EInputKey key, bool inverted, float center, float deadzone) {
    this->axisKeys[axis] = key;
    this->axisInversions[axis] = inverted;
    this->axisCenters[axis] = center;
    this->axisDeadzones[axis] = deadzone;  
  }
};