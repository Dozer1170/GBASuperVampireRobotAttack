#include "gba.h"
#include <math.h>
#include "maps.h"
#include "sprite.h"

#include "sprite/robotsprite.h"
#include "sprite/missile.h"
#include "sprite/vampire.h"
#include "sprite/bars.h"
#include "sprite/powerups.h"


void UpdateSpriteMemory(void)
{
	DMAFastCopy((void*)sprites, (void*)SpriteMem, 512, DMA_16NOW);
}

void Initialize() {
	SetMode(0x0 | BG0_ENABLE | BG1_ENABLE | OBJ_ENABLE | OBJ_MAP_1D);
	InitMaps();
	InitSprites();
}

// Loads the tiles and palettes
void LoadContent() {
	DMAFastCopy((void*)level_Palette,(void*)BGPaletteMem,
		256,DMA_16NOW);
	DMAFastCopy((void*)level_Tiles,(void*)CharBaseBlock(0),
		level.mapSize/4,DMA_32NOW);
	DMAFastCopy((void*)missilePalette,(void*)SpritePal,256,DMA_16NOW);

	
	int n, x, nextSprite = 0;
	
	// Load robot sprite data into memory
	spriteHandlers[0].startChunk = 0;
	x = 0;
	for(n = nextSprite; n < nextSprite + SPRITE_DATA32_SQUARE * 8; n++)
	{
		SpriteData[n] = robotspriteData[x];
		x++;
	}	
	nextSprite = n;
	
	
	// Load health bar sprite data into memory
	x = 0;
	spriteHandlers[1].startChunk = SPRITE_CHUNKS32_SQUARE * 8;
	for (n = nextSprite; n < nextSprite + SPRITE_DATA64_TALL * 10; n++)
	{
		SpriteData[n] = barsData[x];
		x++;
	}
	nextSprite = n;
	
	
	// Load vampire sprite data into memory
	x = 0;
	spriteHandlers[2].startChunk = SPRITE_CHUNKS32_SQUARE * 8 + SPRITE_CHUNKS64_TALL * 10;
	for (n = nextSprite; n < nextSprite + SPRITE_DATA32_SQUARE * 3; n++)
	{
		SpriteData[n] = vampireData[x];
		x++;
	}
	nextSprite = n;
	
	
}

void moveViewport() {
	int nx = MAIN_HANDLER.worldx - 120 + MAIN_HANDLER.width/2;
  	if(MAIN_HANDLER.worldx < level.pixelXMax - 136
      && MAIN_HANDLER.worldx > 119 + MAIN_HANDLER.width/2)
	{
     	level.dx += nx - level.x;
     	bg.dx += (nx/3 - bg.x);
	   level.x = nx;
	   bg.x += (nx/3 - bg.x);
	}
	int ny = MAIN_HANDLER.worldy - 80 + MAIN_HANDLER.height/2;
	if(MAIN_HANDLER.worldy < level.pixelYMax - 96
      && MAIN_HANDLER.worldy > 79 + MAIN_HANDLER.height/2)
	{
     	level.dy += ny - level.y;
     	bg.dy += (ny/3 - bg.y);
	   level.y = ny;
	   bg.y += (ny/3 - bg.y);
	}
}

void moveXDir()
{
    if(key_is_down(KEY_LEFT))
    {
        if( MAIN_HANDLER.dir == -1)
        {
            if(MAIN_HANDLER.gspd + MAIN_HANDLER.acc < MAIN_HANDLER.maxGspd)
            {
                MAIN_HANDLER.gspd  += MAIN_HANDLER.acc;
			}
        }
        else
        {
            if(MAIN_HANDLER.gspd - (MAIN_HANDLER.acc/2) > 0)
                MAIN_HANDLER.gspd  -= (MAIN_HANDLER.acc/2);
			else
					MAIN_HANDLER.dir = -1;
        }
        if(!MAIN_HANDLER.flipped)
        {
            MAIN_HANDLER.flipped = 1;
            MAIN_SPRITE.attribute1 = FLIP_LEFT(MAIN_SPRITE.attribute1);
        }
    }
    if(key_is_down(KEY_RIGHT))
    {
        if( MAIN_HANDLER.dir == 1)
        {
            if(MAIN_HANDLER.gspd + MAIN_HANDLER.acc < MAIN_HANDLER.maxGspd)
            {
                MAIN_HANDLER.gspd  += MAIN_HANDLER.acc;
			}
        }
        else
        {
            if(MAIN_HANDLER.gspd - (MAIN_HANDLER.acc/2) > 0)
                MAIN_HANDLER.gspd  -= (MAIN_HANDLER.acc/2);
			else
                MAIN_HANDLER.dir = 1;
        }
        if(MAIN_HANDLER.flipped)
        {
            MAIN_HANDLER.flipped = 0;
            MAIN_SPRITE.attribute1 = FLIP_RIGHT(MAIN_SPRITE.attribute1);
        }
    }
    MAIN_HANDLER.xspd = (MAIN_HANDLER.gspd * MAIN_HANDLER.angle.cosAngle) *
			MAIN_HANDLER.dir;
}

