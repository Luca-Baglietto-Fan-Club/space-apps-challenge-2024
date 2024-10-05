rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

BUILD_DIR   := ../build
INC_DIR     := ./include
SRC_DIR     := ./src

BINARY      := $(BUILD_DIR)/pre

SOURCES     := $(call rwildcard, $(SRC_DIR), *.cpp)
OBJECTS     := $(patsubst %.cpp, %.o, $(SOURCES))
DEPENDS     := $(patsubst %.cpp, %.d, $(SOURCES))

LD          := g++
LD_FLAGS    := 

CC          := g++
CC_FLAGS    := -c -Wall -Werror -Wshadow -I$(INC_DIR)

RM          := rm -f
RMDIR       := rm -rf

all: pre

pre: $(BINARY)

run:
	$(BINARY)

clean:
	$(RM) $(OBJECTS) $(DEPENDS) $(BINARY)

$(BINARY): $(OBJECTS)
	mkdir -p $(dir $@)
	$(LD) -o $@ $^ $(LD_FLAGS)

-include $(DEPENDS)

%.o : %.cpp Makefile
	$(info $(OBJECTS))
	$(CC) -o $@ $< $(CC_FLAGS) -MMD -MP

.PHONY: all pre run clean