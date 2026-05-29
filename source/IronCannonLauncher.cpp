#include "IronCannonLauncher.h"

#include "Game/Util.h"

namespace NrvIronCannonLauncher {
    FULL_NERVE(IronCannonLauncherNrvRelax, IronCannonLauncher, Relax);
    FULL_NERVE(IronCannonLauncherNrvWait, IronCannonLauncher, Wait);
    FULL_NERVE(IronCannonLauncherNrvShot, IronCannonLauncher, Shot);
}  // namespace NrvKoopaJrShipCannonShell

namespace {
    static const f32 sGunPointModelOffset = 100.0f;
}

IronCannonLauncher::IronCannonLauncher(const char* pName) : IronCannonLauncherPoint(pName), mEffectMtx(0.0f, 0.0f, 0.0f) {
}

void IronCannonLauncher::init(const JMapInfoIter& rIter) {
    IronCannonLauncherPoint::init(rIter);

    MR::useStageSwitchReadA(this, rIter);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, sGunPointModelOffset);
    TVec3f front;
    MR::calcFrontVec(&front, this);
    JMAVECScaleAdd(front.toVecPtr(), mTranslation.toVecPtr(), mEffectMtx.toVecPtr(), sGunPointModelOffset);
    MR::setEffectHostSRT(this, "Shoot", &mEffectMtx, nullptr, nullptr);
    setNerve(&NrvIronCannonLauncher::IronCannonLauncherNrvWait::sInstance);
}

void IronCannonLauncher::exeRelax() {
    if (MR::isOnSwitchA(this))
        setNerve(&NrvIronCannonLauncher::IronCannonLauncherNrvWait::sInstance);
}

void IronCannonLauncher::exeWait() {
    if (MR::isStep(this, mShotInterval))
        setNerve(&NrvIronCannonLauncher::IronCannonLauncherNrvShot::sInstance);
}

void IronCannonLauncher::exeShot() {
    if (MR::isFirstStep(this)) {
        MR::tryStartAllAnim(this, "Shot");
        tryShotBullet(sGunPointModelOffset);
        setNerve(&NrvIronCannonLauncher::IronCannonLauncherNrvWait::sInstance);
    }
}

void IronCannonLauncher::initModelAndConnectToScene() {
    initModelManagerWithAnm("IronCannonLauncher", nullptr, nullptr, false);
    MR::connectToSceneMapObj(this);
}