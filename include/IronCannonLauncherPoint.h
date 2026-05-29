#pragma once

#include "Game/LiveActor/LiveActor.h"

class CannonShellHolder;

class IronCannonLauncherPoint : public LiveActor {
public:
    IronCannonLauncherPoint(const char*);

    virtual void init(const JMapInfoIter& rIter);

    virtual void initModelAndConnectToScene();

    void exeWait();
    void exeShot();

    void initBullet();
    bool tryShotBullet(f32);

    CannonShellHolder* mShellHolder; // 0x8C
    s32 mShotInterval; // 0x90 : Delay between cannon shots [ObjArg0]
    f32 mBulletSpeed; // 0x94 : Speed of each bullet [ObjArg1]
    s32 mBulletLifeTime; // NEW : Lifetime of the bullets [ObjArg2]
};