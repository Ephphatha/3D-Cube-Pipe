#include "Camera.h"

#include <cmath>

Camera::Camera(const Vector3 &position,
               const Vector3 &forward,
               const Vector3 &up,
               const Vector3 &right)
               :
               position(position),
               forward(forward),
               right(right),
               up(up)
{
    CalculateMatrix();
}


void Camera::CalculateMatrix()
{   //Create an orthonormal set of axes from the forward and up vectors.
    this->forward.Normalise();
    this->right = this->forward.Cross(this->up);
    this->right.Normalise();
    this->up = this->right.Cross(this->forward);
    this->up.Normalise();

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
    this->matrix[2] = -this->forward.x;
    this->matrix[6] = -this->forward.y;
    this->matrix[10] = -this->forward.z;
    this->matrix[14] = 0.0f;
    //------------------
    this->matrix[3] = this->matrix[7] = this->matrix[11] = 0.0f;
    this->matrix[15] = 1.0f;
}


const Vector3 Camera::Forward() const
{
    return this->forward;
}

const Vector3 Camera::Right() const
{
    return this->right;
}

const Vector3 Camera::Up() const
{
    return this->up;
}

const Vector3 Camera::Position() const
{
    return this->position;
}


void Camera::Pan(const Quaternion &rotation)
{
    this->forward *= rotation;
    this->right *= rotation;
    this->up *= rotation;
    CalculateMatrix();
}

void Camera::Roll(float angle)
{
    Pan(Quaternion(Forward(), angle));
}

void Camera::Pitch(float angle)
{
    Pan(Quaternion(Right(), angle));
}

void Camera::Yaw(float angle)
{
    Pan(Quaternion(Up(), angle));
}

void Camera::LookAt(const Vector3 &target, const Vector3 &up)
{
    this->forward = target - this->position;
    this->forward.Normalise();

    this->up = up;
    this->up.Normalise();

    CalculateMatrix();
}

void Camera::Dolly(const Vector3 &direction)
{
    this->position += direction;
}

void Camera::Heave(float distance)
{
    Dolly(Up() * distance);
}

void Camera::Surge(float distance)
{
    Dolly(Forward() * distance);
}

void Camera::Sway(float distance)
{
    Dolly(Right() * distance);
}

void Camera::MoveTo(const Vector3 &position)
{
    this->position = position;
}

void Camera::Render() const
{
    glMultMatrixf(this->matrix);
    glTranslatef(-this->position.x, -this->position.y, -this->position.z);
}
