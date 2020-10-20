#include "./background.h"
#include "gbamap.h"

void shiftMapLayer(u16 layer, BackgroundPoint offset)
{
    REGISTER_BACKGROUND_OFFSET[layer] = offset;
}
