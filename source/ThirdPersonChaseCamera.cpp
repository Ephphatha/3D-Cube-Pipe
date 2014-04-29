#include "ThirdPersonChaseCamera.h"

ThirdPersonChaseCamera::ThirdPersonChaseCamera(const Vector3 &position,
                                               const Vector3 &target,
                                               const Vector3 &targetUp,
                                               const Vector3 &targetForward,
                                               const Quaternion& orientation,
                                               float distance)
                                               :
                                               ThirdPersonCamera(position, target, targetUp),
                                               targetForward(targetForward),
                                               orientation(orientation),
                                               chaseDistance(distance)
{
    Update(0.0f);
}

void ThirdPersonChaseCamera::Update(float dt)
{
    Camera::LookAt(this->position + this->targetForward, this->targetUp);

    Camera::Pan(this->orientation);

    Camera::Surge((this->position - this->target).Norm() - this->chaseDistance);

    ThirdPersonCamera::Update(dt);
}
