compile:
	g++ subtask2.cpp `pkg-config --cflags --libs opencv`
run:
	./a.out $(empty) $(video)
all:
	g++ subtask2.cpp `pkg-config --cflags --libs opencv`	
	./a.out $(empty) $(video)

