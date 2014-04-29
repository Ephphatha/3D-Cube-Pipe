#include "ThirdPersonCamera.h"

ThirdPersonCamera::ThirdPersonCamera(const Vector3 &position, const Vector3 &target, const Vector3 &targetUp)
                                     :
                                     DynamicCamera(position, target - position, targetUp),
                                     target(target),
                                     targetUp(targetUp)
{
}

void ThirdPersonCamera::Update(float dt)
{
    Camera::LookAt(this->target, this->targetUp);
}

void ThirdPersonCamera::LookAt(const Vector3 &target, const Vector3 &targetUp)
{
    this->target = target;
    this->targetUp = targetUp;
}
