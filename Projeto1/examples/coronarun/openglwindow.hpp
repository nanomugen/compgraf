#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "coronav.hpp"
#include "aluno.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_objectsProgram{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  GameData m_gameData;

  Coronav m_coronav;
  Aluno m_aluno;
  

  abcg::ElapsedTimer m_restartWaitTimer;
  abcg::ElapsedTimer m_time;
  float m_points{};
  ImFont* m_font{};

  std::default_random_engine m_randomEngine;

  void checkCollisions();

  void restart();
  void update();
};

#endif