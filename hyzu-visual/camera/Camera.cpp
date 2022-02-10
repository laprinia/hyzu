//
// Created by Lavinia
//

#include "Camera.h"

Camera::Camera(const glm::vec3 &cameraPosition, const glm::vec3 &cameraFront, const glm::vec3 &cameraUp) {
    this->cameraFront = cameraFront;
    this->cameraPosition = cameraPosition;
    this->cameraUp = cameraUp;
    this->fieldOfView = 45.0f;
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

float Camera::getFieldOfView() const {
    return fieldOfView;
}

void Camera::setFieldOfView(float fieldOfView) {
    Camera::fieldOfView = fieldOfView;
}



