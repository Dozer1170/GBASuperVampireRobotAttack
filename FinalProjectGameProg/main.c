#include <stdlib.h>
#include <math.h>
#include "gba.h"
#include "maps.h"
#include "sprite.h"

#include "sprite/robotsprite.h"
#include "sprite/missile.h"
#include "sprite/vampire.h"
#include "sprite/bars.h"
#include "sprite/fuelbars.h"
#include "sprite/powerups.h"



int healthBarLoadSpot, fuelBarLoadSpot, x, n, y, missileX = 0;
int recentlyShot = 0, recentlyHit = 0, recentlyDied = 0;
boolean missile = false;

void updateMissile();
void despawnMissile();
void updateVampire();



void UpdateSpriteMemory(void)
{
	DMAFastCopy((void*)sprites, (void*)SpriteMem, 512, DMA_16NOW);
}

void Initialize() {
	SetMode(0x0 | BG0_ENABLE | BG1_ENABLE | OBJ_ENABLE | OBJ_MAP_1D);
	InitMaps();
	InitSprites();
	
	
	//Set up timers
	REG_TM1CNT = TIMER_FREQUENCY_256;
	REG_TM2CNT = TIMER_ENABLE | TIMER_OVERFLOW;
	REG_TM1CNT |= TIMER_ENABLE;
	
}

