TOP = .
include $(TOP)/Makefile.build
include $(TOP)/Makefile.func

TARGET = app

######################################
# path
######################################
OUTPUT_PATH	= $(TOP)/out
BIN_PATH = $(OUTPUT_PATH)/bin
OBJ_PATH = $(OUTPUT_PATH)/obj

include $(TOP)/src/source.in

ASM_SOURCES ?= $(filter %.s,$(SOURCES))
C_SOURCES ?= $(filter %.c,$(SOURCES))
CPP_SOURCES ?= $(filter %.cpp,$(SOURCES))

vpath
vpath %.s $(sort $(dir $(ASM_SOURCES)))
vpath %.c $(sort $(dir $(C_SOURCES)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

OBJECT_FILE = $(subst src,$(OBJ_PATH),$(C_SOURCES:.c=.o)) $(subst src,$(OBJ_PATH),$(ASM_SOURCES:.s=.o)) $(subst src,$(OBJ_PATH),$(CPP_SOURCES:.cpp=.o))

.PHONY: rebuild all clean
all: $(OBJECT_FILE) Makefile | $(OBJ_PATH)
	@$(MAKE) $(BIN_PATH)/$(TARGET).elf --no-print-directory
	@$(MAKE) $(BIN_PATH)/$(TARGET).hex --no-print-directory
	@$(MAKE) $(BIN_PATH)/$(TARGET).bin --no-print-directory

clean:
	@$(RM) -rf $(OBJ_PATH)
	@$(RM) -rf $(BIN_PATH)

rebuild:
	@$(MAKE) clean --no-print-directory
	@$(MAKE) all --no-print-directory

$(OBJ_PATH) $(BIN_PATH):
	@$(MKDIR) -p $@

$(OBJ_PATH)/%.o: %.cpp Makefile | $(OBJ_PATH)
	@$(call PRINT_COMPILE_CPP,"$<")
	@$(MKDIR) -p $(dir $@)
	@$(ECHO) $(CXX) -c $(CXXFLAGS) $(LOCAL_CXXFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -Wa,-a,-ad,-alms=$(@:.o=.lst) $< -o $@
	@$(CXX) -E $(CXXFLAGS) $(LOCAL_CXXFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -Wa,-a,-ad,-alms=$(@:.o=.lst) $< -o $@.i
	@$(CXX) -c $(CXXFLAGS) $(LOCAL_CXXFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -Wa,-a,-ad,-alms=$(@:.o=.lst) $< -o $@

$(OBJ_PATH)/%.o: %.c Makefile | $(OBJ_PATH)
	@$(call PRINT_COMPILE_C,"$<")
	@$(MKDIR) -p $(dir $@)
	@$(ECHO) $(CC) -c $(CFLAGS) $(LOCAL_CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -Wa,-a,-ad,-alms=$(@:.o=.lst) $< -o $@
	@$(CC) -E $(CFLAGS) $(LOCAL_CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -Wa,-a,-ad,-alms=$(@:.o=.lst) $< -o $@.i
	@$(CC) -c $(CFLAGS) $(LOCAL_CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -Wa,-a,-ad,-alms=$(@:.o=.lst) $< -o $@

$(OBJ_PATH)/%.o: %.s Makefile | $(OBJ_PATH)
	@$(call PRINT_COMPILE_ASM,"$<")
	@$(MKDIR) -p $(dir $@)
	@$(ECHO) $(AS) -c $(ASFLAGS) $(LOCAL_ASFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -Wa,-a,-ad,-alms=$(@:.o=.lst) $< -o $@
	@$(CC) -E $(ASFLAGS) $(LOCAL_ASFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -Wa,-a,-ad,-alms=$(@:.o=.lst) $< -o $@.i
	@$(AS) -c $(ASFLAGS) $(LOCAL_ASFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -Wa,-a,-ad,-alms=$(@:.o=.lst) $< -o $@

$(BIN_PATH)/$(TARGET).elf: $(OBJECT_FILE) Makefile | $(BIN_PATH)
	@$(call PRINT_COMPILE_ELF,"$(notdir $@"))
	@$(ECHO) $(CC) $(OBJECT_FILE) $(LDFLAGS) $(LOCAL_LDFLAGS) -Wl,-Map=$(BIN_PATH)/$(TARGET).map,--cref -o $@
	@$(CC) $(OBJECT_FILE) $(LDFLAGS) $(LOCAL_LDFLAGS) -Wl,-Map=$(BIN_PATH)/$(TARGET).map,--cref -o $@

$(BIN_PATH)/$(TARGET).hex: $(BIN_PATH)/$(TARGET).elf Makefile | $(BIN_PATH)
	@$(call PRINT_COMPILE_HEX,"$(notdir $@"))
	@$(ECHO) $(OBJCOPY) -Oihex  $< $@
	@$(OBJCOPY) -Oihex  $< $@

$(BIN_PATH)/$(TARGET).bin: $(BIN_PATH)/$(TARGET).elf Makefile | $(BIN_PATH)
	@$(call PRINT_COMPILE_BIN,"$(notdir $@"))
	@$(ECHO) $(OBJCOPY) -Obinary -S $< $@
	@$(OBJCOPY) -Obinary -S $< $@
