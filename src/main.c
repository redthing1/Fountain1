#include <string.h>
#include "gbamap.h"
#include <tonc.h>
#include "input.h"
#include "gbfs.h"

Map loadMap()
{
    u32 mapDataSize = 0;
    const GBFS_FILE *mapFile = find_first_gbfs_file(find_first_gbfs_file);
    const u16 *mapData = gbfs_get_obj(mapFile, "fountain.bin", &mapDataSize);

    return loadMapFromROM(mapData);
}

int main()
{
    initMapRegisters();
    Map map = loadMap();
    setMapOnScreen(map);

    // loadSpriteSheet();
    // OBJ_ATTR spriteObjects[128];
    // initializeSpriteObjectMemory(spriteObjects, 128);
    // showMapObjects(&map, spriteObjects);
    // setSpritesOnScreen();

    const int SHIFT_SPEED = 2;
    BackgroundPoint backgroundShift = {0, 0};
    while (TRUE)
    {
        vid_vsync();

        KeyState inputState = getInputState();
        backgroundShift.y += getYAxis(inputState) * SHIFT_SPEED;
        backgroundShift.x += getXAxis(inputState) * SHIFT_SPEED;
        shiftMap(map, backgroundShift);

        ObjectPoint objectShift = {0, 0};
        objectShift.x = getXAxis(inputState) * SHIFT_SPEED;
        objectShift.y = getYAxis(inputState) * SHIFT_SPEED;
        shiftMapObjects(map.objects, objectShift, map.numObjects);
        // showMapObjects(&map, spriteObjects);
    }
}