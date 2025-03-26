extern "C" {
#include "z_bg_spot01_idosoko.hpp"

#define FLAGS 0x80400010

void BgSpot01Idosoko_Init(Actor* actor, GameState* state);
void BgSpot01Idosoko_Destroy(Actor* actor, GameState* state);
void BgSpot01Idosoko_Update(Actor* actor, GameState* state);
void BgSpot01Idosoko_Draw(Actor* actor, GameState* state);

void BgSpot01Idosoko_DoNothing(BgSpot01Idosoko* self, PlayState* play);

//GLOBAL_ASM("data/z_Bg_Spot01_Idosoko.data.s")

ActorInit Bg_Spot01_Idosoko_InitVars = {
    ACTOR_BG_SPOT01_IDOSOKO,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT01_MATOYA,
    sizeof(BgSpot01Idosoko),
    (ActorFunc)BgSpot01Idosoko_Init,
    (ActorFunc)BgSpot01Idosoko_Destroy,
    (ActorFunc)BgSpot01Idosoko_Update,
    (ActorFunc)BgSpot01Idosoko_Draw,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

//GLOBAL_ASM("asm/BgSpot01Idosoko_Init.s")
static void BgSpot01Idosoko_SetupAction(BgSpot01Idosoko* self, BgSpot01IdosokoActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}
    
void BgSpot01Idosoko_Init(Actor* actor, GameState* state) {
    BgSpot01Idosoko* self = (BgSpot01Idosoko*) actor;
    PlayState* play = (PlayState*) state;
    CollisionHeader* colHeader = NULL;
    u32 bgId;
    
    DynaPolyActor_Init(&self->dyna, DPM_PLAYER);
    Actor_ProcessInitChain(&self->dyna.actor, sInitChain);
    ZARInfo* zarInfo = FUN_00372f38(actor, play, &self->skelAnimModel, 2, 0);

    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 1), &colHeader);
    bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
    self->dyna.bgId = bgId;
    
    if (LINK_AGE_IN_YEARS != YEARS_ADULT) {
        Actor_Kill(&self->dyna.actor);
    } else {
        BgSpot01Idosoko_SetupAction(self, BgSpot01Idosoko_DoNothing);
    }
}

void BgSpot01Idosoko_DoNothing(BgSpot01Idosoko* self, PlayState* play) {
}

//GLOBAL_ASM("asm/BgSpot01Idosoko_Destroy.s")
void BgSpot01Idosoko_Destroy(Actor* actor, GameState* state) {
    BgSpot01Idosoko* self = (BgSpot01Idosoko*) actor;
    PlayState* play = (PlayState*) state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(&self->dyna.actor, &self->skelAnimModel, 0);
}

//GLOBAL_ASM("asm/BgSpot01Idosoko_Update.s")
void BgSpot01Idosoko_Update(Actor* actor, GameState* state) {
    BgSpot01Idosoko* self = (BgSpot01Idosoko*) actor;
    PlayState* play = (PlayState*) state;
    
    self->actionFunc(self, play);
}

//GLOBAL_ASM("asm/BgSpot01Idosoko_Draw.s")
void BgSpot01Idosoko_Draw(Actor* actor, GameState* state) {
    BgSpot01Idosoko* self = (BgSpot01Idosoko*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &self->dyna.actor.modelMtx);
    
    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&modelMtx);
    self->skelAnimModel->FUN_00372170(0); 
}
}
