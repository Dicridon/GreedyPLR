CXX=g++
CXXFLAGS=-O2 -g -std=c++20 -I./src/components
LDFLAGS=
LDLIBS=

TARGET_DIR=./target
TARGET=$(TARGET_DIR)/libplr.so
OBJ_DIR=./obj
SRC_DIR=./src
COMPONENTS_DIR=./src/components

SRC_GREEDY_PLR_GREEDY_PLR=./src/components/greedy_plr/greedy_plr.cpp
SRC_TYPES_TYPES=./src/components/types/types.cpp
SRC_TESTS_TESTS=./src/components/tests/tests.cpp
SRC_ENTITIES_ENTITIES=./src/components/entities/entities.cpp
SRC_OPERATIONS_OPERATIONS=./src/components/operations/operations.cpp
SRC_TEST_ENTITIES=./tests/test_entities.cpp
SRC_TEST_GREEDY=./tests/test_greedy.cpp

HDR_GREEDY_PLR_GREEDY_PLR=./src/components/greedy_plr/greedy_plr.hpp
HDR_TYPES_TYPES=./src/components/types/types.hpp
HDR_TESTS_TESTS=./src/components/tests/tests.hpp
HDR_ENTITIES_ENTITIES=./src/components/entities/entities.hpp
HDR_OPERATIONS_OPERATIONS=./src/components/operations/operations.hpp

OBJ_GREEDY_PLR_GREEDY_PLR=./obj/greedy_plr_greedy_plr.o
OBJ_TYPES_TYPES=./obj/types_types.o
OBJ_TESTS_TESTS=./obj/tests_tests.o
OBJ_ENTITIES_ENTITIES=./obj/entities_entities.o
OBJ_OPERATIONS_OPERATIONS=./obj/operations_operations.o
OBJ_TEST_ENTITIES=./obj/test_entities.o
OBJ_TEST_GREEDY=./obj/test_greedy.o

OUT_OBJS=$(OBJ_GREEDY_PLR_GREEDY_PLR) $(OBJ_TYPES_TYPES) $(OBJ_ENTITIES_ENTITIES) $(OBJ_OPERATIONS_OPERATIONS)
TEST_OBJS=$(OBJ_TESTS_TESTS) $(OBJ_TEST_ENTITIES) $(OBJ_TEST_GREEDY)

TEST_ENTITIES=./target/test_entities
TEST_GREEDY=./target/test_greedy
TESTS=$(TEST_ENTITIES) $(TEST_GREEDY)

GREEDY_PLR_GREEDY_PLR_DEP=$(SRC_GREEDY_PLR_GREEDY_PLR) $(HDR_GREEDY_PLR_GREEDY_PLR) $(OPERATIONS_OPERATIONS_DEP)
TYPES_TYPES_DEP=$(SRC_TYPES_TYPES) $(HDR_TYPES_TYPES)
TESTS_TESTS_DEP=$(SRC_TESTS_TESTS) $(HDR_TESTS_TESTS)
ENTITIES_ENTITIES_DEP=$(SRC_ENTITIES_ENTITIES) $(HDR_ENTITIES_ENTITIES) $(TYPES_TYPES_DEP)
OPERATIONS_OPERATIONS_DEP=$(SRC_OPERATIONS_OPERATIONS) $(HDR_OPERATIONS_OPERATIONS) $(ENTITIES_ENTITIES_DEP)
TEST_ENTITIES_DEP=$(SRC_TEST_ENTITIES) $(HDR_TEST_ENTITIES) $(ENTITIES_ENTITIES_DEP)
TEST_GREEDY_DEP=$(SRC_TEST_GREEDY) $(HDR_TEST_GREEDY) $(GREEDY_PLR_GREEDY_PLR_DEP)

out: $(OUT_OBJS)
	$(CXX) $(CXXFLAGS) -shared -o $(TARGET) $(OUT_OBJS) -fPIC $(LDFLAGS) $(LDLIBS)

test: $(TESTS)

all: out test

$(OBJ_GREEDY_PLR_GREEDY_PLR): $(GREEDY_PLR_GREEDY_PLR_DEP)
	$(CXX) $(CXXFLAGS) -o $@ -c $(SRC_GREEDY_PLR_GREEDY_PLR)

$(OBJ_TYPES_TYPES): $(TYPES_TYPES_DEP)
	$(CXX) $(CXXFLAGS) -o $@ -c $(SRC_TYPES_TYPES)

$(OBJ_TESTS_TESTS): $(TESTS_TESTS_DEP)
	$(CXX) $(CXXFLAGS) -o $@ -c $(SRC_TESTS_TESTS)

$(OBJ_ENTITIES_ENTITIES): $(ENTITIES_ENTITIES_DEP)
	$(CXX) $(CXXFLAGS) -o $@ -c $(SRC_ENTITIES_ENTITIES)

$(OBJ_OPERATIONS_OPERATIONS): $(OPERATIONS_OPERATIONS_DEP)
	$(CXX) $(CXXFLAGS) -o $@ -c $(SRC_OPERATIONS_OPERATIONS)

$(OBJ_TEST_ENTITIES): $(TEST_ENTITIES_DEP)
	$(CXX) $(CXXFLAGS) -o $@ -c $(SRC_TEST_ENTITIES)

$(OBJ_TEST_GREEDY): $(TEST_GREEDY_DEP)
	$(CXX) $(CXXFLAGS) -o $@ -c $(SRC_TEST_GREEDY)


$(TEST_ENTITIES): $(OBJ_TEST_ENTITIES) $(OBJ_ENTITIES_ENTITIES) $(OBJ_TYPES_TYPES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(TEST_GREEDY): $(OBJ_TEST_GREEDY) $(OBJ_ENTITIES_ENTITIES) $(OBJ_TYPES_TYPES) $(OBJ_GREEDY_PLR_GREEDY_PLR) $(OBJ_OPERATIONS_OPERATIONS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)


.PHONY: clean
clean:
	rm ./target/*
	rm ./obj/*.o