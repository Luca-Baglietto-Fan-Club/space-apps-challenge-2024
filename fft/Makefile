rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

BUILD_DIR   := ../build
GLO_INC_DIR := ../include
INC_DIR     := ./include
SRC_DIR     := ./src

BINARY      := $(BUILD_DIR)/fft

SOURCES     := $(call rwildcard, $(SRC_DIR), *.cpp)
OBJECTS     := $(patsubst %.cpp, %.o, $(SOURCES))
DEPENDS     := $(patsubst %.cpp, %.d, $(SOURCES))

LD          := g++
LD_FLAGS    := -lfftw3 -lm

CC          := g++
CC_FLAGS    := -c -Wall -Werror -Wshadow -I$(INC_DIR)

RM          := rm -f
RMDIR       := rm -rf

all: fft

fft: $(BINARY)

run:
	$(BINARY)

clean:
	$(RM) $(OBJECTS) $(DEPENDS) $(BINARY)

$(BINARY): $(OBJECTS)
	mkdir -p $(dir $@)
	$(LD) -o $@ $^ $(LD_FLAGS)

-include $(DEPENDS)

(BUILD_DIR)/%.o : (SRC_DIR)/%.cpp Makefile
	$(info $(OBJECTS))
	$(CC) -o $@ $< $(CC_FLAGS) -MMD -MP

.PHONY: all fft run clean