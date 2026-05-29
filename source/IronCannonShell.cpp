#include "IronCannonShell.h"

#include "Game/Util.h"

namespace {
    static const f32 sBodySensorRadius = 75.0f;
    static const f32 sAttackSensorRadius = 60.0f;
    static const f32 sShadowRadius = 60.0f;
    static const f32 sBinderRadius = 75.0f;
    static const f32 sStarWandRadius = 150.0f;
    static const s32 sLifeTime = 360;
    static const f32 sCameraShakeDistance = 1000.0f; // not implemented
    static const s32 sDownTime = 45;
    static const f32 sGravity = 1.0f;
    static const f32 sReboundRate = 0.55f;
    static const f32 sReboundMinSpeed = 2.0f;
    static const f32 sAirFrec = 0.99f;
    static const f32 sGroundFrec = 0.94999999f;
    static const s32 sFreezeFrame = 20; // not implemented
    static const f32 sFreezeRumbleSpeed = 0.75f; // not implemented
    static const f32 sFreezeRumbleWidth = 5.0f; // not implemented
    static const s32 sWallHitInvalidTime = 10;

    static const char* sModelName = "KoopaJrShipCannonShell";
    static const char* sFlyEffectName = "LocusSmoke";
    static const char* sExplosionEffectName = "Explosion";
    static const char* sMisFireEffectName = "MisFire";
    static const char* sTouchEffectName = "Touch";
}

IronCannonShell::IronCannonShell(const char* pName) : CannonShellBase(pName) {
	mBaseScale = 1.3f;
    mLifeTime = sLifeTime;
    mHitInvalidTime = sWallHitInvalidTime;
    mDownTime = sDownTime;
}

void IronCannonShell::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm(sModelName, nullptr, nullptr, false);
    MR::startBck(this, sModelName, nullptr);
    initSound(4, false, nullptr, TVec3f());
    initHitSensor(2);
    MR::addHitSensorEnemy(this, "Body", 8, sBodySensorRadius, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEnemyAttack(this, "Attack", 8, sAttackSensorRadius * mBaseScale, TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(sBinderRadius * mBaseScale, 0.0f, 0);
    initEffectKeeper(0, nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::invalidateClipping(this);
    MR::initStarPointerTarget(this, sStarWandRadius, TVec3f());
    MR::initShadowVolumeSphere(this, sShadowRadius * mBaseScale);

    CannonShellBase::init(rIter);
}

void IronCannonShell::kill() {
    MR::tryDeleteEffect(this, sFlyEffectName);
    CannonShellBase::kill();
}

void IronCannonShell::launch(const TVec3f& rStartPos, const TVec3f& rVelocity) {
    CannonShellBase::launch(rStartPos, rVelocity);
    MR::startSound(this, "SE_BM_KOOPAJR_SHIP_SHOOT_NORMAL", -1, -1);
}

void IronCannonShell::explosion() {
    MR::tryEmitEffect(this, sExplosionEffectName);
    MR::startSound(this, "SE_EM_KILLER_EXPLOSION", -1, -1);
    CannonShellBase::explosion();
}

void IronCannonShell::misfire() {
    MR::tryEmitEffect(this, sMisFireEffectName);
    MR::startSound(this, "SE_EM_KILLER_MISS_FIRE", -1, -1);
    CannonShellBase::misfire();
}

void IronCannonShell::exeFly() {
    if (MR::isFirstStep(this)) {
        MR::tryEmitEffect(this, sFlyEffectName);
        if (mFreezeTime)
            MR::tryDeleteEffect(this, sTouchEffectName);
        mFreezeTime = 0;
    }
    MR::startLevelSound(this, "SE_EM_LV_KILLER_FLY", -1, -1, -1);
    CannonShellBase::exeFly();
}

void IronCannonShell::exeDown() {
    if (MR::isFirstStep(this)) {
        MR::onCalcGravity(this);
        if (mFreezeTime) {
            MR::startBlowHitSound(this);
            MR::tryDeleteEffect(this, sTouchEffectName);
        }
        else
            MR::startSound(this, "SE_EM_STOMPED_S", -1, -1);
        if (isNerve(&NrvCannonShellBase::HostTypeHipDropDown::sInstance)) {
            MR::jumpPlayer(-mGravity);
        }
    }

    mVelocity.add(mGravity * mDownGravityMult);
    if (MR::reboundVelocityFromCollision(this, sReboundRate, sReboundMinSpeed, 1.0f))
        MR::tryDeleteEffect(this, sFlyEffectName);

    mVelocity.scale(MR::isBindedGround(this) ? sGroundFrec : sAirFrec);

    if (MR::isStep(this, mDownTime))
        misfire();
}

void IronCannonShell::exeFreeze() {
    if (MR::isFirstStep(this))
        if (!mFreezeTime)
            MR::tryEmitEffect(this, sTouchEffectName);

    CannonShellBase::exeFreeze();
}