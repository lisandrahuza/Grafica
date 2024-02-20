#include "Camera.hpp"

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;

        //TODO - Update the rest of camera parameters

    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraTarget, cameraUpDirection);
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        glm::vec3 cameraFront = glm::normalize(cameraTarget - cameraPosition);
        glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUpDirection, cameraFront));

        if (direction == MOVE_FORWARD) {
            cameraPosition += cameraFront * speed;
            cameraTarget += cameraFront * speed;
        }
        else if (direction == MOVE_BACKWARD) {
            cameraPosition -= cameraFront * speed;
            cameraTarget -= cameraFront * speed;
           // printf("aici");
        }
        else if (direction == MOVE_RIGHT) {
            cameraPosition -= cameraRight * speed;
            cameraTarget -= cameraRight * speed;
        }
        else if (direction == MOVE_LEFT) {
            cameraPosition += cameraRight * speed;
            cameraTarget += cameraRight * speed;
        }

        // You may need to update other camera parameters here if necessary

    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        //TODO
    }
}