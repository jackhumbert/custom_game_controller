#pragma once

#include <RED4ext/RED4ext.hpp>
#include "IInputDeviceGamepad.hpp"

namespace input {

class InputDeviceGamepadXInput : IInputDeviceGamepad
{
  static constexpr const char* NAME = "inputInputDeviceGamepadXInput";
  static constexpr const char* ALIAS = "InputDeviceGamepadXInput";

  /// @hash 499584564:idata
  static constexpr const uintptr_t VFT = inputInputDeviceGamepadXInput_VFT_Addr;

  virtual ~InputDeviceGamepadXInput(); // 00
  virtual void Update(red::DynArray<struct input::BufferedInputEvent> &, unsigned __int64); // 08
  virtual void Reset(red::DynArray<input::BufferedInputEvent> &, unsigned __int64); // 10
  virtual CName GetDeviceName(void) const { // 18
    return CName("xpad");
  };
  virtual input::EInputDevice GetDeviceType(void) const { // 20
    return 5
  };
  virtual void sub_28(); // 28
  virtual void sub_30(); // 30
  virtual void sub_38(); // 38
  virtual void sub_40(); // 40
  virtual unsigned __int64 GetDeviceID(void) const;
  virtual bool IsConnected(void) const;
};

}