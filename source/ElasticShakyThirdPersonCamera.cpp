#include "ElasticShakyThirdPersonCamera.h"

ElasticShakyThirdPersonCamera::ElasticShakyThirdPersonCamera(const Vector3 &position,
                                                             float timeFactor,
                                                             float strength,
                                                             float rate,
                                                             const Vector3 &target,
                                                             const Vector3 &targetUp)
                                                             :
                                                             ElasticCamera(position, timeFactor),
                                                             ShakyCamera(position, strength, rate),
                                                             ThirdPersonCamera(position, target, targetUp)
{
}

void ElasticShakyThirdPersonCamera::Update(float dt)
{
    ElasticCamera::Update(dt);
    ShakyCamera::Update(dt);
    ThirdPersonCamera::Update(dt);
}