// Loads the tiles and palettes
void LoadContent() {
	DMAFastCopy((void*)level_Palette,(void*)BGPaletteMem,
		256,DMA_16NOW);
	DMAFastCopy((void*)level_Tiles,(void*)CharBaseBlock(0),
		level.mapSize/4,DMA_32NOW);
	DMAFastCopy((void*)missilePalette,(void*)SpritePal,256,DMA_16NOW);

	
	int nextSprite = 0;
	
	// Load robot sprite data into memory
	x = 0;
	for(n = nextSprite; n < nextSprite + SPRITE_DATA32_SQUARE * 9; n++)
	{
		SpriteData[n] = robotspriteData[x];
		x++;
	}	
	nextSprite = n;
	
	
	// Load health bar sprite data into memory
	x = 0;
	healthBarLoadSpot = nextSprite;
	for (n = nextSprite; n < nextSprite + SPRITE_DATA64_TALL * 1; n++)
	{
		SpriteData[n] = barsData[x];
		x++;
	}
	nextSprite = n;
	
	
	
	// Load fuel bar sprite data into memory
	x = 0;
	fuelBarLoadSpot = nextSprite;
	for (n = nextSprite; n < nextSprite + SPRITE_DATA64_TALL * 1; n++)
	{
		SpriteData[n] = fuelbarsData[x];
		x++;
	}
	nextSprite = n;
	
	
	
	// Load missile sprite data into memory
	x = 0;
	for (n = nextSprite; n < nextSprite + SPRITE_DATA16_SQUARE * 4; n++)
	{
		SpriteData[n] = missileData[x];
		x++;
	}
	nextSprite = n;
	
	
	
	// Load vampire sprite data into memory
	x = 0;
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
        if(MAIN_HANDLER.totalFuel < 400)
        {
            MAIN_HANDLER.fuel += 1;
            MAIN_HANDLER.totalFuel += 1;
        }
        if(MAIN_HANDLER.fuel > 40)
        {
            MAIN_HANDLER.fuel -= 40;
			
			
			
			
			if (FUELBAR_HANDLER.idle.currFrame == 0)
			{// Already showing full bar
			}
			else
			{
				// Load the previous fuel bar frame
				FUELBAR_HANDLER.idle.currFrame--;
				
				x = SPRITE_DATA32_SQUARE * 9 + SPRITE_DATA64_TALL * 1;
				y = FUELBAR_HANDLER.idle.currFrame * SPRITE_DATA64_TALL;
				for (n = 0; n < SPRITE_DATA64_TALL; n++)
				{
					SpriteData[x] = fuelbarsData[n + y];
					x++;
				}	
			}
			
        }
            
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
	    if( key_is_down( KEY_R ) && MAIN_HANDLER.totalFuel > 0 )
	    {
	        if(MAIN_HANDLER.yspd > -2)
	            MAIN_HANDLER.yspd -= 0.25;
            MAIN_HANDLER.fuel -= 2;
            MAIN_HANDLER.totalFuel -= 2;
            if(MAIN_HANDLER.fuel <= 0 && MAIN_HANDLER.totalFuel > 0) {
                MAIN_HANDLER.fuel += 40;
				
				
				
				if (HEALTHBAR_HANDLER.idle.currFrame == HEALTHBAR_HANDLER.idle.numFrames)
				{// Already showing empty bar. Don't load next stuff
				}
				else
				{
					// Load the next level of the health bar
					FUELBAR_HANDLER.idle.currFrame++;
					
					x = SPRITE_DATA32_SQUARE * 9 + SPRITE_DATA64_TALL * 1;
					y = FUELBAR_HANDLER.idle.currFrame * SPRITE_DATA64_TALL;
					for (n = 0; n < SPRITE_DATA64_TALL; n++)
					{
						SpriteData[x] = fuelbarsData[n + y];
						x++;
					}				
				}
            }
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
	}
	move(&MAIN_HANDLER,MAIN_HANDLER.worldx + MAIN_HANDLER.xspd,
		MAIN_HANDLER.worldy + MAIN_HANDLER.yspd);
	if(MAIN_HANDLER.x < 240 && MAIN_HANDLER.x >= 0)
		MAIN_SPRITE.attribute1 = SET_X(MAIN_SPRITE.attribute1, MAIN_HANDLER.x);
	if(MAIN_HANDLER.y >= 0 && MAIN_HANDLER.y < 160)
		MAIN_SPRITE.attribute0 = SET_Y(MAIN_SPRITE.attribute0, MAIN_HANDLER.y);
		
		
		
		
	updateMissile();
	updateVampire();
	
	
	
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















void updateMissile()
{
	if (recentlyShot)
	{
		MAIN_SPRITE.attribute2 = SPRITE_CHUNKS32_SQUARE * 8;
		recentlyShot--;
	}
	
	
	
	if (recentlyHit)
	{
		if (recentlyHit / 6 == 0)
		{
			MISSILE_SPRITE.attribute2 = MISSILE_HANDLER.idle.frameLocation[1];
		}
		else
		{
			MISSILE_SPRITE.attribute2 = MISSILE_HANDLER.idle.frameLocation[0];
		}
		
		
		recentlyHit--;
		
		if (recentlyHit == 0)
		{
			despawnMissile();
		}
	}
	else if (checkSpriteCollision(&MISSILE_HANDLER, &spriteHandlers[4]) && !recentlyHit)
	{// Check to see if missile hit vampire
		recentlyHit = 11;
		
		spriteHandlers[4].health -= 50;
		
		if (spriteHandlers[4].health == 0)
		{
			recentlyDied = 30;
			spriteHandlers[4].alive = false;
		}
	}
	else if (key_hit(KEY_B) && missile == false)
	{// Shoot a missile if B is hit
		MAIN_SPRITE.attribute2 = SPRITE_CHUNKS32_SQUARE * 8;
		recentlyShot = 6;
		missile = true;
		missileX = 0;
			
		// "Spawn" the missile
		MISSILE_SPRITE.attribute1 &= 0xFE00;
		MISSILE_SPRITE.attribute1 |= MAIN_HANDLER.x + 27;
		MISSILE_HANDLER.x = MAIN_HANDLER.x + 27;
		MISSILE_HANDLER.worldx = MAIN_HANDLER.worldx + 27;
			
		MISSILE_SPRITE.attribute0 &= 0xFF00;
		MISSILE_SPRITE.attribute0 |= MAIN_HANDLER.y + 7;
		MISSILE_HANDLER.y = MAIN_HANDLER.y + 7;
		MISSILE_HANDLER.worldy = MAIN_HANDLER.worldy + 7;
			
			
		if (MAIN_HANDLER.flipped == 1)
		{// Robot facing left
				
			// Adjust spawn
			MISSILE_SPRITE.attribute1 -= 27;
				
			// Flip the rocket
			MISSILE_SPRITE.attribute1 |= HORIZONTAL_FLIP;
			MISSILE_HANDLER.dir = -1;
		}
		else if (MAIN_HANDLER.flipped == 0)
		{// Robot facing right
			if (MISSILE_HANDLER.dir == 1)
			{
				// Already facing the right direction
			}
			else
			{// Rocket facing left
				// Flip the rocket
				MISSILE_SPRITE.attribute1 &= ~HORIZONTAL_FLIP;
				MISSILE_HANDLER.dir = 1;
			}
		}		
	}
	
	
	
	// MISSILE UPDATE~~~~~~~~~~~~~~~~~~~~~~~~~
	else if (missile == true)
	{					
		if (MISSILE_HANDLER.dir == 1)
		{//Going right
				
			if (MISSILE_HANDLER.x > 230)
			{// Hit the right edge
				despawnMissile();
			}
			else
			{
				MISSILE_HANDLER.x += MISSILE_HANDLER.xspd;
				MISSILE_SPRITE.attribute1 += MISSILE_HANDLER.xspd;
			}
			
		}
		else
		{//Going left
			
			if (MISSILE_HANDLER.x < 0)
			{// Hit the left edge
				despawnMissile();	
			}
			else
			{ 
				MISSILE_SPRITE.attribute1 -= MISSILE_HANDLER.xspd;
				MISSILE_HANDLER.x -= MISSILE_HANDLER.xspd;
			}
		}
			
			
			
		if (missileX % 8 == 0)
		{
			MISSILE_HANDLER.xspd++;
			MISSILE_SPRITE.attribute2=NextFrameLocation(&(MISSILE_HANDLER.running));
		}
		
		missileX++;
	}
	// END MISSILE UPDATE~~~~~~~~~~~~~~~~~~~~~

		
} // End updateMissile() function


void despawnMissile()
{
	MISSILE_SPRITE.attribute1 &= 0xFE00;
	MISSILE_SPRITE.attribute1 |= 240;
	MISSILE_HANDLER.x = 240;
			
	MISSILE_SPRITE.attribute0 &= 0xFF00;
	MISSILE_SPRITE.attribute1 |= 160;
	MISSILE_HANDLER.y = 160;
					
	MISSILE_HANDLER.xspd = 0;
					
	missile = false;
	missileX = 0;
}




void updateVampire()
{
	if (spriteHandlers[4].alive == true)
	{
		if (REG_TM2D % 6)
		{
			if (rand() % 10 == 0)
			{//Change direction
				
				if (spriteHandlers[4].dir == 1)
				{//Flip left
					sprites[4].attribute1 |= HORIZONTAL_FLIP;
					spriteHandlers[4].dir = -1;
					spriteHandlers[4].flipped = true;
				}
				else
				{//Flip Right
					sprites[4].attribute1 &= ~HORIZONTAL_FLIP;
					spriteHandlers[4].dir = 1;
					spriteHandlers[4].flipped = false;
				}
			}
			else
			{//Keep moving
				if (spriteHandlers[4].dir == 1)
				{//Go right
					sprites[4].attribute2 = NextFrameLocation(&(spriteHandlers[4].running));
					sprites[4].attribute1 += 1;
					spriteHandlers[4].x += 1;
				}
				else
				{//Go left
					sprites[4].attribute2 = NextFrameLocation(&(spriteHandlers[4].running));
					sprites[4].attribute1 -= 1;
					spriteHandlers[4].x -= 1;
				}
			}
		}
		else
		{//Idle
			sprites[4].attribute2 = spriteHandlers[4].idle.frameLocation[0];
		}
	}
	else
	{// Dead
		if (recentlyDied)
		{// Flickery death
			
			if (recentlyDied % 5 == 0)
			{
				sprites[4].attribute0 &= 0xFF00;
				sprites[4].attribute0 |= 160;
	
				sprites[4].attribute1 &= 0xFE00;
				sprites[4].attribute1 |= 240;
			}
			else
			{
				sprites[4].attribute0 &= 0xFF00;
				sprites[4].attribute0 |= spriteHandlers[4].y;
	
				sprites[4].attribute1 &= 0xFE00;
				sprites[4].attribute1 |= spriteHandlers[4].x;
			}
			
			
			recentlyDied--;
			
			if (recentlyDied == 0)
			{
				sprites[4].attribute0 &= 0xFF00;
				sprites[4].attribute0 |= 160;
				spriteHandlers[4].y = 160;
	
				sprites[4].attribute1 &= 0xFE00;
				sprites[4].attribute1 |= 240;
				spriteHandlers[4].x = 240;
			}
		}
	}
}



