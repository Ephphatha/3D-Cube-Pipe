/*!
**  \file DebugObject.h
**  \brief Defines the DebugObject class.
**
**  Just a simple object that indicates the orientation of the world co-ordinates.
**
**  \author Andrew James
*/
#ifndef __DebugObject
#define __DebugObject

/*!
**  \class DebugObject
**  \brief A set of axes used as a development tool.
**
**  (So that I know where the hell the origin is and what direction the axes are pointing.)
*/
class DebugObject
{
public:
    /*!
    **  \brief Draws the axes.
    **
    **  The axes are drawn from the origin in the positive direction, with x set to red,
    **   y set to green and z set to blue. (So the red line points in the +x direction,
    **   the green line in the +y direction and the blue line in the +z direction).
    */
    void Draw() const;
};
#endif
