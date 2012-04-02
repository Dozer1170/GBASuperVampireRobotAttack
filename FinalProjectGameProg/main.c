#include "gba.h"
#include <math.h>
#include "sprite/robotsprite.h"
#include "sprite/sprite.c"
#include "init.c"

#define MAP_PIXEL_X_MAX 2160
#define MAP_PIXEL_Y_MAX 740
#define LEVEL_WIDTH 300
#define LEVEL_HEIGHT 100
#define LANDSCAPE_WIDTH 30
#define FACTORY_MAP_SIZE 30000
#define FACTORY_PALETTE_SIZE 256
#define FACTORY_TILE_SET_SIZE 7232

#define GROUND 0
#define AIR 1

#define MAIN_HANDLER spriteHandlers[0]
#define MAIN_SPRITE sprites[0]

extern const u16 factoryhitmap_Map[];
extern const u16 factorylevel_Map[];
extern const u16 factorylevel_Palette[];
extern const u8 factorylevel_Tiles[];

//extern const u16 levellandscape_Map[];

const u16 *levelhitmap_Map;
const u16 *level_Map;
const u16 *level_Palette;
const u8 *level_Tiles;

u16* levelMap =(u16*)ScreenBaseBlock(30);
u16* levelHitMap = (u16*)ScreenBaseBlock(20);
u16* levelLandscapeMap = (u16*)ScreenBaseBlock(15);

volatile u32 *BUTTONS = (volatile u32*)0x04000130;

typedef struct tagSprite
{
	u16 attribute0;
	u16 attribute1;
	u16 attribute2;
	s16 attribute3;
} ALIGN4 Sprite,*pSprite;

typedef struct OBJ_AFFINE
{
    u16 fill0[3];
    s16 pa;
    u16 fill1[3];
    s16 pb;
    u16 fill2[3];
    s16 pc;
    u16 fill3[3];
    s16 pd;
} ALIGN4 OBJ_AFFINE;

typedef struct tagAnimationHandler {
	int numFrames, currFrame;
	int frameLocation[2];
} AnimationHandler;

typedef struct tagAngleInfo {
	float cosAngle, sinAngle;
	float slopeFactor;
} AngleInfo;


typedef struct HitBox
{
	// From the left
	int xOffset;
	
	// From the top
	int yOffset;
	
	// From the right
	int negXOffset;
	
	// From the bottom
	int negYOffset;
	
	int width;
	int height;
} HitBox


typedef struct tagSpriteHandler
{
	int x, y;
	int mode;
	int worldx, worldy;
	float acc, dec, xspd, yspd;
	float gspd, maxGspd;
	int width, height;
	int alive;
	int dir;
	int flipped;
	AngleInfo angle;
	AnimationHandler standing;
	AnimationHandler running;
	AnimationHandler jumpUp;
	AnimationHandler jumpDown;
	HitBox hitBox;
} SpriteHandler;

int NextFrameLocation(AnimationHandler *handler) {
	if(handler->currFrame >= handler->numFrames - 1)
	    handler->currFrame = -1;
	return handler->frameLocation[++handler->currFrame];
}

Sprite sprites[128];
SpriteHandler spriteHandlers[128];

typedef struct BgInfo {
    int x, y ;
	int dx, dy;
	int backgroundNextCol, backgroundPrevCol;
	int backgroundNextRow, backgroundPrevRow;
	int xNextCol;
	int xPrevCol;
	int yNextRow;
	int yPrevRow;
} BgInfo;

BgInfo bg, level;

u16 __key_curr=0, __key_prev=0;

inline u32 key_curr_state()         {   return __key_curr;          }
inline u32 key_prev_state()         {   return __key_prev;          }
inline u32 key_is_down(u32 key)     {   return  __key_curr & key;   }
inline u32 key_is_up(u32 key)       {   return ~__key_curr & key;   }
inline u32 key_was_down(u32 key)    {   return  __key_prev & key;   }
inline u32 key_was_up(u32 key)      {   return ~__key_prev & key;   }
inline u32 key_released(u32 key)
{   return (~__key_curr &  __key_prev) & key;  }
inline u32 key_held(u32 key)
{   return ( __key_curr &  __key_prev) & key;  }
inline u32 key_hit(u32 key)
{   return ( __key_curr &~ __key_prev) & key;  }



