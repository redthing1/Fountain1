.PHONY: clean

all: maps

clean:
	rm -rv *.bin

maps: fountain.bin

fountain.bin: ../map/fountain.tmx
	Tiled2GBA -b $< $@
