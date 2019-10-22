#
# This file is part of SPICE.
# Copyright (C) 2019  Albert Farres
#
# SPICE is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# SPICE is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with SPICE.  If not, see <http://www.gnu.org/licenses/>.
#
AUTOMAKE_OPTIONS = foreign

PROGRAM        := spice
CSOURCES       := spi.c
CXXSOURCES     := utils.cpp avr_bitset.cpp Timer.cpp View.cpp Controller.cpp Model.cpp SwapContainer.cpp driver.cpp
if WITH_USART_PRINTF
CSOURCES       += USART.c
endif
LIBS           := ustl/libustl.a

ustl/libustl.a:
	$(MAKE) V=1 -C ustl
ustl_clean:
	$(MAKE) V=1 -C ustl clean
ustl_maintainer_clean:
	$(MAKE) V=1 -C ustl maintainer-clean

OBJECTS        := $(patsubst %.cpp,%.o,$(CXXSOURCES))
OBJECTS        += $(patsubst %.c,%.o,$(CSOURCES))

#all: $(PROGRAM).elf lst text eeprom
all-am: hex ehex

if WITH_AVRDUDE
#AVRDUDE_OPTS  = -U lfuse:w:0xe2:m # program CKDIV8
#AVRDUDE_OPTS  = -U lfuse:w:0x62:m # unprogram CKDIV8
upload: hex ehex
	$(AVRDUDE) $(AVRDUDE_PROGRAMMER) -p $(AVRDUDE_MCU) $(AVRDUDE_OPTS) -U flash:w:$(PROGRAM).hex -U eeprom:w:$(PROGRAM)_eeprom.hex
endif

DEPDIR   = .deps
$(eval include $(srcdir)/deps.mk)

$(DEPDIR)/%.d: %.cpp %.hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -mmcu=$(MCU_TARGET) -MM -MF $@ $<
$(DEPDIR)/%.d: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -mmcu=$(MCU_TARGET) -MM -MF $@ $<
$(DEPDIR)/%.d: %.c %.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -mmcu=$(MCU_TARGET) -MM -MF $@ $<
$(DEPDIR)/%.d: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -mmcu=$(MCU_TARGET) -MM -MF $@ $<


-include $(DEPS)
%.o: %.cpp $(DEPDIR)/%.d
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -mmcu=$(MCU_TARGET) -c -o $@ $<
%.o: %.c $(DEPDIR)/%.d
	$(CC) $(CPPFLAGS) $(CFLAGS) -mmcu=$(MCU_TARGET) -c -o $@ $<

$(PROGRAM).elf: $(OBJECTS) $(LIBS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -mmcu=$(MCU_TARGET) -Wl,--gc-sections,-Map,$(PROGRAM).map -o $@ $^

clean: ustl_clean
	@rm -vrf .deps
	@rm -vrf *.o $(PROGRAM).elf *.eps *.png *.pdf *.bak
	@rm -vrf *.lst *.map $(EXTRA_CLEAN_FILES)

maintainer-clean: ustl_maintainer_clean

lst:  $(PROGRAM).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

# Rules for getting size info
sizeinfo: $(PROGRAM).elf
	nm --print-size --size-sort --radix=d -C $^
	$(OBJSIZE) --format=avr $^

# Rules for building the .text rom images

text: hex bin srec

hex:  $(PROGRAM).hex
bin:  $(PROGRAM).bin
srec: $(PROGRAM).srec

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

# Rules for building the .eeprom rom images

eeprom: ehex ebin esrec

ehex:  $(PROGRAM)_eeprom.hex
ebin:  $(PROGRAM)_eeprom.bin
esrec: $(PROGRAM)_eeprom.srec

%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@ \
	  || { echo empty $@ not generated; exit 0; }

%_eeprom.srec: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@ \
	  || { echo empty $@ not generated; exit 0; }

%_eeprom.bin: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@ \
	  || { echo empty $@ not generated; exit 0; }

EXTRA_CLEAN_FILES       = *.bin *.srec

