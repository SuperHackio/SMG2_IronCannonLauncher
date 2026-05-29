#pragma once

#include "CannonShellBase.h"

class IronCannonShell : public CannonShellBase {
public:
    IronCannonShell(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void kill();

    virtual void launch(const TVec3f& rStartPos, const TVec3f& rVelocity);
    virtual void explosion();
    virtual void misfire();
    virtual void exeFly();
    virtual void exeDown();
    virtual void exeFreeze();
};