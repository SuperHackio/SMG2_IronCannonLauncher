#pragma once

#include "IronCannonLauncherPoint.h"

class IronCannonLauncher : public IronCannonLauncherPoint {
public:
    IronCannonLauncher(const char*);

    virtual void init(const JMapInfoIter& rIter);

    virtual void initModelAndConnectToScene();

    void exeRelax();
    void exeWait();
    void exeShot();


    TVec3f mEffectMtx;  // 0x98
};