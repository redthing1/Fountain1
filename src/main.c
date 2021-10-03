#include <string.h>
#include <tonc.h>

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer = (OBJ_AFFINE *)obj_buffer;

int main()
{
    REG_DISPCNT = DCNT_MODE0;
    REG_DISPCNT |= DCNT_BG0;

    pal_bg_mem[0] = CLR_BLUE;
}