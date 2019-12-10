# Project Directories

PRJ_NAME 	:= $(shell basename $(CURDIR))
SRC 		:= src
BLD			:= bld
DEPDIR 		:= $(BLD)/deps

# Application Files
C_SOURCE_FILES 			:= $(shell find ./$(SRC) -type f -name *.c)
C_SOURCE_FILES_BASE 	:= $(notdir $(C_SOURCE_FILES))
C_OBJECT_FILES 			:= $(addprefix $(BLD)/, $(patsubst %.c, %.o, $(C_SOURCE_FILES_BASE)))
C_DEP_FILES 			:= $(addprefix $(DEPDIR)/, $(patsubst %.c, %.d, $(C_SOURCE_FILES_BASE)))

CXX_SOURCE_FILES 		:= $(shell find ./$(SRC) -type f -name *.cpp)
CXX_SOURCE_FILES_BASE 	:= $(notdir $(CXX_SOURCE_FILES))
CXX_OBJECT_FILES 		:= $(addprefix $(BLD)/, $(patsubst %.cpp, %.o, $(CXX_SOURCE_FILES_BASE)))
CXX_DEP_FILES 			:= $(addprefix $(DEPDIR)/, $(patsubst %.cpp, %.d, $(CXX_SOURCE_FILES_BASE)))

SOURCE_FILES			:= $(C_SOURCE_FILES) $(CXX_SOURCE_FILES)
SOURCE_FILES_BASE		:= $(C_SOURCE_FILES_BASE) $(CXX_SOURCE_FILES_BASE)
OBJECT_FILES			:= $(C_OBJECT_FILES) $(CXX_OBJECT_FILES)
ARM_OBJECTS				:= $(patsubst %.o, %.o.arm, $(OBJECT_FILES))
SOURCE_DIRS				:= $(shell find ./$(SRC)/* -type d)
DEP_FILES				:= $(C_DEP_FILES) $(CXX_DEP_FILES)

CC 		:= gcc
CXX		:= g++
ASM_CMD := objdump
IMG_CMD := objcopy

CC_ARM	:= /opt/arm/bin/arm-gcc
CXX_ARM	:= /opt/arm/bin/arm-g++


OPT 			:= -O3 -g
INCLUDE_DIRS 	:= $(addprefix -I, $(SOURCE_DIRS))
WARN			:= -Wall
LINK			:= -pthread
CFLAGS 			:= $(INCLUDE_DIRS) $(OPT) $(WARN) $(LINK)
DEPFLAGS		= -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

# Output Files
EXE := $(PRJ_NAME).exe
ARM	:= arm_$(PRJ_NAME).exe
IMG := $(PRJ_NAME).img

all: $(EXE) $(ARM)

exe: $(EXE)

img: $(IMG)

clean:
	@echo Cleaning
	@rm -rf $(BLD)/*.o $(BLD)/*.o.arm $(EXE) $(IMG)

cleanall:
	@echo Cleaning all
	@rm -rf $(BLD)/* $(EXE) $(IMG)	
	
remake: clean all

$(ARM): $(OBJECT_FILES)
	@echo Linking ARM
	@$(CXX_ARM) $(CFLAGS) -o $@ $(ARM_OBJECTS)

$(EXE): $(OBJECT_FILES)
	@echo Linking x86
	@$(CXX) $(CFLAGS) -o $@ $^

$(IMG): $(EXE)
	@echo Building Image
	@$(IMG_CMD) -Obinary $^ $@

# Generic Rules
$(BLD)/%.o: $(SRC)/**/%.c $(SRC)/**/%.h $(DEPDIR)/%.d | $(DEPDIR)
	@echo Compiling $(@F)
	@$(CC_ARM) $(DEPFLAGS) $(CFLAGS) -o $@.arm -c $<
	@$(CC) $(DEPFLAGS) $(CFLAGS) -o $@ -c $<

$(BLD)/%.o: $(SRC)/**/%.c $(DEPDIR)/%.d | $(DEPDIR)
	@echo Compiling $(@F)
	@$(CC_ARM) $(DEPFLAGS) $(CFLAGS) -o $@.arm -c $<
	@$(CC) $(DEPFLAGS) $(CFLAGS) -o $@ -c $<

$(BLD)/%.o: $(SRC)/**/%.cpp $(SRC)/**/%.h $(DEPDIR)/%.d | $(DEPDIR)
	@echo Compiling $(@F)
	@$(CXX_ARM) $(DEPFLAGS) $(CFLAGS) -o $@.arm -c $<
	@$(CXX) $(DEPFLAGS) $(CFLAGS) -o $@ -c $<

$(BLD)/%.o: $(SRC)/**/%.cpp $(DEPDIR)/%.d | $(DEPDIR)
	@echo Compiling $(@F)
	@$(CXX_ARM) $(DEPFLAGS) $(CFLAGS) -o $@.arm -c $<
	@$(CXX) $(DEPFLAGS) $(CFLAGS) -o $@ -c $<

.PHONY: all exe img clean remake

$(DEP_FILES):

$(DEPDIR):
	mkdir $@

include $(wildcard $(DEP_FILES))