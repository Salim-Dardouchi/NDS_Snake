#include "../HelperFunctions/HelperFunctions.h"
#include <stdio.h>

#include "CApp.h"

#define APP_BLANK_SPACES            "               " //this is the most retarded thing I had to write in a const char*

enum e_status{
    ST_ALL_CLEARED   =  0,
    ST_GAMEOVER      =  0x1,
    ST_PAUSED        =  0x2,
    ST_RESETGAME     =  0x100,
};

CApp::CApp():
    m_ptFood(t_point{SNAKE_POS_MAX_X+1, SNAKE_POS_MAX_Y+1}),
    m_ptPoison(t_point{SNAKE_POS_MAX_X+1, SNAKE_POS_MAX_Y+1}),
    m_iSnakeFoodTimeout(0),
    m_iSnakePoisonTimeout(0),
    m_nbSprites(0),
    m_uStatus(ST_ALL_CLEARED),
    m_uScore(0)
{
    _InitializeNFLib();
    _InitializeSound();
    m_pSnake = new CSnake();
    m_pSnake->SetNbSprites(m_nbSprites); 
    Run();
}

CApp::~CApp(){
    delete m_pSnake;
}

void CApp::Run(){
    int running = 1;
    int timer=0;
    
    while(running){
        scanKeys();
        _ProcessInputs();
        if(mIsBitsSet(m_uStatus, ST_RESETGAME)){
            delete m_pSnake;
            m_pSnake = new CSnake;
            
            m_ptFood    = t_point{SNAKE_POS_MAX_X+1, SNAKE_POS_MAX_Y+1};
            m_ptPoison  = t_point{SNAKE_POS_MAX_X+1, SNAKE_POS_MAX_Y+1};
            m_iSnakeFoodTimeout = 0;
            m_iSnakePoisonTimeout = 0;

            m_uScore = 0;
            mBitsClr(m_uStatus, ST_GAMEOVER);
            mBitsClr(m_uStatus, ST_RESETGAME);


        }
        timer++;
        //Snake Moving and drawing
        m_pSnake->Draw();
        if(!(timer%6) && (mIsBitsClr(m_uStatus, ST_GAMEOVER)) && (mIsBitsClr(m_uStatus, ST_PAUSED)) ){
            m_pSnake->Move();
            //Food Logic
            if(!m_iSnakeFoodTimeout){
                m_ptFood = t_point{rand()%SNAKE_POS_MAX_X, rand()%SNAKE_POS_MAX_Y};
                m_iSnakeFoodTimeout = 10+rand()%40+1;
            }
            if(m_ptFood.x>SNAKE_POS_MAX_X){
                m_iSnakeFoodTimeout--;
                NF_Show3dSprite(252, false);
            }
            else{
                NF_Show3dSprite(252, true);
            }

            //Food Detection
            if(m_pSnake->IsFoodDetected(m_ptFood.x, m_ptFood.y)){
                _CreateNewBodySprite();
                m_pSnake->GrowUp();
                m_ptFood = t_point{SNAKE_POS_MAX_X+1, SNAKE_POS_MAX_Y+1};
                m_uScore += 50;
                NF_PlayRawSound(0, 127, 64, false, 0);
            }

            //Poison logic
            if(!m_iSnakePoisonTimeout){
                m_ptPoison = t_point{rand()%SNAKE_POS_MAX_X, rand()%SNAKE_POS_MAX_Y};
                m_iSnakePoisonTimeout = 40+rand()%80+1;
            }
            if(m_ptPoison.x>SNAKE_POS_MAX_X){
                m_iSnakePoisonTimeout--;
                NF_Show3dSprite(253, false);
            }
            else{
                NF_Show3dSprite(253, true);
            }

            //Poison detection
            if(m_pSnake->IsFoodDetected(m_ptPoison.x, m_ptPoison.y)){
                _CreateNewBodySprite();
                m_pSnake->Shrink();
                m_ptPoison = t_point{SNAKE_POS_MAX_X+1, SNAKE_POS_MAX_Y+1};
                m_uScore-=50;
                NF_PlayRawSound(1, 127, 64, false, 0);
            }

            //Gameover Detection
            if(m_pSnake->IsBiteHimself())
                mBitsSet(m_uStatus, ST_GAMEOVER);
        }
       
        NF_Move3dSprite(252, APP_PADDING_HRZ+(m_ptFood.x*TILE_SIZE), APP_PADDING_VRT+(m_ptFood.y*TILE_SIZE));
        NF_Move3dSprite(253, APP_PADDING_HRZ+(m_ptPoison.x*TILE_SIZE), APP_PADDING_VRT+(m_ptPoison.y*TILE_SIZE));

        //Showing score
        char strScore[32] = "";
        sprintf(strScore, "Score - %.5d", m_uScore);

        char strPaused[16] = "";
        
        if(mIsBitsSet(m_uStatus, ST_PAUSED))
            sprintf(strPaused, "PAUSED");
        
        else if(mIsBitsSet(m_uStatus, ST_GAMEOVER)){
            sprintf(strPaused, "GAME OVER");
            NF_WriteText(1, 0, 6, 16, "SELECT - Play Again");
        }
        else{
            NF_WriteText(1, 0, 6, 16, APP_BLANK_SPACES);
            NF_WriteText(1, 0, 21, 16, APP_BLANK_SPACES);
            sprintf(strPaused, APP_BLANK_SPACES); 
        }

        NF_WriteText(1, 0, 11, 10, (const char*)strPaused);
        NF_WriteText(1, 0, 3, 3, (const char*)strScore);

        NF_UpdateTextLayers();
        NF_Draw3dSprites();
        glFlush(0);
        swiWaitForVBlank();
        NF_Update3dSpritesGfx();
    }
}
void CApp::_InitializeNFLib(){
    srand((unsigned int)time(nullptr));

    NF_SetRootFolder("NITROFS");

    NF_Set3D(0, 0); //3D on top screen
    NF_Set2D(1, 0); //2D on bottom screen

    consoleDemoInit(); //Initializing console 
    swiWaitForVBlank();

    //iprintf("\n Initializing NitroFS (cartridge read-only filesystem for assets)\n");

    NF_InitTiledBgBuffers(); //Initializing buffers for tiled backgrounds
    NF_InitTiledBgSys(0); //Initializing buffer for one of our backgrounds
    NF_InitTiledBgSys(1);

    NF_InitSpriteBuffers(); //Initializing our sprite buffers (we'll need them)
    NF_Init3dSpriteSys(); //Initializing the 3D sprite system.

    NF_LoadTiledBg(BACKGROUND_ONE_PATH, "bg", 256, 256); //Loading our main background
    NF_LoadTiledBg(BACKGROUND_TWO_PATH, "bg2", 256, 256);
    
    const char *strFilePathTable[SPRITE_NUMBER] = {
        "sprite/snakeHead",
        "sprite/snakeTail",
        "sprite/food",
        "sprite/poison",
        "sprite/snakeBody",
    };

    for(int k=0; k<SPRITE_NUMBER; k++)
        LoadSpriteAndPal(strFilePathTable[k], k, TILE_SIZE, TILE_SIZE);
    
    for(int k=0; k<SPRITE_NUMBER; k++){
        NF_Vram3dSpriteGfx(k, k, true);
        NF_Vram3dSpritePal(k, k);
    }

    NF_CreateTiledBg(0, 3, "bg"); // Creating a background with the one we loaded
    NF_CreateTiledBg(1, 3, "bg2");
    
    /*We loaded all of our sprites */

    for(int k=250; k<SPRITE_NUMBER+250; k++){
        NF_Create3dSprite(k, k-250, k-250, 256+TILE_SIZE, 0);
    }

    for(int k=0; k<2; k++)    
        _CreateNewBodySprite();
    
    NF_LoadTextFont("fonts/default", "normal", 256, 256, 0);
    NF_InitTextSys(1);
    NF_CreateTextLayer(1, 0, 0, "normal");

    /* We have created our sprites */

}

