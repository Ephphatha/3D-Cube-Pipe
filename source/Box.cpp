#include "Box.h"

#pragma comment(lib, "OpenGL32.lib")

#include <cmath>

const GLfloat Box::vertices[72] =
{
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
};

const GLfloat Box::normals[72] =
{
     1.0f,  0.0f,  0.0f,
     1.0f,  0.0f,  0.0f,
     1.0f,  0.0f,  0.0f,
     1.0f,  0.0f,  0.0f,

     0.0f,  1.0f,  0.0f,
     0.0f,  1.0f,  0.0f,
     0.0f,  1.0f,  0.0f,
     0.0f,  1.0f,  0.0f,

     0.0f,  0.0f,  1.0f,
     0.0f,  0.0f,  1.0f,
     0.0f,  0.0f,  1.0f,
     0.0f,  0.0f,  1.0f,

    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,

     0.0f, -1.0f,  0.0f,
     0.0f, -1.0f,  0.0f,
     0.0f, -1.0f,  0.0f,
     0.0f, -1.0f,  0.0f,

     0.0f,  0.0f, -1.0f,
     0.0f,  0.0f, -1.0f,
     0.0f,  0.0f, -1.0f,
     0.0f,  0.0f, -1.0f,
};

const GLfloat Box::colours[72] =
{
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,

    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,

    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
};

Box::Box():angle(),axis(),next(),prev(),isActive(false)
{
}

Box::Box(const Box &rhs):angle(rhs.angle),axis(rhs.axis),next(),prev(),isActive(false)
{
    SetNext(rhs.next);  // These two calls will most likely fail, we want this to happen.
    SetPrev(rhs.prev.lock());
}

Box::Box(const float &_angle, const Vector3 &_axis):angle(_angle),axis(_axis),next(),prev(),isActive(false)
{
}

void Box::operator=(const Box &rhs)
{
    this->angle = rhs.angle;
    this->axis = rhs.axis;
    this->isActive = false; // Even if the rhs box is active, we only want one active box at a time.

    SetNext(rhs.next);  // These two calls will most likely fail, we want this to happen.
    SetPrev(rhs.prev.lock());
}

void Box::Draw() const
{
    glPushMatrix();

    glTranslatef(this->axis.x, this->axis.y, this->axis.z);

    glRotatef(angle, this->axis.x, this->axis.y, this->axis.z);

    if(this->isActive)
    {
        glDrawArrays(GL_LINES, 0, 24);
    }
    else
    {
        glDrawArrays(GL_QUADS, 0, 24);
    }

    if(this->next)
    {   // Here's the trick! Drawing the elements recursively applies any
        //  rotations to this box and all boxes further down the list.
        this->next->Draw();
    }

    glPopMatrix();
}

void Box::Rotate(const float &angle)
{
    if(this->axis != Vector3(0))
    {   // Causes funny things to happen if you try rotate the head node.
        this->angle += angle;
    }
}

bool Box::SetNext(const boost::shared_ptr<Box> &next)
{
    if(!next || ((&(*next) != this) && (next->axis != -this->axis)))
    {   // Make sure you don't end up with a self reference,
        //  otherwise the memory will not be freed properly.
        // If the pointer is a null pointer we must be dropping
        //  an element of the list, so allow it.
        this->next = next;

        return true;
    }

    return false;
}

bool Box::SetPrev(const boost::shared_ptr<Box> &prev)
{
    if(prev && (&(*(prev->Next())) == this))
    {   //Make sure the previous pointer is valid, and actually links to this box.
        this->prev = prev;

        return true;
    }

    return false;
}

const boost::shared_ptr<Box> Box::Next(void)
{
    return this->next;
}

const boost::shared_ptr<Box> Box::Prev(void)
{
    return this->prev.lock();
}

void Box::Active(const bool &isActive)
{
    this->isActive = isActive;
}


MasterBox::MasterBox():Box(),position(),forward(0.0f, 0.0f, 1.0f),right(1.0f, 0.0f, 0.0f),up(0.0f, 1.0f, 0.0f)
{
    CalculateMatrix();
}

MasterBox::MasterBox(const Box &rhs):Box(rhs),position(),forward(0.0f, 0.0f, 1.0f),right(1.0f, 0.0f, 0.0f),up(0.0f, 1.0f, 0.0f)
{
    this->axis = Vector3();
    this->angle = 0.0f;
    CalculateMatrix();
}

MasterBox::MasterBox(const MasterBox &rhs):Box(rhs),position(rhs.position),forward(rhs.forward),right(rhs.right),up(rhs.up)
{
    CalculateMatrix();
}

MasterBox::MasterBox(const Vector3 &_position):Box(),position(_position),forward(0.0f, 0.0f, 1.0f),right(1.0f, 0.0f, 0.0f),up(0.0f, 1.0f, 0.0f)
{
    CalculateMatrix();
}

MasterBox& MasterBox::operator=(const MasterBox &rhs)
{
    this->Box::operator=(rhs);
    this->position = rhs.position;

    return *this;
}

const Vector3 MasterBox::Forward() const
{
    return this->forward;
}

const Vector3 MasterBox::Right() const
{
    return this->right;
}

const Vector3 MasterBox::Up() const
{
    return this->up;
}

void MasterBox::Dolly(const Vector3 &direction)
{
    this->position += direction;
}

void MasterBox::Roll(float angle)
{
    Pan(Quaternion(Forward(), angle));
}

void MasterBox::Pitch(float angle)
{
    Pan(Quaternion(Right(), angle));
}

void MasterBox::Yaw(float angle)
{
    Pan(Quaternion(Up(), angle));
}

void MasterBox::Pan(const Quaternion &rotation)
{
    this->up *= rotation;
    this->right *= rotation;
    this->forward *= rotation;
    CalculateMatrix();
}

void MasterBox::Draw() const
{
    glPushMatrix();

    glTranslatef(this->position.x, this->position.y, this->position.z);

    glMultMatrixf(this->matrix);

    if(this->isActive)
    {
        glDrawArrays(GL_LINES, 0, 24);
    }
    else
    {
        glDrawArrays(GL_QUADS, 0, 24);
    }

    if(this->next)
    {   // Here's the trick! Drawing the elements recursively applies any
        //  rotations to this box and all boxes further down the list.
        this->next->Draw();
    }

    glPopMatrix();
}

void MasterBox::CalculateMatrix()
{   //Create an orthonormal set of axes from the forward and up vectors.
    this->forward.Normalise();
    this->up = this->forward.Cross(this->right);
    this->up.Normalise();
    this->right = this->up.Cross(this->forward);
    this->right.Normalise();

    this->matrix[0] = this->right.x;
    this->matrix[4] = this->right.y;
    this->matrix[8] = this->right.z;
    this->matrix[12] = 0.0f;
    //------------------
    this->matrix[1] = this->up.x;
    this->matrix[5] = this->up.y;
    this->matrix[9] = this->up.z;
    this->matrix[13] = 0.0f;
    //------------------
    this->matrix[2] = this->forward.x;
    this->matrix[6] = this->forward.y;
    this->matrix[10] = this->forward.z;
    this->matrix[14] = 0.0f;
    //------------------
    this->matrix[3] = this->matrix[7] = this->matrix[11] = 0.0f;
    this->matrix[15] = 1.0f;
}
