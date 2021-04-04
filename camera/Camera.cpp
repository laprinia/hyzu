//
// Created by Lavinia on 4/4/2021.
//

#include "Camera.h"

Camera::Camera(const glm::vec3 &cameraPosition, const glm::vec3 &cameraFront, const glm::vec3 &cameraUp) {
    this->cameraFront=cameraFront;
    this->cameraPosition=cameraPosition;
    this->cameraUp=cameraUp;
}

const glm::vec3 &Camera::getCameraPosition() const {
    return cameraPosition;
}

void Camera::setCameraPosition(const glm::vec3 &cameraPosition) {
    Camera::cameraPosition = cameraPosition;
}

const glm::vec3 &Camera::getCameraFront() const {
    return cameraFront;
}

void Camera::setCameraFront(const glm::vec3 &cameraFront) {
    Camera::cameraFront = cameraFront;
}

const glm::vec3 &Camera::getCameraUp() const {
    return cameraUp;
}

void Camera::setCameraUp(const glm::vec3 &cameraUp) {
    Camera::cameraUp = cameraUp;
}



