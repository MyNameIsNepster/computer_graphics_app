all:
	gcc -Iinclude src/*   -lSOIL -lglu32 -lopengl32 -lglut32 -lm -o beadando.exe -Wall -Wextra -Wno-implicit -Wno-unused
linux:
	gcc -Iinclude src/*  -lSOIL -lGL -lGLU -lglut -lm -o beadando.exe -Wall -Wextra -Wno-implicit -Wno-unused