void CApp::_ProcessInputs(){
    int keys = keysDown();

    if(keys & KEY_UP){
        m_pSnake->ChangeDirection(DIR_UP);
    }
    
    if(keys & KEY_DOWN){
        m_pSnake->ChangeDirection(DIR_DOWN);
    }
    
    if(keys & KEY_LEFT){
        m_pSnake->ChangeDirection(DIR_LEFT);
    }
    
    if(keys & KEY_RIGHT){
        m_pSnake->ChangeDirection(DIR_RIGHT);
    }

    if(keys & KEY_START){
        if(mIsBitsClr(m_uStatus, ST_GAMEOVER)){
            mBitsTgl(m_uStatus, ST_PAUSED);
            NF_PlayRawSound(2, 127, 64, false, 0);
        }
    }

    if(keys & KEY_SELECT){
        if(mIsBitsSet(m_uStatus, ST_GAMEOVER))
            mBitsSet(m_uStatus, ST_RESETGAME);
    }
}

void CApp::_InitializeSound(){

    soundEnable();
    NF_InitRawSoundBuffers();

    const char* strFilePathTable[4] = {
        "sound/collect",
        "sound/poison",
        "sound/pause",
    };

    NF_LoadRawSound(strFilePathTable[0], 0, 44100, 0);
    NF_LoadRawSound(strFilePathTable[1], 1, 22050, 0);
    NF_LoadRawSound(strFilePathTable[2], 2, 44100, 0);
}

void CApp::_CreateNewBodySprite(){
    NF_Create3dSprite(m_nbSprites, 4, 4, 256, 192);
    m_nbSprites++;
    m_pSnake->SetNbSprites(m_nbSprites);
    NF_Sort3dSprites();
}