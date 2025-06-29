#include <Registrar.hpp>
#include <RED4ext/Scripting/Natives/Generated/EInputKey.hpp>
#include <IsKey.hpp>
#include <IsAxis.hpp>
#include <IsRelative.hpp>

using namespace RED4ext;

class IHookable {
public:
  template<typename R, uint32_t Hash, typename... Args>
  inline R Hook(Args... args) {
    static auto func = UniversalRelocFunc<R (*)(IHookable *, Args...)>(Hash);
    return func(this, std::forward<Args>(args)...);
  }

  template<typename R, uint32_t Hash, typename... Args>
  inline R Hook(Args... args) const {
    static auto func = UniversalRelocFunc<R (*)(IHookable const *, Args...)>(Hash);
    return func(this, std::forward<Args>(args)...);
  }
};

struct UserMappingButtonGroup : IHookable {
  bool Add(uint16_t key, CName name) { 
    return Hook<bool, 1221729244>(key, name);
  }
};

struct UserMappingButton : IHookable {
  bool Add(uint16_t key, CName name) { 
    return Hook<bool, 763041730>(key, name);
  }

  bool AddFake(uint16_t key, CName name, float thresholdPress, float thresholdRelease) { 
    return Hook<bool, 3283622355>(key, name, thresholdPress, thresholdRelease);
  }
};

struct UserMappingAxis : IHookable {
  struct RealAxisInput {
    uint16_t key;
    uint16_t unk02;
    uint32_t unk04;
  };

  struct Unknown {
    uint32_t unk00;
    uint32_t unk04;
    uint32_t unk08;
  };
  RED4EXT_ASSERT_SIZE(Unknown, 0xC);

  struct Axis {  
    DynArray<RealAxisInput> realAxisInput;
    DynArray<Unknown> unk10;
    uint8_t invert;
    uint8_t unk21;
    uint64_t unk28;
  };
  RED4EXT_ASSERT_SIZE(Axis, 0x30);

  struct Pair {
    CName name1;
    CName name2;
    CName name3;
    void * axis;
    uint64_t unk20;
    CName name4;
    float unk30;
    uint32_t unk34;
  };

  bool Add(uint16_t key, CName name, uint8_t invert) { 
    // return Hook<bool, 3994556065>(key, name, invert);
    
    // manually add to avoid the inlined IsAxis
    if (!IsAxis(key))
      return false;

    Axis axis {
      .realAxisInput = DynArray<RealAxisInput>(Memory::Allocator<Memory::PoolGMPL_Input>::Get()),
      .unk10 = DynArray<Unknown>(Memory::Allocator<Memory::PoolGMPL_Input>::Get()),
      .invert = 0,
      .unk21 = 0,
      .unk28 = 0
    };
    auto pair = this->axes.Insert(name, axis);

    RealAxisInput real {
      .key = key,
      .unk04 = 0
    };
    pair.first->realAxisInput.PushBack(real);
    pair.first->invert = invert;

    return true;
  }

  bool AddFake(uint16_t key, CName name, float val, uint8_t invert) { 
    return Hook<bool, 4277802704>(key, name, val, invert);
  }

  HashMap<CName, Axis> axes;
  HashMap<CName, float> floats;
  DynArray<Pair *> pairs;
};

struct UserMappingRelative : IHookable {
  bool Add(uint16_t key, CName name, uint8_t invert) { 
    return Hook<bool, 3346078191>(key, name, invert);
  }
};

struct UserMapping : IHookable {
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
    CName name;
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
  Map<uint16_t, MappingCollection> keyMap; // 70
  Map<CName, RawMappingCollection> rawMappings; // 98
  HashMap<CString, EInputKey> stringToKey; // C0
  HashMap<CString, EInputKey> inputKeyMap_F0; // F0
  HashMap<EInputKey, CString> unk120; // 120
  uint64_t unk150[4];
  DynArray<MappingPreset*> mappingPresets;
  uint8_t unk180;
  uint8_t unk181;
  uint64_t unk188;
  uint64_t listener_id;

  MappingCollection * GetOrCreateMapping(uint16_t key) { 
    return Hook<MappingCollection *, 1563959014>(key);
  }
  
  RawMappingCollection * GetOrCreateMapping(CName name) { 
    return Hook<RawMappingCollection *, 2841518691>(name);
  }

  MappingPreset const * GetMappingPresetByName(CName name) { 
    return Hook<MappingPreset const *, 834084382>(name);
  }
};
RED4EXT_ASSERT_SIZE(UserMapping, 0x198);
RED4EXT_ASSERT_OFFSET(UserMapping, axes, 0x60);

enum EMappingType : uint8_t {
  Internal = 0, // maybe
  Button = 1,
  FakeButton = 2,
  Axis = 3,
  FakeAxis = 4,
  Relative = 5,
  Unk6 = 6 
};

struct DLA
{
  uint64_t unk00[0x108/8];
  uint64_t unk108;
  uint32_t unk110;
  uint32_t unk114;
  uint64_t unk118;
  uint64_t unk120;
};

struct ListenerAction : IScriptable
{
  uint64_t unk40;
  uint32_t unk48;
};


// REGISTER_HOOK_HASH(void, 1768234721, DLA_Something, DLA * self, ListenerAction* la, uint8_t * unk) {
//   return DLA_Something_Original(self, la, unk);
// }


REGISTER_HOOK_HASH(bool, 1385634503, AddMapping, 
  UserMapping* self, uint16_t key, CName name, EMappingType aType, float val, CName presetName, float thresholdPress, float thresholdRelease, uint32_t overridableUI, uint32_t unk28
) {
  // return AddMapping_Original(self, key, name, aType, val, presetName, thresholdPress, thresholdRelease, overridableUI, unk28);

  // original does a lot of inline key-checking that we want to avoid
  auto type = aType;
  if (type == Axis && IsKey(key)) {
    type = FakeAxis;
  } else if (type == Button && IsAxis(key)) {
    type = FakeButton;
  }

  if ((!IsKey(key) || type != Button)
    && (!IsAxis(key) || type != FakeButton)
    && (!IsKey(key) || type != FakeAxis)
    && (!IsAxis(key) || type != Axis)
    && (!IsRelative(key) || type != Relative)
    && (type != Unk6)
  ) {
    return false;
  }

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

  bool invert = false;

  switch (type) {
    case Button: {
      if (key >= 300) {
        status = self->buttonGroups->Add(key, name);
      } else {
        status = self->buttons->Add(key, name);
      }
    } break;
    case FakeButton: {
      status = self->buttons->AddFake(key, name, thresholdPress, thresholdRelease);
    } break;
    case Axis: {
      if (presetName) {
        invert = self->GetMappingPresetByName(presetName)->axis;
      }
      status = self->axes->Add(key, name, invert);
    } break;
    case FakeAxis: {
      if (presetName) {
        invert = self->GetMappingPresetByName(presetName)->fakeAxis;
      }
      status = self->axes->AddFake(key, name, val, invert);
    } break;
    case Relative: {
      if (presetName) {
        invert = self->GetMappingPresetByName(presetName)->relativeAxis;
      }
      status = self->relativeAxes->Add(key, name, invert);
    } break;
    default:
      status = true;
  }
  if (status) {
    keyMap->actions.EmplaceBack(mapping);
    nameMap->mappings.EmplaceBack(mapping);
  }
  return status;
}