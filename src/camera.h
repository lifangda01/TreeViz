#pragma once

#include "usda.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

enum Camera_Mode {
    LOOKAT,
    WALKAROUND
};

// Default camera values
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat SPEED      =  0.05f;
const GLfloat SENSITIVTY =  0.1f;
const GLfloat ZOOM       =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    QVector3D Position;
    QVector3D Front;
    QVector3D Up;
    QVector3D Right;
    QVector3D WorldUp;
    // Eular Angles
    GLfloat Yaw;
    GLfloat Pitch;

    GLfloat xRot, yRot;

    // Camera options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;

    // Constructor with vectors
    Camera(QVector3D position = QVector3D(0.0f, 0.0f, 3.0f), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        this->Position = QVector3D(posX, posY, posZ);
        this->WorldUp = QVector3D(upX, upY, upZ);
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    QMatrix4x4 GetViewMatrix(QMatrix4x4 view, Camera_Mode mode = LOOKAT)
    {
        if (mode == WALKAROUND)
            view.lookAt(this->Position, this->Position + this->Front, this->Up);
        else if (mode == LOOKAT)
            view.lookAt(this->Position, QVector3D(0.0f, 0.0f, 0.0f), this->WorldUp);
        return view;
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime = 1.0f)
    {
        GLfloat velocity = this->MovementSpeed * deltaTime;
        if (direction == FORWARD)
            this->Position += this->Front * velocity;
        if (direction == BACKWARD)
            this->Position -= this->Front * velocity;
        if (direction == LEFT)
            this->Position -= this->Right * velocity;
        if (direction == RIGHT)
            this->Position += this->Right * velocity;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, Camera_Mode mode = LOOKAT ,GLboolean constrainPitch = true)
    {
        if (mode == WALKAROUND)
        {
            xoffset *= this->MouseSensitivity;
            yoffset *= this->MouseSensitivity;

            this->Yaw   -= xoffset;
            this->Pitch += yoffset;

            // Make sure that when pitch is out of bounds, screen doesn't get flipped
            if (constrainPitch)
            {
                if (this->Pitch > 89.0f)
                    this->Pitch = 89.0f;
                if (this->Pitch < -89.0f)
                    this->Pitch = -89.0f;
            }

            // Update Front, Right and Up Vectors using the updated Eular angles
            this->updateCameraVectors();
        }
        else if (mode == LOOKAT)
        {
            // Convert position to sphere
            QVector3D sphere; // r, theta, phi
            sphere.setX(qSqrt(qPow(this->Position.x(),2) + qPow(this->Position.y(),2) +
                              qPow(this->Position.z(),2)));
            sphere.setY(qAtan2(this->Position.z() , this->Position.x()));
            sphere.setZ(qAtan2( qSqrt(qPow(this->Position.x(),2) + qPow(this->Position.z(),2)) ,
                                this->Position.y() ));

            // Change phi and theta but keep r
            float theta = sphere.y() + xoffset * 0.005f;
            float phi   = sphere.z() + yoffset * 0.005f;
            phi = (phi >= PI/2 || phi < 0) ? sphere.z() : phi;
            sphere.setZ(phi);
            sphere.setY(theta);

            yRot = -theta / PI * 180.0f /*FIXME*/ - 100.0f;

            // Change sphere back to position cartesian
            this->Position.setX(sphere.x() * qSin(sphere.z()) * qCos(sphere.y()));
            this->Position.setY(sphere.x() * qCos(sphere.z()));
            this->Position.setZ(sphere.x() * qSin(sphere.z()) * qSin(sphere.y()));
        }
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat yoffset)
    {
        float max_zoom = 70.0f;
        if (this->Zoom >= 1.0f && this->Zoom <= max_zoom)
            this->Zoom -= yoffset;
        if (this->Zoom <= 1.0f)
            this->Zoom = 1.0f;
        if (this->Zoom >= max_zoom) //45
            this->Zoom = max_zoom;
    }

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        QVector3D front;
        QVector3D temp;
        front.setX(qCos(qDegreesToRadians(this->Pitch)) * qCos(qDegreesToRadians(this->Yaw)));
        front.setY(qSin(qDegreesToRadians(this->Pitch)));
        front.setZ(qCos(qDegreesToRadians(this->Pitch)) * qSin(qDegreesToRadians(this->Yaw)));
        front.normalize();
        this->Front = front;
        // Also re-calculate the Right and Up vector
        temp = QVector3D::crossProduct(this->Front, this->WorldUp);
        temp.normalize();
        this->Right = temp;  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        temp = QVector3D::crossProduct(this->Right, this->Front);
        temp.normalize();
        this->Up = temp;
    }
};
