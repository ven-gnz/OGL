#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>




Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
  : Front(glm::vec3(0.0f,0.0f,-1.0f)), MovementSpeed(SPEED),MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
 
 {
  Position = position;
  Up = up;
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();

}

glm::mat4 Camera::GetViewMatrix(){  


/*
24.1.
DISCLOSURE : Here was the original implementation. When comparing to source code and trying to
run the program, the camera is obviously not working as intended.

There's some dissecting work to be done here next time I sit down to study. I will seek to improve my solution
iteratively be rereading the chapter start, and only after some time refer to source code.

  glm::vec4 R = glm::vec4(Right,0.0);
  glm::vec4 U = glm::vec4(Up,0.0);
  glm::vec4 D = glm::vec4(Front, 0.0);

  glm::mat4x4 rotation =  {R.x, U.x, D.x, 0.0,
                          R.y, U.y, D.y, 0.0,
                          R.z, U.z, D.z, 0.0,
                          0.0, 0.0, 0.0, 1.0};
  
  glm::mat4x4 translation = {1.0,0.0,0.0, -Position.x,
                        0.0,1.0,0.0, -Position.y,
                        0.0,0.0,1.0, -Position.z,
                        0.0,0.0,0.0,1.0};
  
  return rotation * translation;

  */


}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime){
  float velocity = MovementSpeed * deltaTime;
  if(direction == FORWARD){
    Position +=  Front * velocity;
  }
  if(direction == BACKWARD){
    Position -=  Front * velocity;
  }
  if(direction == LEFT){
    Position -= Right * velocity;
  }
  if(direction == RIGHT){
    Position += Right * velocity;
  }
  // This bit was stolen from the exercise solution. It did not fix my weird bug.
  Position.y = 0.0f;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch){

    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if(constrainPitch){

        if(Pitch > 89.0f) { Pitch = 89.0f; }
        if(Pitch < -89.0f){ Pitch = -89.0f; }
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(front);

  Right = glm::normalize(glm::cross(Front,Up));
  Up = glm::normalize(glm::cross(Right,Front));

}

