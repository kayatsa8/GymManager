
all: clean compile link run

run: bin/studio
	bin/studio

link:
	@echo "Linking all components"
	g++ -o bin/studio bin/main.o bin/Action.o bin/Customer.o bin/Studio.o bin/Trainer.o bin/Workout.o

compile:
	@echo "Compiling"
	g++ -g -c -Wall -Weffc++ -std=c++11 -o bin/main.o src/main.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -o bin/Action.o src/Action.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -o bin/Customer.o src/Customer.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -o bin/Studio.o src/Studio.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -o bin/Trainer.o src/Trainer.cpp
	g++ -g -c -Wall -Weffc++ -std=c++11 -o bin/Workout.o src/Workout.cpp

clean:
	@echo "System cleaning bin directory"
	rm -rf bin/*
