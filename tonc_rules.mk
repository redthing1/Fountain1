#
# Yes, this is almost, but not quite, completely like to 
# DKP's base_rules and gba_rules
#

export PATH	:=	$(DEVKITARM)/bin:$(PATH)

# LIBGBA		?=	$(DEVKITPRO)/libgba

# --- Executable names ---

PREFIX		?=	arm-none-eabi-

export CC	:=	$(PREFIX)gcc
export CXX	:=	$(PREFIX)g++
export AS	:=	$(PREFIX)as
export AR	:=	$(PREFIX)ar
export NM	:=	$(PREFIX)nm
export OBJCOPY	:=	$(PREFIX)objcopy

# LD defined in Makefile


# === LINK / TRANSLATE ================================================

%.gba : %.elf
	@$(OBJCOPY) -O binary $< $@
	@echo built ... $(notdir $@)
	@gbafix $@ -t$(TITLE)

#----------------------------------------------------------------------

%.mb.elf :
	@echo Linking multiboot
	$(LD) -specs=gba_mb.specs $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@
	$(NM) -Sn $@ > $(basename $(notdir $@)).map

#----------------------------------------------------------------------

%.elf :
	@echo Linking cartridge
	$(LD) -specs=gba.specs $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@	
	$(NM) -Sn $@ > $(basename $(notdir $@)).map

#----------------------------------------------------------------------

%.a :
	@echo $(notdir $@)
	@rm -f $@
	$(AR) -crs $@ $^

# === OBJECTIFY =======================================================

%.iwram.o : %.iwram.cpp
	@echo $(notdir $<)
	$(CXX) $(CXXFLAGS) $(IARCH) -c $< -o $@
	
#----------------------------------------------------------------------
%.iwram.o : %.iwram.c
	@echo $(notdir $<)
	$(CC) $(CFLAGS) $(IARCH) -c $< -o $@

#----------------------------------------------------------------------

%.o : %.cpp
	@echo $(notdir $<)
	$(CXX) $(CXXFLAGS) $(RARCH) -c $< -o $@

#----------------------------------------------------------------------

%.o : %.c
	@echo $(notdir $<)
	$(CC) $(CFLAGS) $(RARCH) -c $< -o $@

#----------------------------------------------------------------------

%.o : %.s
	@echo $(notdir $<)
	$(CC) -x assembler-with-cpp $(ASFLAGS) -c $< -o $@

#----------------------------------------------------------------------

%.o : %.S
	@echo $(notdir $<)
	$(CC) -x assembler-with-cpp $(ASFLAGS) -c $< -o $@


#----------------------------------------------------------------------
# canned command sequence for binary data
#----------------------------------------------------------------------

define bin2o
	bin2s $< | $(AS) -o $(@)
	echo "extern const u8" `(echo $(<F) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`"_end[];" > `(echo $(<F) | tr . _)`.h
	echo "extern const u8" `(echo $(<F) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`"[];" >> `(echo $(<F) | tr . _)`.h
	echo "extern const u32" `(echo $(<F) | sed -e 's/^\([0-9]\)/_\1/' | tr . _)`_size";" >> `(echo $(<F) | tr . _)`.h
endef


