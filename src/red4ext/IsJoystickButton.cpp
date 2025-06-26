#include <IsJoystickButton.hpp>
#include <IsJoystickAxis.hpp>
#include <RED4ext/Scripting/Natives/Generated/EInputKey.hpp>

// REGISTER_HOOK_HASH(bool, 0, IsJoystick, uint16_t key) {
//   auto result = IsJoystickAxis(key);
//   result |= ((key >= (uint16_t)RED4ext::EInputKey::IK_Joy1) && (key <= (uint16_t)RED4ext::EInputKey::IK_Joy16));
//   return result;
// }

using namespace RED4ext;

bool IsJoystickButton(uint16_t key) {
  auto ekey = (EInputKey)key;
  auto result = IsJoystickAxis(key);
  result |= ((ekey >= EInputKey::IK_Joy1) && (ekey <= EInputKey::IK_Joy16));
  return result;
}
