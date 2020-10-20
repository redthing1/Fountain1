.PHONY: clean

MAPS_DIR := ../map

MAPS_SRC := $(shell find $(MAPS_DIR) -name '*.tmx')
MAPS_BIN := $(notdir $(MAPS_SRC:%.tmx=%.bin))

all: $(MAPS_BIN)

clean:
	rm -rfv *.bin

$(MAPS_BIN): $(MAPS_SRC)
	Tiled2GBA -b $< $@
