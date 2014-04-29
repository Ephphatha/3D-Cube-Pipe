/*!
**  \file ElasticCamera.h
**  \brief Defines a Camera class that moves smoothly along a path with a delay.
**
**  B-Spline curve calculation is based on code originally written by Tim Lambert
**   (from http://www.cse.unsw.edu.au/~lambert/splines/source.html)
**
**  \author Andrew James
*/
#ifndef __ElasticCamera
#define __ElasticCamera
#include "DynamicCamera.h"

#include <list>

/*!
**  \class ElasticCamera
**  \brief A camera that moves along a curve defined by a set of control points.
**
**  Could be used for a third person view behind a plane or spaceship, where smooth
**   movement is desired.
**  Will not pan the view, this class should be inherited along with another camera
**   class like ThirdPersonCamera where that sort of automated movement is desired.
**
*/
class ElasticCamera : public virtual DynamicCamera
{
public:
    /*!
    **  \brief Creates an ElasticCamera with the specified position and speed.
    **
    **  \param position     Initial camera position.
    **  \param timeFactor   The "speed" of the camera, setting it to 2 will make it
    **                       travel along a segment in half a second (twice as fast
    **                       as the default) setting it to 0.5 will make the camera
    **                       take two seconds to travel along a curve segment
    */
    ElasticCamera(const Vector3 &position = Vector3(),
                  float timeFactor = 1.0f);

    /*!
    **  \brief Moves the camera along the curve.
    **
    **  Will remove the leading control point every second (at the default timeFactor)
    **   and if the list drops below 4 points, will duplicate the last control point.
    **  \param dt Time elapsed since last update (specify 0 for a static camera).
    */
    virtual void Update(float dt);

    /*!
    **  \brief Adds a point to the internal list of control points.
    **
    **  The camera will most likely not pass through this point exactly unless the
    **   control point is repeated three or more times.
    **  The camera will not move toward this point until at least* 4/timefactor seconds
    **   have passed. To immediately move to a point call ElasticCammera::SetPosition()
    **
    **  * Highly dependant on the number of control points in the list, and the lower
    **     bound is actually more like 2/timeFactor seconds. The message is don't expect
    **     immediate movement using this function.
    **  \param point The new control point.
    **  \sa SetPosition();
    */
    void MoveTo(const Vector3 &point);

    /*!
    **  \brief Moves the camera to the specified position.
    **
    **  Clears the list of points and fills it with this Vector3, InterpolatePosition()
    **   therefore will set the position to the provided Vector3.
    **
    **  \param position The target position.
    */
    void SetPosition(const Vector3 &position);

    /*!
    **  \brief Sets the time factor to the specified value.
    **
    **  Call SetTimeFactor() with no arguments to set it to the default of 1.
    **
    **  \param timeFactor The new time factor (must not be negative).
    */
    void SetTimeFactor(float timeFactor = 1.0f);

    /*!
    **  \brief Adjusts how fast the camera moves along a curve segment.
    **
    **  Adjusts the timeFactor (and therefore the speed) which affects how long it takes
    **   to travel along a curve segment. Values < 1 will slow the camera down, values
    **   > 1 will speed it up, and anything else won't change it.
    **
    **  \param scalar Adjustment value (non-negative real number).
    */
    void AdjustSpeed(float scalar);

protected:
    std::list<Vector3> points;  //!< The list of control points defining the curve.
    float   t,                  //!< Represents how far along the current curve segment we are.
            timeFactor;         //!< Used to speed up and slow down the speed along the curve (at 1.0f a segment is traversed in 1 second).

    /*!
    **  \brief The basis function for a uniform cubic B-Spline
    **
    **  \param i Index of the blending function to use. Domain = {-2, -1, 0, 1}.
    **  \param t Parametric value of the line. Domain = [0, 1].
    */
    static float Basis(int i, float t);

    /*!
    **  \brief Calculates the camera position based on the first four points of the internal list.
    **
    **  The camera tracks along a B-Spline curve, this allows smooth movement but can be computationally expensive.
    **  Since the update function is called only once per frame it shouldn't be much of an issue.
    **
    **  As a B-Spline curve is dependant on four points, the internal list must contain at least 4
    **   points to allow a position to be generated.
    */
    void InterpolatePosition();
};
#endif
