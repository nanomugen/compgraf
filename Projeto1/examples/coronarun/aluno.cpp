#include "aluno.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Aluno::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = glGetUniformLocation(m_program, "color");
  m_rotationLoc = glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  m_translationLoc = glGetUniformLocation(m_program, "translation");

  m_rotation = 0.0f;
  m_translation = glm::vec2(0);
  m_velocity = glm::vec2(0);

  // clang-format off
  std::array<glm::vec2, 24> positions{
      //body
      //0                       1
      glm::vec2{-0.5f, +4.0f}, glm::vec2{0.5f, +4.0f},
      //2                         3
      glm::vec2{-01.25f, +3.0f}, glm::vec2{+01.25f, +3.0f},
      //4                       5
      glm::vec2{-0.75f, +2.0f}, glm::vec2{+0.75f, +2.0f},
      //6                       7
      glm::vec2{-1.5f, +1.5f}, glm::vec2{+1.5f, +1.5f},
      //8                       9
      glm::vec2{-3.0f, -1.0f}, glm::vec2{+3.0f, -1.0f},
      //10                    11
      glm::vec2{-1.25f, +0.25f}, glm::vec2{+1.25f, +0.25f},
      //12                      13
      glm::vec2{-2.0f, -04.0f}, glm::vec2{+2.0f, -4.0f},
      //14
      glm::vec2{+0.0f, -2.0f},
      };

  // Normalize
  for (auto &position :positions) {
    position /= glm::vec2{8.0f, 8.0f};
  }

  std::array indices{0, 1, 3,
                     0, 2, 3,

                     4, 5, 7,
                     4, 6, 7,
                     6, 7, 10,
                     6, 8, 10,
                     7, 10, 11,
                     7, 9, 11,
                     10, 11, 12,
                     10, 11, 13,
                     2, 3, 4,
                     3, 4, 5,
                     };

  // Generate VBO
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  glGenBuffers(1, &m_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  glBindVertexArray(m_vao);

  glEnableVertexAttribArray(positionAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  // End of binding to current VAO
  glBindVertexArray(0);
}

void Aluno::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  glUseProgram(m_program);

  glBindVertexArray(m_vao);

  glUniform1f(m_scaleLoc, m_scale);
  glUniform1f(m_rotationLoc, m_rotation);
  glUniform2fv(m_translationLoc, 1, &m_translation.x);

  glUniform4fv(m_colorLoc, 1, &m_color.r);
  glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);

  glUniform4f(m_colorLoc,1.0f,1.0f,1.0f,1.0f);

  glDrawElements(GL_TRIANGLES, 10 * 3, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);


  glUseProgram(0);
}

void Aluno::terminateGL() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}

void Aluno::update(const GameData &gameData, float deltaTime) {
  if (gameData.m_input[static_cast<size_t>(Input::Left)] && m_translation.x >= -0.90f)
    m_translation += glm::vec2(-1.0f,0.0f) * deltaTime;
  if (gameData.m_input[static_cast<size_t>(Input::Right)] && m_translation.x <= +0.90f)
    m_translation += glm::vec2(1.0f,0.0f) * deltaTime;
  if (gameData.m_input[static_cast<size_t>(Input::Up)] && m_translation.y <= +0.87f)
    m_translation += glm::vec2(0.0f,1.0f) * deltaTime;
  if (gameData.m_input[static_cast<size_t>(Input::Down)] && m_translation.y >= -0.87f)
    m_translation += glm::vec2(0.0f,-1.0f) * deltaTime;
}
