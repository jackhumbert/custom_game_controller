#include <Registrar.hpp>
#include <IsKey.hpp>
// #include <IsJoystickAxis.hpp>
#include <IsAxis.hpp>
#include <IsRelative.hpp>

// originally checks for
// (key < 150 || key > 155) && !IsRelative(key)
REGISTER_HOOK_HASH(bool, 989204148, IsKey, uint16_t key) {
  // return IsKey_Original(key) && !IsJoystickAxis(key);
  return !IsAxis(key) && !IsRelative(key);
}