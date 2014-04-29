/*!
**  \file ElasticThirdPersonCamera.h
**  \brief Defines an observer camera that will move smoothly along a curve while constantly looking at a target point.
**
**  \author Andrew James
*/
#ifndef __ElasticThirdPersonCamera
#define __ElasticThirdPersonCamera
#include "ElasticCamera.h"
#include "ThirdPersonCamera.h"

#pragma warning(push)
#pragma warning(disable:4250)
// Because of the way we're inheriting camera functionality we get a warning about
//  functions being overriden via dominance, this can safely be ignored.

/*!
**  \class ElasticThirdPersonCamera
**  \brief Combines the functionality of ThirdPersonCamera and ElasticCamera.
**
**  Creates a camera best used for a flyby through scenery.
*/
class ElasticThirdPersonCamera : public ElasticCamera, public ThirdPersonCamera
{
public:
    /*!
    **  \brief Creates a camera at the specified initial position, looking at the target.
    **
    **  Defaults to sitting at the origin, looking down the -z axis with +y up.
    **
    **  \param position     Initial position of the camera.
    **  \param timeFactor   "Speed" of the camera.
    **  \param target       Focal point of the camera.
    **  \param targetUp     Local up direction.
    */
    ElasticThirdPersonCamera(const Vector3 &position = Vector3(),
                             float timeFactor = 1.0f,
                             const Vector3 &target = Vector3(0.0f, 0.0f, -1.0f),
                             const Vector3 &targetUp = Vector3(0.0f, 1.0f, 0.0f));

    /*!
    **  \brief Moves the camera along the path, then rotates to look at the target again.
    **
    **  \param dt Time elapsed since last update.
    **  \sa ElasticCamera::Update()
    **  \sa ThirdPersonCamera::Update()
    */
    void Update(float dt);
};

#pragma warning(pop)
#endif
