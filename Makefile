
OS := $(shell uname -s)

ifeq ($(OS), Linux)
  FLAGS = -I/usr/include -I/opt/local/include/eigen3 -L/usr/lib -L/usr/lib/nvidia-304-updates -lglut -lGLEW -lGL -lGLU -lX11 -Wno-write-strings
endif

ifeq ($(OS), Darwin)
  FLAGS = -framework OpenGL -framework GLUT -I/opt/X11/include -I/opt/local/include -I/opt/local/include/eigen3 -L/opt/X11/lib -L/opt/local/lib -lGLEW -lGLUT -Wno-write-strings
endif


Question1: Question1.cpp camera.h light.h timer.h Mesh.* Material.* student_code/shader.h
	g++ Question1.cpp Mesh.cpp Material.cpp -o Question1 $(FLAGS)


Question2_3: Question2_3.cpp camera.h light.h timer.h Material.* Mesh.* Texture.* student_code/shader.h
	g++ Question2_3.cpp Mesh.cpp Texture.cpp Material.cpp -o Question2_3 $(FLAGS)


clean:
	-rm -f Question1 Question2_3 

