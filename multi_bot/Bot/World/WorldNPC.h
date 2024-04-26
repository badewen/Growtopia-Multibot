#pragma once

#include <stdint.h>

#include <chrono>

using WorldNPCIndex = uint32_t;

enum eWorldNPCType : uint8_t {
    NpcNone,
    Ghost,
    GhostJar,
    BeeSwarm,
    HarvestGhost,
    GrowGa,
    GhostShark,
    XmasGhost,
    Blast,
    Pinata,
    GhostCaptureMachine,
    BossGhost,
    MindControlGhost,
    GhostBeGone,
    HuntedTurkey,
    Trickster,
    ThanksgivingTurkeyBoss,
    ThanksgivingTurketBossFeatherProjectile,
    AttackerMinionTurkey,
    BeachEnemy
};

struct WorldNPC
{
    uint8_t Index;
    eWorldNPCType Type;
    float PosX, PosY;
    float TargetX, TargetY;
    float Speed;
    
    // reserved for world info.cpp
    std::chrono::milliseconds LastUpdate;
};
