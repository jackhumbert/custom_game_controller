#pragma once

#include <RED4ext/RED4ext.hpp>
#include <winrt/Windows.Gaming.Input.h>
#include <winrt/Windows.Foundation.h>
#include <hidsdi.h>
#include <hidpi.h>

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