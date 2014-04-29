/*!
**  \file ThirdPersonChaseCamera.h
**  \brief Extends the ThirdPersonCamera class to chase the target at a specified offset.
**
**  Will constantly look at the target point, and will move to remain at a certain position relative to that point.
**  \author Andrew James
*/
#ifndef __ThirdPersonChaseCamera
#define __ThirdPersonChaseCamera
#include "ThirdPersonCamera.h"

#include "Quaternion.h"

/*!
**  \class ThirdPersonChaseCamera
**  \brief Camera that emulates following someone.
*/
class ThirdPersonChaseCamera : public ThirdPersonCamera
{
public:
    /*!
    **  \brief Creates a ThirdPersonCamera that is a certain distance away from the target and offset.
    **
    **  By default creates a camera that looks down on the origin from 45° elevation in the +z direction.
    **
    **  \param position         The camera position.
    **  \param target           The focal point of the camera.
    **  \param targetUp         The local up direction.
    **  \param targetForward    The forward direction of the target (used as a basis for the offset)
    **  \param orientation      The desired offset specified as a rotation around an axis
    **                           (can be calculated with the dot and cross product of the forward and offset vectors).
    **  \param distance         The distance between the camera and the target.
    **  \todo The last three parameters are unnecessary, an offset can be calculated from the position
    **         and target position.
    */
    ThirdPersonChaseCamera(const Vector3 &position = Vector3(),
                           const Vector3 &target = Vector3(0.0f, 0.0f, -1.0f),
                           const Vector3 &targetUp = Vector3(0.0f, 1.0f, 0.0f),
                           const Vector3 &targetForward = Vector3(0.0f, 0.0f, 1.0f),
                           const Quaternion& orientation = Quaternion(Vector3(1.0f, 0.0f, 0.0f),
                                                                      Quaternion::DegreesToRadians(45)),
                           float distance = 0.0f);

    /*!
    **  \brief Moves the camera to the desired offset and makes it look at the target.
    */
    virtual void Update(float dt);

protected:
    Vector3 targetForward;  //!< The forward vector of the target.
    Quaternion orientation; //!< Quaternion describing the rotation needed to get from the above vector to the final position.
    float chaseDistance;    //!< Magnitude of the offset vector.
};
#endif
