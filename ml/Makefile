rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

BUILD_DIR   := ../build/ml
INC_DIR     := ./include
SRC_DIR     := ./src

BINARY      := $(BUILD_DIR)/bin

SOURCES     := $(call rwildcard, $(SRC_DIR), *.cpp)
OBJECTS     := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
DEPENDS     := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.d, $(SOURCES))

LD          := g++
LD_FLAGS    := 

CC          := g++
CC_FLAGS    := -c -Wall -Werror -Wshadow -I$(INC_DIR)

RM          := rm -f
RMDIR       := rm -rf

all: ml

ml: $(BINARY)

run:
	$(BINARY)

clean:
	$(RMDIR) $(BUILD_DIR)

$(BINARY): $(OBJECTS)
	mkdir -p $(dir $@)
	$(LD) -o $@ $^ $(LD_FLAGS)

-include $(DEPENDS)

%.o : %.cpp Makefile
	$(info $(OBJECTS))
	$(CC) -o $@ $< $(CC_FLAGS) -MMD -MP

.PHONY: all ml run clean