//ScreenXCol: the column in our 256x256 background to copy to
//bgColumn: the column from the background map we would like to copy from
void copyColumn(int screenXCol, int screenYOff, int bgYOff, int bgColumn,
	u16* toMap, const u16* fromMap, int mapWidth)
{
	screenXCol = screenXCol % 32;
	int i;
	for(i = 0; i < 32; i++)
	{
		toMap[screenXCol + ((i+screenYOff)%32) * 32] =
			fromMap[bgColumn+(mapWidth*(i+bgYOff))];
	}
}
//ScreenYRow: the row in our 256x256 background to copy to
//bgRow: the row from the background map we would like to copy from
void copyRow(int screenXOff, int screenYRow, int bgXOff, int bgRow,
	u16* toMap, const u16* fromMap, int mapWidth)
{
	int i;
	for(i = 0; i < 32; i++)
	{
		toMap[((i+screenXOff)%32) + screenYRow * 32] =
			fromMap[(bgXOff + i) + (mapWidth*bgRow)];
	}
}


// Update OAM
void UpdateSpriteMemory(void)
{
	DMAFastCopy((void*)sprites, (void*)SpriteMem, 512, DMA_16NOW);
}




void Initialize() {
	SetMode(0x0 | BG0_ENABLE | BG2_ENABLE | OBJ_ENABLE | OBJ_MAP_1D);
	InitMaps();
	InitSprites();
}


void moveViewport() {
	int nx = MAIN_HANDLER.worldx - 120 + MAIN_HANDLER.width/2;
  	if(MAIN_HANDLER.worldx < MAP_PIXEL_X_MAX && MAIN_HANDLER.worldx > 119 + MAIN_HANDLER.width/2)
	{
     	level.dx += nx - level.x;
	    level.x = nx;
	}
	int ny = MAIN_HANDLER.worldy - 80 + MAIN_HANDLER.height/2;
	if(MAIN_HANDLER.worldy < MAP_PIXEL_Y_MAX && MAIN_HANDLER.worldy > 79 + MAIN_HANDLER.height/2)
	{
     	level.dy += ny - level.y;
	    level.y = ny;
	}
}


//Checks a single pixel if there is a solid object
bool checkSolidCollision(int x, int y) {
	bool rval = 0;
	u16 tileStart = levelhitmap_Map[(x/8) + (LEVEL_WIDTH * (y/8))] * 64;
	u8 pixel = level_Tiles[tileStart + (x%8) + (8*(y%8))];
	if(9 == pixel || 10 == pixel || 4 == pixel) {
		rval = 1;
	}
	return rval;
}

//Checks a single pixel if there is a solid object and sets sprites
//angle values based upon what it it
bool checkSolidCollisionSet(SpriteHandler *sprite, int x, int y) {
	bool rval = 0;
	u16 tileStart = levelhitmap_Map[(x/8) + (LEVEL_WIDTH * (y/8))] * 64;
	u8 pixel = level_Tiles[tileStart + (x%8) + (8*(y%8))];
	if(2 == pixel) {
		rval = 1;
		sprite->angle.sinAngle = 0;
		sprite->angle.cosAngle = 1;
		sprite->angle.slopeFactor = 0;
	}
	if(4 == pixel) {
		rval = 1;
		sprite->angle.sinAngle = .7;
		sprite->angle.cosAngle = .7;
		sprite->angle.slopeFactor = .15;
	}
	if(5 == pixel) {
		rval = 1;
		sprite->angle.sinAngle = .7;
		sprite->angle.cosAngle = .7;
		sprite->angle.slopeFactor = -.15;
	}
	return rval;
}

