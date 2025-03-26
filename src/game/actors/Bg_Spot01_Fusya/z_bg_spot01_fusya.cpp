extern "C" {
#include "z_bg_spot01_fusya.hpp"

#define FLAGS 0x80400010

void BgSpot01Fusya_Init(Actor* actor, GameState* state);
void BgSpot01Fusya_Destroy(Actor* actor, GameState* state);
void BgSpot01Fusya_Update(Actor* actor, GameState* state);
void BgSpot01Fusya_Draw(Actor* actor, GameState* state);

void FUN_003e8674(BgSpot01Fusya* self, PlayState* play);

void FUN_0036ef10(Vec3f* destination, u32 arg1, float arg2);

//GLOBAL_ASM("data/z_Bg_Spot01_Fusya.data.s")

ActorInit Bg_Spot01_Fusya_InitVars = {
    ACTOR_BG_SPOT01_FUSYA,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT01_OBJECTS,
    sizeof(BgSpot01Fusya),
    (ActorFunc)BgSpot01Fusya_Init,
    (ActorFunc)BgSpot01Fusya_Destroy,
    (ActorFunc)BgSpot01Fusya_Update,
    (ActorFunc)BgSpot01Fusya_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 12800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 1300, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1300, ICHAIN_STOP),
};

//GLOBAL_ASM("asm/BgSpot01Fusya_Init.s")
static void BgSpot01Fusya_SetupAction(BgSpot01Fusya* self, BgSpot01FusyaActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

void BgSpot01Fusya_Init(Actor* actor, GameState* state) {
    BgSpot01Fusya* self = (BgSpot01Fusya*) actor;
    PlayState* play = (PlayState*) state;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    ZARInfo* zarInfo = FUN_00372f38(actor, play, &self->skelAnimModel, 0, 0);
    self->unk_1AC = 100.0f;
    self->unk_1B0 = 100.0f;
    self->unk_1B4 = 0.5f;
    
    if (gSaveContext.sceneSetupIndex < 4) {
        gSaveContext.eventChkInf[(0x65 >> 4)] &= ~(1 << (0x65 & 0xF));
    }

    BgSpot01Fusya_SetupAction(self, FUN_003e8674);
}

void FUN_003e8674(BgSpot01Fusya* self, PlayState* play) {
    Actor* actor = &self->actor;

    if (gSaveContext.eventChkInf[(0x65 >> 4)] & (1 << (0x65 & 0xF))) {
        self->unk_1B0 = 1800.0f;
    }

    actor->shape.rot.z += self->unk_1AC;

    f32 temp = ((self->unk_1AC - 100.0f) / 1700.0f) + 1.0f;
    FUN_0036ef10(&self->actor.world.pos, 0x010001dc, temp);

    Math_ApproachF(&self->unk_1AC, self->unk_1B0, self->unk_1B4, 100.0f);
}

//GLOBAL_ASM("asm/BgSpot01Fusya_Destroy.s")
void BgSpot01Fusya_Destroy(Actor* actor, GameState* state) {
    BgSpot01Fusya* self = (BgSpot01Fusya*) actor;

    FUN_00350f34(&self->actor, &self->skelAnimModel, 0);
}

//GLOBAL_ASM("asm/BgSpot01Fusya_Update.s")
void BgSpot01Fusya_Update(Actor* actor, GameState* state) {
    BgSpot01Fusya* self = (BgSpot01Fusya*) actor;
    PlayState* play = (PlayState*) state;

    self->actionFunc(self, play);
}

//GLOBAL_ASM("asm/BgSpot01Fusya_Draw.s")
void BgSpot01Fusya_Draw(Actor* actor, GameState* state) {
    BgSpot01Fusya* self = (BgSpot01Fusya*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->actor.modelMtx);
    
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0); 
}
}
