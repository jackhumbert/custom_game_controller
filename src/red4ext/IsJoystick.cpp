#include <IsJoystick.hpp>
#include <IsJoystickAxis.hpp>
#include <IsJoystickButton.hpp>

bool IsJoystick(uint16_t key) {
  return IsJoystickAxis(key) || IsJoystickButton(key);
}