compile:
	g++ subtask1.cpp `pkg-config --cflags --libs opencv`
run:
	./a.out $(file)
all:
	g++ subtask1.cpp `pkg-config --cflags --libs opencv`	
	./a.out $(file)

