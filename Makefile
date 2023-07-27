CXX       := g++
CXX_FLAGS := -std=c++17 -Wall -Wextra -Werror
LD_FLAGS  :=  -l curl

BIN          := bin
BUILD        := ./build
# LIBRARIES  :=
OBJ_DIR      := $(BUILD)/objects
APP_DIR      := $(BUILD)/apps
TEST_DIR     := $(BUILD)/tests
EXECUTABLE   := forecast
TEST_EXEC    := test_forecast
INCLUDE      := -Iinclude
SRC          := $(wildcard src/*.cpp)
TESTS        := $(wildcard tests/*.cpp)
OBJECTS      := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJECTS := $(TESTS:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

all: build $(APP_DIR)/$(EXECUTABLE) $(TEST_DIR)/$(TEST_EXEC)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -c $< -MMD -o $@

$(APP_DIR)/$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) -o $(APP_DIR)/$(EXECUTABLE) $^ $(LD_FLAGS)

$(TEST_DIR)/$(TEST_EXEC): $(TEST_OBJECTS) $(filter-out $(OBJ_DIR)/src/main.o, $(OBJECTS))
	@mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) -o $(TEST_DIR)/$(TEST_EXEC) $^ $(LD_FLAGS)

-include $(DEPENDENCIES)

.PHONY: all build clean debug release info

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXX_FLAGS += -DDEBUG -ggdb
debug: all

test: $(TEST_DIR)/$(TEST_EXEC)

release: CXX_FLAGS += -02
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
	-@rm -rvf $(TEST_DIR)/*

info:
	@echo "[*] Application dir: $(APP_DIR)     "
	@echo "[*] Tests dir:       $(TEST_DIR)    "
	@echo "[*] Object dir:      $(OBJ_DIR)     "
	@echo "[*] Sources:         $(SRC)         "
	@echo "[*] Objects:         $(OBJECTS)     "
	@echo "[*] Dependencies:    $(DEPENDENCIES)"

