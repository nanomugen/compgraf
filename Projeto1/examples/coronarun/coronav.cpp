#include "coronav.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void Coronav::initializeGL(GLuint program, int quantity) {
  terminateGL();

  // Start pseudo-random number generator
  auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  m_program = program;
  m_colorLoc = glGetUniformLocation(m_program, "color");
  m_rotationLoc = glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  m_translationLoc = glGetUniformLocation(m_program, "translation");

  
  m_coronav.clear();
  m_coronav.resize(quantity);

  for (auto &corona : m_coronav) {
    corona = createCorona();

    // Criar longe
    do {
      corona.m_translation = {m_randomDist(m_randomEngine),
                                m_randomDist(m_randomEngine)};
    } while (glm::length(corona.m_translation) < 0.5f);
  }
}

void Coronav::paintGL() {
  glUseProgram(m_program);

  for (auto &corona : m_coronav) {
    glBindVertexArray(corona.m_vao);

    glUniform4fv(m_colorLoc, 1, &corona.m_color.r);
    glUniform1f(m_scaleLoc, corona.m_scale);
    glUniform1f(m_rotationLoc, corona.m_rotation);

    for (auto i : {-2, 0, 2}) {
      for (auto j : {-2, 0, 2}) {
        glUniform2f(m_translationLoc, corona.m_translation.x + j,
                    corona.m_translation.y + i);

        glDrawArrays(GL_TRIANGLE_FAN, 0, corona.m_polygonSides + 2);
      }
    }

    glBindVertexArray(0);
  }

  glUseProgram(0);
}

void Coronav::terminateGL() {
  for (auto corona : m_coronav) {
    glDeleteBuffers(1, &corona.m_vbo);
    glDeleteVertexArrays(1, &corona.m_vao);
  }
}

void Coronav::update(float deltaTime) {
  for (auto &corona : m_coronav) {
    corona.m_rotation = glm::wrapAngle(
        corona.m_rotation + corona.m_angularVelocity * deltaTime);
    corona.m_translation += corona.m_velocity * deltaTime;

    // Wrap-around
    if (corona.m_translation.x < -1.0f) corona.m_translation.x += 2.0f;
    if (corona.m_translation.x > +1.0f) corona.m_translation.x -= 2.0f;
    if (corona.m_translation.y < -1.0f) corona.m_translation.y += 2.0f;
    if (corona.m_translation.y > +1.0f) corona.m_translation.y -= 2.0f;
  }
}

Coronav::Corona Coronav::createCorona(glm::vec2 translation,
                                              float scale) {
  Corona corona;

  auto &re{m_randomEngine};  // Shortcut

  // Randomly choose the number of sides
  std::uniform_int_distribution<int> randomSides(35, 60);
  corona.m_polygonSides = randomSides(re);

  corona.m_color = glm::vec4(1,0,0,1) * 0.7f;

  corona.m_color.a = 1.0f;
  corona.m_rotation = 0.0f;
  corona.m_scale = scale;
  corona.m_translation = translation;

  // Choose a random angular velocity
  corona.m_angularVelocity = m_randomDist(re);

  // Choose a random direction
  glm::vec2 direction{m_randomDist(re), m_randomDist(re)};
  corona.m_velocity = glm::normalize(direction)/1.75f;

  // Create geometry
  std::vector<glm::vec2> positions(0);
  positions.emplace_back(0, 0);
  auto step{M_PI * 2 / corona.m_polygonSides};
  std::uniform_real_distribution<float> randomRadius(0.5f, 0.7f);
  int count = 0;
  for (auto angle : iter::range(0.0, M_PI * 2, step)) {
    auto radius{randomRadius(re)};
    if(count%3==0) radius = radius/2;
    count++;
    positions.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
  }
  positions.push_back(positions.at(1));

  // Generate VBO
  glGenBuffers(1, &corona.m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, corona.m_vbo);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
               positions.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  glGenVertexArrays(1, &corona.m_vao);

  // Bind vertex attributes to current VAO
  glBindVertexArray(corona.m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, corona.m_vbo);
  glEnableVertexAttribArray(positionAttribute);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  glBindVertexArray(0);

  return corona;
}
