#include <IsJoystickAxis.hpp>

using namespace RED4ext;

bool IsJoystickAxis(uint16_t key) {
  auto ekey = (EInputKey)key;
  auto result = false;
  result |= ((ekey >= EInputKey::IK_JoyX) && (ekey <= EInputKey::IK_JoyR));
  result |= ((ekey >= EInputKey::IK_JoyU) && (ekey <= EInputKey::IK_JoySlider2));
  return result;
}