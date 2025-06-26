#include <IsJoystickAxis.hpp>

// Allows joystick keys to be treated as axes
bool __fastcall IsJoystickAxis(uint16_t key) {
  auto result = false;
  result |= ((key >= (uint16_t)RED4ext::EInputKey::IK_JoyX) && (key <= (uint16_t)RED4ext::EInputKey::IK_JoyR));
  result |= ((key >= (uint16_t)RED4ext::EInputKey::IK_JoyU) && (key <= (uint16_t)RED4ext::EInputKey::IK_JoySlider2));
  return result;
}