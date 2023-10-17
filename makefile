CC=g++
C_FLAGS=-Wall -Wextra -Wno-unknown-pragmas -std=c++20
VAL_FLAGS=--leak-check=full

SRC=src
TEST=tests
OBJ=build/obj
BIN=build/bin

OUTBIN=$(BIN)/main
TESTBIN=$(BIN)/tester

SRCS=$(wildcard $(SRC)/*.cpp)
OBJS=$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))

TSRCS=$(wildcard $(TEST)/*.cpp)
TOBJS=$(patsubst $(TEST)/%.cpp, $(OBJ)/tests/%.o, $(TSRCS))


.PHONY: all
.PHONY: build
.PHONY: build-tests
.PHONY: release
.PHONY: debug
.PHONY: memleak
.PHONY: clean
.PHONY: loc


all: build
build: $(OUTBIN)
build-tests: $(TESTBIN)

release: C_FLAGS=-Wall -Wextra -Wno-unknown-pragmas -std=c++20 -O2
release: clean
release: $(OUTBIN)



$(OUTBIN): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(C_FLAGS) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(C_FLAGS) -c $< -o $@



test: $(TESTBIN)
	clear
	@echo Test results:
	@./$(TESTBIN)

$(TESTBIN): C_FLAGS=-Wall -Wextra -ggdb -Wno-unknown-pragmas -std=c++20 -D TEST_BUILD
$(TESTBIN): $(TOBJS) $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(C_FLAGS) $^ -o $@

$(OBJ)/tests/%.o: $(TEST)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(C_FLAGS) -c $< -o $@



debug: $(TESTBIN)
	gdb ./$(TESTBIN)

memleak: $(TESTBIN)
	valgrind $(VAL_FLAGS) $(TESTBIN)


clean:
	$(RM) -r $(OBJ) $(BIN)

loc:
	scc -s lines --no-cocomo --no-gitignore -w --size-unit binary --exclude-ext md,makefile,json --exclude-dir tests/framework
