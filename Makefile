rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

BUILD_DIR   := ./build
INC_DIR     := ./include
SRC_DIR     := ./src
TRAIN_MAIN  := $(SRC_DIR)/train.cpp
TRAIN_OBJ   := $(patsubst %.cpp, %.o, $(TRAIN_MAIN))
RUN_MAIN    := $(SRC_DIR)/run.cpp
RUN_OBJ     := $(patsubst %.cpp, %.o, $(RUN_MAIN))
GLO_MAINS   := $(TRAIN_MAIN) $(RUN_MAIN)

ML_DIR		:= ./ml
ML_INC_DIR  := $(ML_DIR)/include
ML_SRC_DIR  := $(ML_DIR)/src
ML_MAIN     := $(ML_SRC_DIR)/main.cpp

FFT_DIR     := ./fft
FFT_INC_DIR := $(FFT_DIR)/include
FFT_SRC_DIR := $(FFT_DIR)/src
FFT_MAIN    := $(FFT_SRC_DIR)/main.cpp

PRE_DIR     := ./pre
PRE_INC_DIR := $(PRE_DIR)/include
PRE_SRC_DIR := $(PRE_DIR)/src
PRE_MAIN    := $(PRE_SRC_DIR)/main.cpp

TRAIN_BIN   := $(BUILD_DIR)/train
RUN_BIN     := $(BUILD_DIR)/run

ML_SOURCES  := $(filter-out $(ML_MAIN),   $(call rwildcard, $(ML_SRC_DIR), *.cpp))
FFT_SOURCES := $(filter-out $(FFT_MAIN),  $(call rwildcard, $(FFT_SRC_DIR), *.cpp))
PRE_SOURCES := $(filter-out $(PRE_MAIN),  $(call rwildcard, $(PRE_SRC_DIR), *.cpp))
GLO_SOURCES := $(filter-out $(GLO_MAINS), $(call rwildcard, $(SRC_DIR), *.cpp))

SOURCES     := $(ML_SOURCES) $(FFT_SOURCES) $(PRE_SOURCES) $(GLO_SOURCES)
OBJECTS     := $(patsubst %.cpp, %.o, $(SOURCES))
DEPENDS     := $(patsubst %.cpp, %.d, $(SOURCES))
INCLUDES    := $(INC_DIR) $(ML_INC_DIR) $(FFT_INC_DIR) $(PRE_INC_DIR)

LD          := g++
LD_FLAGS    := -lm -lfftw3l

CC          := g++
CC_FLAGS    := -c -Wall -Werror -Wshadow -O2 $(foreach inc, $(INCLUDES), -I$(inc))

RM          := rm -f
RMDIR       := rm -rf

all:
	$(info $(SOURCES))

train: $(TRAIN_BIN)

execute_train:
	$(TRAIN_BIN)

run: $(RUN_BIN)

execute_run:
	$(RUN_BIN)

clean:
	$(RM) $(OBJECTS) $(DEPENDS)
# $(RMDIR) $(BUILD_DIR)

$(TRAIN_BIN): $(TRAIN_OBJ) $(OBJECTS)
	mkdir -p $(dir $@)
	$(LD) -o $@ $^ $(LD_FLAGS)

$(RUN_BIN): $(RUN_OBJ) $(OBJECTS)
	mkdir -p $(dir $@)
	$(LD) -o $@ $^ $(LD_FLAGS)

-include $(DEPENDS)

%.o : %.cpp Makefile
	$(CC) -o $@ $< $(CC_FLAGS) -MMD -MP

.PHONY: all train run execute_train execute_run clean