void Update() {
    ButtonPoll();
    moveXDir();
    if(MAIN_HANDLER.mode == GROUND)
    {
      if(key_hit(KEY_A))
		{
			MAIN_HANDLER.mode = AIR;
			MAIN_HANDLER.yspd -= MAIN_HANDLER.jumpStr;
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
	    if( key_is_down( KEY_R ) && MAIN_HANDLER.fuel > 0 )
	    {
	            MAIN_HANDLER.yspd = -1;
	            MAIN_HANDLER.fuel -= 1;
        }
        else
        {
            if(MAIN_HANDLER.yspd < 10)
    			MAIN_HANDLER.yspd = MAIN_HANDLER.yspd + .25;
    			
    		if(MAIN_HANDLER.yspd < 0) {
                MAIN_SPRITE.attribute2=NextFrameLocation(&(MAIN_HANDLER.jumpUp));
    		} else {
                MAIN_SPRITE.attribute2=NextFrameLocation(&(MAIN_HANDLER.jumpDown));
    		}
		}
    		
		if(key_is_down(KEY_LEFT))
   		{
            if( MAIN_HANDLER.dir == -1)
   			{
   				if(MAIN_HANDLER.gspd + MAIN_HANDLER.acc/4 < MAIN_HANDLER.maxGspd)
   					MAIN_HANDLER.gspd  += MAIN_HANDLER.acc/4;
   			}
   			else
   			{
   				if(MAIN_HANDLER.gspd - (MAIN_HANDLER.acc/2) > 0)
   					MAIN_HANDLER.gspd  -= (MAIN_HANDLER.acc/2);
   				else
   					MAIN_HANDLER.dir = -1;
   			}
   			if(!MAIN_HANDLER.flipped) {
   					MAIN_HANDLER.flipped = 1;
   					MAIN_SPRITE.attribute1 =
   						FLIP_LEFT(MAIN_SPRITE.attribute1);
   			}
        }

		if(key_is_down(KEY_RIGHT))
		{
			if( MAIN_HANDLER.dir == 1)
			{
				if(MAIN_HANDLER.gspd + MAIN_HANDLER.acc/4 < MAIN_HANDLER.maxGspd)
					MAIN_HANDLER.gspd  += MAIN_HANDLER.acc/4;
			}
			else
			{
				if(MAIN_HANDLER.gspd - (MAIN_HANDLER.acc/2) > 0)
					MAIN_HANDLER.gspd  -= (MAIN_HANDLER.acc/2);
				else
					MAIN_HANDLER.dir = 1;
			}
			if(MAIN_HANDLER.flipped) {
					MAIN_HANDLER.flipped = 0;
					MAIN_SPRITE.attribute1 =
						FLIP_RIGHT(MAIN_SPRITE.attribute1);
			}
		}
		MAIN_HANDLER.xspd = (MAIN_HANDLER.gspd) * MAIN_HANDLER.dir;
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
		moveBackgroundRight(&level);
	}
	if(level.dx < -7)
	{
     	moveBackgroundLeft(&level);
	}
	if(level.dy > 7)
	{
		moveBackgroundDown(&level);
	}
	if(level.dy < -7)
	{
		moveBackgroundUp(&level);
	}
	if(bg.dx > 7)
	{
		moveBackgroundRight(&bg);
	}
	if(bg.dx < -7)
	{
     	moveBackgroundLeft(&bg);
	}
	if(bg.dy > 7)
	{
		moveBackgroundDown(&bg);
	}
	if(bg.dy < -7)
	{
		moveBackgroundUp(&bg);
	}
}

void Draw() {
   WaitVBlank();
	UpdateSpriteMemory();
	DrawLevelBackground();
	REG_BG0VOFS = level.y;
	REG_BG0HOFS = level.x;
	REG_BG1VOFS = bg.y;
	REG_BG1HOFS = bg.x;
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
