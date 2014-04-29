#include "ElasticThirdPersonCamera.h"

ElasticThirdPersonCamera::ElasticThirdPersonCamera(const Vector3 &position,
                                                   float timeFactor,
                                                   const Vector3 &target,
                                                   const Vector3 &targetUp)
                                                   :
                                                   ElasticCamera(position, timeFactor),
                                                   ThirdPersonCamera(position, target, targetUp)
{
}

void ElasticThirdPersonCamera::Update(float dt)
{
    ElasticCamera::Update(dt);
    ThirdPersonCamera::Update(dt);
}
