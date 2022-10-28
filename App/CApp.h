#include <nds.h>
#include <nf_lib.h>
#include <time.h>
#include <maxmod9.h>

#include "../Headers/parameters.h"
#include "../Headers/macros.h"
#include "../Snake/CSnake.h"

#define BACKGROUND_ONE_PATH     "bg/bg"
#define BACKGROUND_TWO_PATH     "bg/bg2"
#define SPRITE_NUMBER           (5)

typedef struct s_point{
    int x;
    int y;
}t_point;
class CApp{
private:
    CSnake* m_pSnake;
    t_point m_ptFood;
    t_point m_ptPoison;

    int     m_iSnakeFoodTimeout;
    int     m_iSnakePoisonTimeout;
    unsigned int m_nbSprites;
    unsigned int m_uStatus;
    unsigned int m_uScore;
public:
    CApp();
    ~CApp();
    void Run();
private:
    void _InitializeNFLib();
    void _ProcessInputs();
    void _CreateNewBodySprite();
    void _InitializeSound();
};