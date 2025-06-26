#pragma once

#include <RED4ext/RED4ext.hpp>
#include <concrt.h>
#include <winrt/Windows.Gaming.Input.h>
#include <winrt/Windows.Foundation.h>
#include <hidsdi.h>
#include <hidpi.h>
#include <XInput.hpp>
#include <Red/Input.hpp>

using namespace winrt;
using namespace Windows::Gaming::Input;

class BaseGamepad {
public:
  virtual ~BaseGamepad() {                                                    // 00

  }                                          
  virtual void Update(RED4ext::DynArray<Input>& inputs, HWND hwnd) {          // 08
    
  }

  virtual void Reset(RED4ext::DynArray<Input>& inputs, HWND hwnd) {           // 10

  }

  virtual RED4ext::CName GetDeviceName() const {                              // 18
    return RED4ext::CName("xpad");
  }

  virtual EInputDevice GetDeviceType() const {                                // 20
    return EInputDevice::XINPUT_PAD;
  }

  virtual void sub_28(uint32_t * unk) {                                       // 28 fires on setup? related to axis
    return;
    }                         

  virtual uint64_t sub_30(uint32_t * unk) {                                   // 30 fires after setup
    return 0;
  }                                                    

  virtual void sub_38() {                                                     // 38 scePadResetLightBar
    return;
  } 
                                                     
  virtual void sub_40(int a2) {                                               // 40 set lightbar?
    return;
  } 
                                                     
  virtual uint64_t GetDeviceID() const {                                      // 48
    return this->userIndex;
  }

  virtual bool IsConnected() const {                                          // 50
    return false;
  }                                

  BaseGamepad(uint32_t & gamepadIndex) { 
    userIndex = gamepadIndex;
  }

protected:
  uint32_t userIndex;
  XINPUT_STATE inputState = {0};
};

RED4EXT_ASSERT_SIZE(BaseGamepad, 0x20);