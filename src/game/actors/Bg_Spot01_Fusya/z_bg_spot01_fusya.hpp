#ifndef _Z_BG_SPOT01_FUSYA_H_
#define _Z_BG_SPOT01_FUSYA_H_

#include "global.hpp"

struct BgSpot01Fusya;

typedef void (*BgSpot01FusyaActionFunc)(struct BgSpot01Fusya*, PlayState*);

typedef struct BgSpot01Fusya {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ BgSpot01FusyaActionFunc actionFunc;
    /* 0x01A8 */ char unk_1A8[0x4];
    /* 0x01AC */ f32 unk_1AC;
    /* 0x01B0 */ f32 unk_1B0;
    /* 0x01B4 */ f32 unk_1B4;
    /* 0x01B8 */ SkeletonAnimationModel* skelAnimModel;
} BgSpot01Fusya; // size = 0x1BC

extern ActorInit Bg_Spot01_Fusya_InitVars;

#endif
