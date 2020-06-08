OBJECTS = main.o Renderer.o SnakeGame.o Snake.o Stage.o Timer.o

main: $(OBJECTS)
	g++ -o main $(OBJECTS) -lncursesw 

main.o: main.cpp
	g++ -c main.cpp

Renderer.o: Renderer.h Renderer.cpp Stage.h Snake.h
	g++ -c Renderer.cpp

SnakeGame.o: SnakeGame.h SnakeGame.cpp Renderer.h
	g++ -c SnakeGame.cpp

Snake.o: Snake.h Snake.cpp
	g++ -c Snake.cpp

Stage.o: Stage.h Stage.cpp
	g++ -c Stage.cpp

Timer.o: Timer.h Timer.cpp
	g++ -c Timer.cpp

clean:
	rm  $(OBJECTS)


