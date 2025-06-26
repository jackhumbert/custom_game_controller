#include <Registrar.hpp>
#include <IsJoystick.hpp>

REGISTER_HOOK_HASH(bool, 1763577801, IsPadKey, uint16_t key) {
  return IsPadKey_Original(key) || IsJoystick(key);
}