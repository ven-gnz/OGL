#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>




Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
  : Front(glm::vec3(0.0f,0.0f,-1.0f)), MovementSpeed(SPEED),MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
 
 {
  Position = position;
  Up = up;
  WorldUp = glm::vec3(Up.x,Up.y,Up.z);
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

 glm::vec3 p = Position;
 glm::vec3 target = glm::vec3(0.0f,0.0f,0.0f);
 glm::vec3 d = glm::normalize(p - target);
 glm::vec3 u = WorldUp;
 glm::vec3 r = glm::normalize(glm::cross(u,d));
 glm::vec3 cameraUp = glm::cross(d,r);

 glm::mat4 rotation = {
  r.x, u.x, d.x, 0.0,
  r.y, u.y, d.y, 0.0,
  r.z, u.z, d.z, 0.0,
  0.0, 0.0, 0.0, 1.0
 };

 glm::mat4 translation = {
  1.0, 0.0, 0.0, 0.0,
  0.0, 1.0, 0.0, 0.0,
  0.0, 0.0, 1.0, 0.0,
  -p.x, -p.y, -p.z, 1.0
 };

 return rotation * translation;


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


/*
EDIT 27.1.

Due to the feedback in tsemppirinki, I figured out the mistake. The world up was read from up, ie updated from current up value. Maybe implementing 
this I had some semantics as to why, that now can only be found in the cultural heritage archives of alternative realities.
Therefore we had two mistakes: the worldup must remain static, and due to the orthonormalization (Gram-Schmidt process) working as intended, the
angles had to line up. Therefore the camera vector complied.

Good stuff and good catch from tsemppirinki!
*/
void Camera::updateCameraVectors()
{
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(front);

  Right = glm::normalize(glm::cross(Front,WorldUp));
  Up = glm::normalize(glm::cross(Right,Front));

}

