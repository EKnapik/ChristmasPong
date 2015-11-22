# Ma Make File
pong: main.o shaderSetup.o render.o
		gcc -o pong main.o shaderSetup.o render.o -framework OpenGL -framework GLUT

shaderSetup.o: shaderSetup.c shaderSetup.h
		gcc -c -std=c99 shaderSetup.c

render.o: render.c render.h shaderSetup.c shaderSetup.h
		gcc -c -std=c99 render.c

main.o: main.c render.c render.h
		gcc -c -std=c99 main.c

clean:
		$(RM) pong *.o
