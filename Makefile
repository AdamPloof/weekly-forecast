CXX       := g++
CXX_FLAGS := -std=c++17 -Wall -Wextra -Werror
LD_FLAGS  :=  -l curl

BIN          := bin
BUILD        := ./build
# LIBRARIES    :=
OBJ_DIR      := $(BUILD)/objects
APP_DIR     := $(BUILD)/apps
EXECUTABLE   := forecast
INCLUDE      := -Iinclude
SRC          := $(wildcard src/*.cpp)
OBJECTS      := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

# TEST_EXEC := tests
# TEST_SRC  := test

all: build $(APP_DIR)/$(EXECUTABLE)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -c $< -MMD -o $@

$(APP_DIR)/$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) -o $(APP_DIR)/$(EXECUTABLE) $^ $(LD_FLAGS)

-include $(DEPENDENCIES)

.PHONY: all build clean debug release info

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXX_FLAGS += -DDEBUG -ggdb
debug: all

release: CXX_FLAGS += -02
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*

info:
	@echo "[*] Application dir: $(APP_DIR)     "
	@echo "[*] Object dir:      $(OBJ_DIR)     "
	@echo "[*] Sources:         $(SRC)         "
	@echo "[*] Objects:         $(OBJECTS)     "
	@echo "[*] Dependencies:    $(DEPENDENCIES)"