//returns the 'topmost' y value where there is a collision
//on the left and right side of the sprite
int checkABSensors(SpriteHandler *sprite, int nextX, int nextY) {
	int i;
	int leftX = nextX + 2;
	int rightX = nextX + sprite->width - 4;
	int y = nextY + sprite->height/2;
	int max = sprite->height/2 + 8;
	for(i = 0; i < max; i++) {
		if(checkSolidCollisionSet(sprite,leftX,y+i)) {
			return y+i-sprite->height;
		}
  		if(checkSolidCollisionSet(sprite,rightX,y+i)) {
			return y+i-sprite->height;
		}
	}
	return nextY;
}



void Update() {
    ButtonPoll();
    if(MAIN_HANDLER.mode == GROUND)
    {
		if(key_is_down(KEY_LEFT))
		{
			if(!checkSolidCollision(MAIN_HANDLER.worldx+1,
				MAIN_HANDLER.worldy + 4)) {
				if(MAIN_HANDLER.gspd + MAIN_HANDLER.acc < MAIN_HANDLER.maxGspd)
					MAIN_HANDLER.gspd  += MAIN_HANDLER.acc;
			}
			if(!MAIN_HANDLER.flipped) {
					MAIN_HANDLER.flipped = 1;
					MAIN_SPRITE.attribute1 =
						FLIP_LEFT(MAIN_SPRITE.attribute1);
			}
			MAIN_HANDLER.dir = -1;
		}
		if(key_is_down(KEY_RIGHT))
		{
	     	if(!checkSolidCollision(MAIN_HANDLER.worldx +
			 	MAIN_HANDLER.width - 4, MAIN_HANDLER.worldy + 4)) {
				if(MAIN_HANDLER.gspd + MAIN_HANDLER.acc < MAIN_HANDLER.maxGspd)
					MAIN_HANDLER.gspd  += MAIN_HANDLER.acc;
			}
			if(MAIN_HANDLER.flipped) {
					MAIN_HANDLER.flipped = 0;
					MAIN_SPRITE.attribute1 =
						FLIP_RIGHT(MAIN_SPRITE.attribute1);
			}
			MAIN_HANDLER.dir = 1;
		}
		if(key_hit(KEY_A))
		{
			MAIN_HANDLER.mode = AIR;
			MAIN_HANDLER.yspd = -5;
		}
		MAIN_HANDLER.gspd = MAIN_HANDLER.gspd +
			((MAIN_HANDLER.angle.slopeFactor * MAIN_HANDLER.dir)
			* MAIN_HANDLER.angle.sinAngle) - .05;
			
		if(MAIN_HANDLER.gspd < 0)
		    MAIN_HANDLER.gspd = 0;
			
        MAIN_HANDLER.xspd = (MAIN_HANDLER.gspd * MAIN_HANDLER.angle.cosAngle) *
			MAIN_HANDLER.dir;
		if(MAIN_HANDLER.mode != AIR)
			MAIN_HANDLER.yspd = (MAIN_HANDLER.gspd*MAIN_HANDLER.angle.sinAngle);
        
        if(MAIN_HANDLER.xspd == 0) {
            MAIN_SPRITE.attribute2=NextFrameLocation(&(MAIN_HANDLER.standing));
    	} else {
            MAIN_SPRITE.attribute2=NextFrameLocation(&(MAIN_HANDLER.running));
		}
	}
	
	if(MAIN_HANDLER.mode == AIR)
	{
		if(MAIN_HANDLER.yspd < 10)
			MAIN_HANDLER.yspd = MAIN_HANDLER.yspd + .25;
			
		if(MAIN_HANDLER.yspd < 0) {
            MAIN_SPRITE.attribute2=NextFrameLocation(&(MAIN_HANDLER.jumpUp));
		} else {
            MAIN_SPRITE.attribute2=NextFrameLocation(&(MAIN_HANDLER.jumpDown));
		}
	}
	
	move(&MAIN_HANDLER,MAIN_HANDLER.worldx + MAIN_HANDLER.xspd,
		MAIN_HANDLER.worldy + MAIN_HANDLER.yspd);
	if(MAIN_HANDLER.x < 240 && MAIN_HANDLER.x >= 0)
		MAIN_SPRITE.attribute1 = SET_X(MAIN_SPRITE.attribute1, MAIN_HANDLER.x);
	if(MAIN_HANDLER.y >= 0 && MAIN_HANDLER.y < 160)
		MAIN_SPRITE.attribute0 = SET_Y(MAIN_SPRITE.attribute0, MAIN_HANDLER.y);
}

