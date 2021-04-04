//
// Created by Lavinia on 4/4/2021.
//

#ifndef HYZU_CAMERA_H
#define HYZU_CAMERA_H
#include <glm/glm.hpp>

class Camera {
private:
    glm::vec3 cameraPosition;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

public:
    Camera(const glm::vec3& cameraPosition,const glm::vec3& cameraFront,const glm::vec3& cameraUp);



    const glm::vec3 &getCameraPosition() const;

    void setCameraPosition(const glm::vec3 &cameraPosition);

    const glm::vec3 &getCameraFront() const;

    void setCameraFront(const glm::vec3 &cameraFront);

    const glm::vec3 &getCameraUp() const;

    void setCameraUp(const glm::vec3 &cameraUp);

private:


};


#endif //HYZU_CAMERA_H
