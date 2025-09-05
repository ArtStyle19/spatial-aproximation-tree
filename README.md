sudo dnf install freeglut freeglut-devel glfw glfw-devel mesa-libGLU-devel
g++ main.cpp -o modo_grafico -lGL -lGLU -lglut -lglfw -lm
