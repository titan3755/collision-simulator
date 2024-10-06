all: compile link run

compile:
	g++ -c main.cpp -IC:\sfml_internal\include -DSFML_STATIC

link:
	g++ main.o -o sfml-app -LC:\sfml_internal\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows

run:
	./sfml-app.exe

clean:
	rm -f sfml-app.exe *.o