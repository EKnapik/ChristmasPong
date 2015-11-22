# Ma Make File
pong: main.o shaderSetup.o
		gcc -o pong main.o shaderSetup.o -framework OpenGL -framework GLUT

shaderSetup.o: shaderSetup.c shaderSetup.h
		gcc -c -std=c99 shaderSetup.c

main.o: main.c shaderSetup.c shaderSetup.h
		gcc -c -std=c99 main.c

clean:
		$(RM) pong *.o
