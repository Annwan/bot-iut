# Programs and stuff
CC = g++
CCFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17
LDFLAGS =
LDLIBS = -ldpp -lsqlite3
RM = rm -rf

# Sources and stuff
CPPSRC = src/main.cpp src/log.cpp src/config.cpp
CPPOBJ = $(CPPSRC:src/%.cpp=build/%.o)
EXEC = build/main.out

# House keeping rules
.PHONY: build run clean mrproper

build: $(EXEC)

run: build
	./$(EXEC)

clean:
	$(RM) $(CPPOBJ)

mrproper: clean
	$(RM) $(EXEC)

# Building stuff

$(EXEC): $(CPPOBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

build/%.o: src/%.cpp
	$(CC) $(CCFLAGS) -c $< -o $@

build/main.o: src/log.h src/config.h
build/log.o: src/log.h
build/config.o: src/config.h src/log.h