void DrawLevelBackground() {
    if(level.dx > 7)
	{
		int inc = level.dx / 8;
		level.dx = level.dx % 8;
		int i;
		for(i = 0; i < inc; i++) {
			copyColumn(level.xNextCol, level.yPrevRow + 1,
				level.backgroundPrevRow + 1, level.backgroundNextCol,
				levelMap, level_Map, LEVEL_WIDTH);
			copyColumn(level.xNextCol, level.yPrevRow + 1,
				level.backgroundPrevRow + 1, level.backgroundNextCol,
				levelHitMap, levelhitmap_Map, LEVEL_WIDTH);
			level.backgroundPrevCol++;
			level.backgroundNextCol++;
			level.xNextCol = (level.xNextCol + 1) % 32;
			level.xPrevCol = (level.xPrevCol + 1) % 32;
		}
	}
	if(level.dx < -7)
	{
     	int inc = abs(level.dx / 8);
		level.dx = level.dx % 8;
		int i;
		for(i = 0; i < inc; i++) {
			copyColumn(level.xPrevCol, level.yPrevRow + 1,
				level.backgroundPrevRow + 1, level.backgroundPrevCol,
				levelMap, level_Map, LEVEL_WIDTH);
			copyColumn(level.xPrevCol, level.yPrevRow + 1,
				level.backgroundPrevRow + 1, level.backgroundPrevCol,
				levelHitMap, levelhitmap_Map, LEVEL_WIDTH);
			level.backgroundNextCol--;
			level.backgroundPrevCol--;
			if(level.xPrevCol <= 0)
				level.xPrevCol = 31;
			else
			    level.xPrevCol--;

			if(level.xNextCol <= 0)
			    level.xNextCol = 31;
			else
			    level.xNextCol--;
		}
	}
	if(level.dy > 7)
	{
		int i;
		int inc = level.dy / 8;
		level.dy = level.dy % 8;
	    for(i = 0; i < inc; i++) {
			copyRow(level.xPrevCol + 1,level.yNextRow,
				level.backgroundPrevCol + 1, level.backgroundNextRow,
				levelMap, level_Map, LEVEL_WIDTH);
	  		copyRow(level.xPrevCol + 1,level.yNextRow,
				level.backgroundPrevCol + 1, level.backgroundNextRow,
				levelHitMap, levelhitmap_Map, LEVEL_WIDTH);
			level.backgroundPrevRow++;
			level.backgroundNextRow++;
			level.yNextRow = (level.yNextRow + 1) % 32;
			level.yPrevRow = (level.yPrevRow + 1) % 32;
		}
	}
	if(level.dy < -7)
	{
		int i;
  		int inc = abs(level.dy / 8);
		level.dy = level.dy % 8;
		for(i = 0; i < inc; i++) {
			copyRow(level.xPrevCol + 1,level.yPrevRow,
				level.backgroundPrevCol + 1, level.backgroundPrevRow,
				levelMap, level_Map, LEVEL_WIDTH);
			copyRow(level.xPrevCol + 1,level.yPrevRow,
				level.backgroundPrevCol + 1, level.backgroundPrevRow,
				levelHitMap, levelhitmap_Map, LEVEL_WIDTH);
			level.backgroundNextRow--;
			level.backgroundPrevRow--;
			if(level.yPrevRow <= 0)
				level.yPrevRow = 31;
			else
			    level.yPrevRow--;

			if(level.yNextRow <= 0)
			    level.yNextRow = 31;
			else
			    level.yNextRow--;
		}
	}
}

void Draw() {
    WaitVBlank();
	UpdateSpriteMemory();
	DrawLevelBackground();
	REG_BG0VOFS = level.y;
	REG_BG0HOFS = level.x;
	REG_BG2VOFS = level.y;
	REG_BG2HOFS = level.x;
}

int main()
{
	Initialize();
	LoadContent();
	while(1)
	{
		Update();
		Draw();
 	}
	return 0;
}
