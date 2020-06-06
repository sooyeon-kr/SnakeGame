main: main.cpp SnakeGame.h SnakeGame.cpp Snake.h Snake.cpp Renderer.h Renderer.cpp Stage.h Stage.cpp Timer.h Timer.cpp linux_kbhit.h
	g++ -o main main.cpp SnakeGame.cpp Snake.cpp Renderer.cpp Stage.cpp Timer.cpp -lncursesw 
