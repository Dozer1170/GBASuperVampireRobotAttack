#include "gba.h"
#include <math.h>
#include "maps.h"
#include "sprite.h"
#include "sprite/robotsprite.h"

void UpdateSpriteMemory(void)
{
	DMAFastCopy((void*)sprites, (void*)SpriteMem, 512, DMA_16NOW);
}

void Initialize() {
	SetMode(0x0 | BG0_ENABLE | BG2_ENABLE | OBJ_ENABLE | OBJ_MAP_1D);
	InitMaps();
	InitSprites();
}

// Loads the tiles and palettes
void LoadContent() {
	DMAFastCopy((void*)level_Palette,(void*)BGPaletteMem,
		FACTORY_PALETTE_SIZE,DMA_16NOW);
	DMAFastCopy((void*)level_Tiles,(void*)CharBaseBlock(0),
		FACTORY_TILE_SET_SIZE/4,DMA_32NOW);
	DMAFastCopy((void*)robotspritePalette,(void*)SpritePal,256,DMA_16NOW);

	int n;
	for(n = 0; n < 4096; n++)
		SpriteData[n] = robotspriteData[n];
}

void moveViewport() {
	int nx = MAIN_HANDLER.worldx - 120 + MAIN_HANDLER.width/2;
  	if(MAIN_HANDLER.worldx < MAP_PIXEL_X_MAX - 136
      && MAIN_HANDLER.worldx > 119 + MAIN_HANDLER.width/2)
	{
     	level.dx += nx - level.x;
	   level.x = nx;
	}
	int ny = MAIN_HANDLER.worldy - 80 + MAIN_HANDLER.height/2;
	if(MAIN_HANDLER.worldy < MAP_PIXEL_Y_MAX - 96
      && MAIN_HANDLER.worldy > 79 + MAIN_HANDLER.height/2)
	{
     	level.dy += ny - level.y;
	   level.y = ny;
	}
}

void Update() {
    ButtonPoll();
    if(MAIN_HANDLER.mode == GROUND)
    {
		if(key_is_down(KEY_LEFT))
		{
         if( MAIN_HANDLER.dir == -1)
			{
				if(MAIN_HANDLER.gspd + MAIN_HANDLER.acc < MAIN_HANDLER.maxGspd)
					MAIN_HANDLER.gspd  += MAIN_HANDLER.acc;
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
				if(MAIN_HANDLER.gspd + MAIN_HANDLER.acc < MAIN_HANDLER.maxGspd)
					MAIN_HANDLER.gspd  += MAIN_HANDLER.acc;
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
		if(key_hit(KEY_A))
		{
			MAIN_HANDLER.mode = AIR;
			MAIN_HANDLER.yspd = MAIN_HANDLER.yspd - 5;
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
