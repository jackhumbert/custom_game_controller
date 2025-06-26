#include <Red/Input.hpp>

Input *__fastcall UpdateInput(Input *input, RED4ext::EInputKey key, EInputAction action, float value, int a5, int a6, HWND hwnd, uint32_t index) {
  input->hwnd = hwnd;
  input->index = index;
  input->unk0C = a5;
  input->unk10 = a6;
  input->value = value;
  input->unk18 = 1;
  input->unk30 = 0i64;
  input->unk38 = 0;
  input->key = key;
  input->action = action;
  return input;
}