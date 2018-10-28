#include "garbage.h"
#include "alien.h"
#include "blackAlien.h"
#include "ship.h"
#include "bullet.h"
#include "blackbullet.h"
#include "alienbullet.h"
#include "title.h"
#include "gameOver.h"

#define SHOW 0
#define HID 1

typedef struct
{
	int row;
	int col;
	int deltar;
	int deltac;
  int width;
	int height;
	int isHid;
	int health;
	const unsigned short *img;
} SPRITE;
