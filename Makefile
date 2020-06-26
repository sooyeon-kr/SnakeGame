OBJECTS = main.o Renderer.o SnakeGame.o Snake.o Stage.o Timer.o
CC=g++
snakegame: $(OBJECTS)
	$(CC) -o snakegame $(OBJECTS) -lncursesw

main.o: main.cpp SnakeGame.h
	$(CC) -c main.cpp

Renderer.o: Renderer.cpp Renderer.h Stage.h Snake.h
	$(CC) -c Renderer.cpp

SnakeGame.o: SnakeGame.cpp SnakeGame.h Renderer.h Timer.h
	$(CC) -c SnakeGame.cpp

Snake.o: Snake.h Snake.cpp SharedDataType.h
	$(CC) -c Snake.cpp

Stage.o: Stage.h Stage.cpp SharedDataType.h
	$(CC) -c Stage.cpp

Timer.o: Timer.h Timer.cpp
	$(CC) -c Timer.cpp

clean:
	rm  $(OBJECTS)
