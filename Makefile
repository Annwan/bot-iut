# Programs and stuff
CC = g++
CCFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17
LDFLAGS =
LDLIBS = -ldpp -lsqlite3 -lfmt
RM = rm -rf

# Sources and stuff
CPPSRC = src/main.cpp src/config.cpp src/log.cpp
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
build/config.o: src/config.h src/log.h
build/log.o: src/log.h
