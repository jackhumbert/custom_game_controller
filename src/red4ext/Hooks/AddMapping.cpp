#include <Registrar.hpp>
#include <RED4ext/Scripting/Natives/Generated/EInputKey.hpp>
#include <IsJoystickAxis.hpp>

using namespace RED4ext;

template<typename R, typename C, typename... Args>
R Exec(uint32_t aHash, C cls, Args&&... args) {
  static auto func = reinterpret_cast<R (*)(C, Args...)>(UniversalRelocBase::Resolve(aHash));
  return func(cls, std::forward<Args>(args)...);
}

struct UserMappingButtonGroup {
  bool Add(uint16_t key, CName name) { 
    return Exec<bool>(1221729244, this, key, name);
  }
};

struct UserMappingButton {
  bool Add(uint16_t key, CName name) { 
    return Exec<bool>(763041730, this, key, name);
  }

  bool AddFake(uint16_t key, CName name) { 
    return Exec<bool>(3283622355, this, key, name);
  }
};

struct UserMappingAxis {
  bool Add(uint16_t key, CName name, float val, uint8_t preset) { 
    return Exec<bool>(3994556065, this, key, name, val, preset);
  }

  bool AddFake(uint16_t key, CName name, float val, uint8_t preset) { 
    return Exec<bool>(4277802704, this, key, name, val, preset);
  }
};

struct UserMappingRelative{
  bool Add(uint16_t key, CName name, float val, uint8_t preset) { 
    return Exec<bool>(3346078191, this, key, name, val, preset);
  }
};

struct UserMapping {
  struct Mapping {
    uint8_t type;
    uint8_t unk01;
    uint16_t key16;
    EInputKey key32;
    CName name;
    CName presetName;
    float val;
    float thresholdPress;
    float thresholdRelease;
    uint32_t overridableUI;
    uint32_t unk28;
    uint32_t unk2C;
  };
  RED4EXT_ASSERT_SIZE(Mapping, 0x30);

  struct MappingCollection {
    DynArray<Mapping> actions;
  };

  struct RawMappingCollection {
    DynArray<Mapping> mappings;
    uint8_t needs_update;
  };

  struct MappingPreset {
    uint64_t unk00;
    uint8_t axis;
    uint8_t fakeAxis;
    uint8_t relativeAxis;
  };

  DynArray<void*> unk00; // 00
  uint8_t sorted; // 10
  DynArray<void*>  unk18; // 18
  CString invalid_str; // 28
  void * contextManager; // 48
  UserMappingButton * buttons; // 50
  UserMappingButtonGroup * buttonGroups; // 58
  UserMappingAxis * axes; // 60
  UserMappingRelative * relativeAxes; // 68
  Map<EInputKey, MappingCollection> keyMap; // 70
  Map<CName, RawMappingCollection> rawMappings; // 98
  HashMap<CString, EInputKey> stringToKey; // C0
  HashMap<CString, EInputKey> inputKeyMap_F0; // F0
  HashMap<EInputKey, CString> unk120; // 120

  MappingCollection & GetOrCreateMapping(uint16_t key) { 
    return Exec<MappingCollection &>(1563959014, this, key);
  }
  
  RawMappingCollection & GetOrCreateMapping(CName name) { 
    return Exec<RawMappingCollection &>(2841518691, this, name);
  }

  MappingPreset const * GetMappingPresetByName(CName name) const { 
    return Exec<MappingPreset const *>(834084382, this, name);
  }
};

enum EMappingType : uint8_t {
  Internal = 0, // maybe
  Button = 1,
  FakeButton = 2,
  Axis = 3,
  FakeAxis = 4,
  Relative = 5,
  Unk6 = 6 
};

REGISTER_HOOK_HASH(bool, 1385634503, AddMapping, 
  UserMapping* self, uint16_t key, CName name, EMappingType type, float val, CName presetName, float thresholdPress, float thresholdRelease, uint32_t overridableUI, uint32_t unk28
) {
  // return AddMapping_Original(self, key, name, type, val, presetName, a7, a8, a9, a10);
  auto keyMap = self->GetOrCreateMapping(key);
  auto nameMap = self->GetOrCreateMapping(name);
  bool status = false;

  UserMapping::Mapping mapping = {
    .type = type,
    .key16 = key,
    .key32 = (EInputKey)key,
    .name = name,
    .presetName = presetName,
    .val = val,
    .thresholdPress = thresholdPress,
    .thresholdRelease = thresholdRelease,
    .overridableUI = overridableUI,
    .unk28 = unk28
  };

  uint8_t preset = 0;

  switch (type) {
    case Button: {
      if (key >= 300) {
        status = self->buttonGroups->Add(key, name);
      } else {
        status = self->buttons->Add(key, name);
      }
    } break;
    case FakeButton: {
      status = self->buttons->AddFake(key, name);
    } break;
    case Axis: {
      preset = self->GetMappingPresetByName(presetName)->axis;
      status = self->axes->Add(key, name, val, preset);
    } break;
    case FakeAxis: {
      preset = self->GetMappingPresetByName(presetName)->fakeAxis;
      status = self->axes->AddFake(key, name, val, preset);
    } break;
    case Relative: {
      preset = self->GetMappingPresetByName(presetName)->relativeAxis;
      status = self->relativeAxes->Add(key, name, val, preset);
    } break;
    default:
      status = true;
  }
  if (status) {
    keyMap.actions.PushBack(mapping);
    nameMap.mappings.PushBack(mapping);
  }
  return status;
}
