#include <Registrar.hpp>
#include <RED4ext/Scripting/Natives/Generated/EInputKey.hpp>
#include <IsJoystickAxis.hpp>

// REGISTER_HOOK_HASH(bool, 0, IsJoystick, uint16_t key) {
//   auto result = IsJoystickAxis(key);
//   result |= ((key >= (uint16_t)RED4ext::EInputKey::IK_Joy1) && (key <= (uint16_t)RED4ext::EInputKey::IK_Joy16));
//   return result;
// }


bool IsJoystick(uint16_t key) {
  auto result = IsJoystickAxis(key);
  result |= ((key >= (uint16_t)RED4ext::EInputKey::IK_Joy1) && (key <= (uint16_t)RED4ext::EInputKey::IK_Joy16));
  return result;
}
