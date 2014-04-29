/*!
**  \file ElasticShakyThirdPersonCamera.h
**  \brief Defines an observer camera that will move smoothly along a curve while constantly looking at a target point.
**
**  \author Andrew James
*/
#ifndef __ElasticShakyThirdPersonCamera
#define __ElasticShakyThirdPersonCamera
#include "ElasticCamera.h"
#include "ShakyCamera.h"
#include "ThirdPersonCamera.h"

#pragma warning(push)
#pragma warning(disable:4250)
// Because of the way we're inheriting camera functionality we get a warning about
//  functions being overriden via dominance, this can safely be ignored.

/*!
**  \class ElasticShakyThirdPersonCamera
**  \brief Combines the functionality of ThirdPersonCamera, ShakyCamera and ElasticCamera.
**
**  Creates a camera best used for a flyby through a Micheal Bay movie.
*/
class ElasticShakyThirdPersonCamera : public ElasticCamera, public ShakyCamera, public ThirdPersonCamera
{
public:
    /*!
    **  \brief Creates a camera at the specified initial position, looking at the target.
    **
    **  Defaults to sitting at the origin, looking down the -z axis with +y up.
    **
    **  \param position     Initial position of the camera.
    **  \param timeFactor   "Speed" of the camera.
    **  \param strength     Strength of the shake affect.
    **  \param rate         How often the shake effect is calculated per second.
    **  \param target       Focal point of the camera.
    **  \param targetUp     Local up direction.
    */
    ElasticShakyThirdPersonCamera(const Vector3 &position = Vector3(),
                                  float timeFactor = 1.0f,
                                  float strength = 1.0f,
                                  float rate = 60.0f,
                                  const Vector3 &target = Vector3(0.0f, 0.0f, -1.0f),
                                  const Vector3 &targetUp = Vector3(0.0f, 1.0f, 0.0f));

    /*!
    **  \brief Moves the camera along the path, applies the shake affect, then rotates to look at the target again.
    **
    **  \param dt Time elapsed since last update.
    **  \sa ElasticCamera::Update()
    **  \sa ThirdPersonCamera::Update()
    */
    void Update(float dt);
};

#pragma warning(pop)
#endif
