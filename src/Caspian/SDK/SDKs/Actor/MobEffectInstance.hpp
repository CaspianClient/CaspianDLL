#include "../../../Memory/Memory.hpp"

enum class MobEffectID  : uint32_t {
    Empty          = 0,
    Speed          = 1,
    Slowness       = 2,
    Haste          = 3,
    MiningFatigue  = 4,
    Strength       = 5,
    InstantHealth  = 6,
    InstantDamage  = 7,
    JumpBoost      = 8,
    Nausea         = 9,
    Regeneration   = 10,
    Resistance     = 11,
    FireResistance = 12,
    WaterBreathing = 13,
    Invisibility   = 14,
    Blindness      = 15,
    NightVision    = 16,
    Hunger         = 17,
    Weakness       = 18,
    Poison         = 19,
    Wither         = 20,
    HealthBoost    = 21,
    Absorption     = 22,
    Saturation     = 23,
    Levitation     = 24,
    FatalPoison    = 25,
    ConduitPower   = 26,
    SlowFalling    = 27,
    BadOmen        = 28,
    VillageHero    = 29,
};

struct FactorCalculationData {
    FactorCalculationData() = delete;

    int                                              mPaddingDuration;        // this+0x0
    float                                            mFactorStart;            // this+0x4
    float                                            mFactorTarget;           // this+0x8
    float                                            mFactorCurrent;          // this+0xC
    std::function<void(FactorCalculationData&, int)> mUpdateFn;               // this+0x10
    int                                              mEffectChangedTimestamp; // this+0x50
    float                                            mFactorPreviousFrame;    // this+0x54
    bool                                             mHadEffectLastTick;      // this+0x58
    bool                                             mHadApplied;             // this+0x59
};

class MobEffectInstance {
public:
    MobEffectID                      mId;                              // this+0x0
    int                              mDuration;                        // this+0x4
    int                              mDurationEasy;                    // this+0x8
    int                              mDurationNormal;                  // this+0xC
    int                              mDurationHard;                    // this+0x10
    int                              mAmplifier;                       // this+0x14
    bool                             mDisplayOnScreenTextureAnimation; // this+0x18
    bool                             mAmbient;                         // this+0x19
    bool                             mNoCounter;                       // this+0x1A
    bool                             mEffectVisible;                   // this+0x1B
    FactorCalculationData            mFactorCalculationData;           // this+0x20
};