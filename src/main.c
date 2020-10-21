#include <string.h>
#include "gbamap.h"
#include <tonc.h>
#include "input.h"
#include "gbfs.h"

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer = (OBJ_AFFINE *)obj_buffer;
const GBFS_FILE *gbfs_dat;

Map loadMap()
{
    u32 mapDataSize = 0;
    const u16 *mapData = gbfs_get_obj(gbfs_dat, "fountain.bin", &mapDataSize);

    return loadMapFromROM(mapData);
}

int main()
{
    gbfs_dat = find_first_gbfs_file(find_first_gbfs_file);

    initMapRegisters();
    Map map = loadMap();
    setMapOnScreen(map);

    oam_init(obj_buffer, 128);
    REG_DISPCNT |= DCNT_OBJ | DCNT_OBJ_1D;

    u32 eggcat_img_len;
    const u32 *eggcat_img = gbfs_get_obj(gbfs_dat, "egg.img.bin", &eggcat_img_len);
    u32 eggcat_pal_len;
    const u32 *eggcat_pal = gbfs_get_obj(gbfs_dat, "egg.pal.bin", &eggcat_pal_len);
    memcpy(&tile_mem[4][0], eggcat_img, eggcat_img_len);
    memcpy(pal_obj_mem, eggcat_pal, eggcat_pal_len);

    int px = SCREEN_WIDTH / 2 - 8, py = SCREEN_HEIGHT / 2 - 8;
    u32 tid = 0, pb = 0;
    OBJ_ATTR *player = &obj_buffer[0];
    obj_set_attr(player,
                 ATTR0_SQUARE,             // Square, regular sprite
                 ATTR1_SIZE_16,            // 16x16p,
                 ATTR2_PALBANK(pb) | tid); // palbank 0, tile 0

    const int SHIFT_SPEED = 1;
    BackgroundPoint backgroundShift = {128, 248};
    u32 frames = 0;
    int player_frame = 0;
    while (TRUE)
    {
        vid_vsync();
        frames++;

        KeyState inputState = getInputState();
        int ymove = getYAxis(inputState);
        int xmove = getXAxis(inputState);
        backgroundShift.y += ymove * SHIFT_SPEED;
        backgroundShift.x += xmove * SHIFT_SPEED;
        shiftMap(map, backgroundShift);

        bool moving = (ymove != 0 || xmove != 0);
        if (moving)
        {
            // when running
            if (frames % 6 == 0)
            {
                player_frame = (player_frame + 1) % 4;
            }
        }
        else
        {
            player_frame = 0;
        }

        obj_set_attr(player, ATTR0_SQUARE, ATTR1_SIZE_16, ATTR2_PALBANK(pb) | player_frame * 4);

        obj_set_pos(player, px, py);
        oam_copy(oam_mem, obj_buffer, 1); // only need to update one
    }
}