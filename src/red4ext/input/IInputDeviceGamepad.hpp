#pragma once

#include <RED4ext/RED4ext.hpp>
#include <RED4ext/Scripting/Natives/Generated/input/EInputDevice.hpp>

namespace input {

class IInputDeviceGamepad
{
  static constexpr const char* NAME = "inputIInputDeviceGamepad";
  static constexpr const char* ALIAS = "IInputDeviceGamepad";

  /// @hash 3559394325:idata
  static constexpr const uintptr_t VFT = inputIInputDeviceGamepad_VFT_Addr;

  virtual ~IInputDeviceGamepad() = 0; // 00
  virtual void Update(red::DynArray<struct input::BufferedInputEvent> &, unsigned __int64) = 0; // 08
  virtual void Reset(red::DynArray<input::BufferedInputEvent> &, unsigned __int64) = 0; // 10
  virtual CName GetDeviceName(void) const = 0; // 18
  virtual input::EInputDevice GetDeviceType(void) const = 0; // 20
  virtual void sub_28() {}; // 28
  virtual void sub_30() {}; // 30
  virtual void sub_38() {}; // 38
  virtual void sub_40() {}; // 40
  virtual unsigned __int64 GetDeviceID(void) const = 0; // 48
  virtual bool IsConnected(void) const = 0; // 50

  uint32_t deviceID; // 08 dwUserIndex
  uint32_t unk0C;
  uint64_t unk10;
  uint64_t unk18;
};

}


// public: virtual void input::IInputDeviceGamepad::SetPortHandle(int)
// ?SetPortHandle@IInputDeviceGamepad@input@@UEAAXH@Z