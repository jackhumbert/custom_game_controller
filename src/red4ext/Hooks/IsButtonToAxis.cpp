#include <Registrar.hpp>
#include <IsJoystickAxis.hpp>

// IsKey
REGISTER_HOOK_HASH(bool, 989204148, IsButtonToAxis, uint16_t key) {
  return IsButtonToAxis_Original(key) && !IsJoystickAxis(key);
}
