#pragma once

#include <RED4ext/RED4ext.hpp>
#include "IInputDeviceGamepad.hpp"

namespace input {

class InputDeviceGamepadPS4Pad : IInputDeviceGamepad
{
  static constexpr const char* NAME = "inputInputDeviceGamepadPS4Pad";
  static constexpr const char* ALIAS = "InputDeviceGamepadPS4Pad";

  /// @hash 499584564:idata
  static constexpr const uintptr_t VFT = inputInputDeviceGamepadPS4Pad_VFT_Addr;

  virtual ~InputDeviceGamepadPS4Pad(); // 00
  virtual void Update(red::DynArray<struct input::BufferedInputEvent> &, unsigned __int64); // 08
  virtual void Reset(red::DynArray<input::BufferedInputEvent> &, unsigned __int64); // 10
  virtual CName GetDeviceName(void) const { // 18
    return CName("ps4pad");
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

  uint64_t unk20;
  uint64_t unk28;
  uint64_t unk30;
  uint64_t unk38;
  uint64_t unk40;
  uint64_t unk48;
  uint64_t unk50;
  uint64_t unk58;
  uint64_t unk60;
  uint64_t unk68;
  uint64_t unk70;
  uint64_t unk78;
  uint64_t unk80;
  uint32_t unk88;

};

}