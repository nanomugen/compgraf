project(coronarun)

add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp coronav.cpp
                               aluno.cpp )

enable_abcg(${PROJECT_NAME})
