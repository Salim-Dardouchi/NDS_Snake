#include "HelperFunctions.h"


void LoadSpriteAndPal(const char* strPath, int iVramPos, int iWidth, int iHeight){
    NF_LoadSpriteGfx(strPath, iVramPos, iWidth, iHeight);
    NF_LoadSpritePal(strPath, iVramPos);
}
