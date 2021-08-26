# Programs and stuff
CC = g++
CCFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17
LDFLAGS =
LDLIBS = -ldpp
RM = rm -rf

# Sources and stuff
CPPSRC = src/main.cpp
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
