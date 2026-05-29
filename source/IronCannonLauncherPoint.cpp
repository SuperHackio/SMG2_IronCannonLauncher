#include "IronCannonLauncherPoint.h"

#include "IronCannonShell.h"
#include "Game/Util.h"

namespace NrvIronCannonLauncherPoint {
    FULL_NERVE(IronCannonLauncherPointNrvWait, IronCannonLauncherPoint, Wait);
    FULL_NERVE(IronCannonLauncherPointNrvShot, IronCannonLauncherPoint, Shot);
}  // namespace NrvKoopaJrShipCannonShell

namespace {
    static const s32 sDefaultShotInterval = 300;
    static const f32 sDefaultBulletSpeed = 30.0f;
    static const f32 sGunPointOffset = 75.0f;
}

IronCannonLauncherPoint::IronCannonLauncherPoint(const char* pName)
    : LiveActor(pName), mShellHolder(nullptr), mShotInterval(sDefaultShotInterval), mBulletSpeed(sDefaultBulletSpeed), mBulletLifeTime(0) {
}

void IronCannonLauncherPoint::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelAndConnectToScene();
    MR::getJMapInfoArg0NoInit(rIter, &mShotInterval);
    MR::getJMapInfoArg1NoInit(rIter, &mBulletSpeed);
    MR::getJMapInfoArg2NoInit(rIter, &mBulletLifeTime);
    initBullet();
    initEffectKeeper(0, "IronCannonLauncherPoint", false);

    bool isAppear = false;
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        isAppear = true;
    }
    MR::useStageSwitchReadA(this, rIter);
    MR::setGroupClipping(this, rIter, 32);
    initNerve(&NrvIronCannonLauncherPoint::IronCannonLauncherPointNrvWait::sInstance, 0);

    if (isAppear)
        makeActorDead();
    else
        makeActorAppeared();
}

void IronCannonLauncherPoint::exeWait() {
    if (!MR::isStageStateScenarioOpeningCamera() && MR::isValidSwitchA(this) && !MR::isOnSwitchA(this)) {
        setNerve(&NrvIronCannonLauncherPoint::IronCannonLauncherPointNrvWait::sInstance);
        return;
    }

    if (MR::isGreaterStep(this, mShotInterval))
        setNerve(&NrvIronCannonLauncherPoint::IronCannonLauncherPointNrvShot::sInstance);
}

void IronCannonLauncherPoint::exeShot() {
    if (MR::isFirstStep(this)) {
        tryShotBullet(0.0f);
        setNerve(&NrvIronCannonLauncherPoint::IronCannonLauncherPointNrvWait::sInstance);
    }
}

void IronCannonLauncherPoint::initModelAndConnectToScene() {
    MR::connectToSceneMapObjMovement(this);
}

void IronCannonLauncherPoint::initBullet() {
    mShellHolder = new CannonShellHolder(3);
    for (s32 i = 0; i < 3; i++) {
        IronCannonShell* pShell = new IronCannonShell("ƒLƒƒƒmƒ“’e–C‘ä‚Ì’e");
        pShell->initWithoutIter();
        pShell->makeActorDead();
        if (mBulletLifeTime > 0)
            pShell->mLifeTime = mBulletLifeTime;
        mShellHolder->registerCannonShell(pShell);
    }
}

bool IronCannonLauncherPoint::tryShotBullet(f32 offset) {
    IronCannonShell* pShell = reinterpret_cast<IronCannonShell*>(mShellHolder->getValidShell());
    if (pShell == nullptr)
        return false;

    TPos3f mtx;
    MR::makeMtxTRS(mtx, mTranslation, mRotation, mScale);
    TVec3f vec;
    mtx.getZDir(vec);

    TVec3f vec2;
    JMAVECScaleAdd(vec.toVecPtr(), mTranslation.toVecPtr(), vec2.toVecPtr(), sGunPointOffset + offset);
    pShell->launch(vec2, vec * mBulletSpeed);

    TVec3f shadowdir;
    mtx.getYDir(shadowdir);
    shadowdir.negate();
    MR::setShadowDropDirection(pShell, nullptr, shadowdir);

    MR::emitEffect(this, "Shoot");
    return true;
}