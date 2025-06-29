#include <IsAxis.hpp>
#include <IsJoystickAxis.hpp>

using namespace RED4ext;

bool IsAxis(uint16_t key) {
  // return ((key - 150) <= 5);
  return ((uint16_t)(key - 150U) <= 5U) || IsJoystickAxis(key);
}