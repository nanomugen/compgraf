#ifndef ALUNO_HPP_
#define ALUNO_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class Coronav;
class OpenGLWindow;

class Aluno {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(const GameData &gameData, float deltaTime);
  void setRotation(float rotation) { m_rotation = rotation; }

 private:
  friend Coronav;
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  glm::vec4 m_color{glm::vec4(0.4f,0.8f,0.4f,1.0f)};
  float m_rotation{};
  float m_scale{0.25f};
  glm::vec2 m_translation{glm::vec2(0)};
  glm::vec2 m_velocity{glm::vec2(0)};
};

#endif