#include <stdlib.h>
#include <math.h>
#include "gba.h"
#include "maps.h"

#include "sprite/robotsprite.h"
#include "sprite/missile.h"
#include "sprite/vampire.h"
#include "sprite/bars.h"
#include "sprite/fuelbars.h"
#include "sprite/powerups.h"

#include "sprite.h"

//Sound Stuff
#include "Sound.h"
#include "timer.h"
#include "Rocket.c"
#include "Idle2.c"
#include "Start.c"
#include "max.c"
#include "Fire.c"

int healthBarLoadSpot, fuelBarLoadSpot, x, n, y, missileDX = 0;
int recentlyShot = 0, recentlyHit = 0, recentlyDied = 0;
int maxLevel = 1;
bool missile = false;
bool needReset = false;

int oldVOFFS, oldHOFFS;

sound s_move = {&rocket, 8000, 32044};
sound s_jump = {&max, 8000, 7162};
sound s_start = {&start, 8000, 10454};
sound s_idle = {&idle2, 8000, 27518};
sound s_shoot = {&fire, 8000, 11164};

void updateMissile();
void despawnMissile();
void updateVampire();

inline void DrawPixel3(int x, int y, unsigned short c)
{
	videoBuffer[y*240+x]=c;
}

void UpdateSpriteMemory(void)
{
	DMAFastCopy((void*)sprites, (void*)SpriteMem, 512, DMA_16NOW);
}

void Initialize() {
	InitMaps();
	InitSprites();
	SetInterupt();
	
	PlaySound(&s_start);
	
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
		level.tileSize/4,DMA_32NOW);
   DMAFastCopy((void*)bg_Tiles,(void*)CharBaseBlock(2),
		bg.tileSize/4,DMA_32NOW);
	DMAFastCopy((void*)missilePalette,(void*)SpritePal,256,DMA_16NOW);

	SpritePal[66] = RGB(31,31,31);
	
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

void loadFullscreenBitmap(u16 *bitmap) {
   DMAFastCopy(bitmap, videoBuffer, 19200, DMA_32NOW);
}

inline void EnableScreen() {
   SetMode(0x0 | OBJ_ENABLE | OBJ_MAP_1D | BG0_ENABLE | BG1_ENABLE);
}

void reset() {
   needReset = false;
   currLevel = 0;
   SetMode(0x0);
   Initialize();
   WaitVBlank();
   EnableScreen();
   LoadContent();
}

void resetAfterLoop() {
   needReset = true;
}

void gameOver() {
   SetMode(MODE_3 | BG2_ENABLE);
	loadFullscreenBitmap(gameover_Bitmap);
	while(!key_hit(KEY_START)) {
      ButtonPoll();
   }
   resetAfterLoop();
}

bool withinGoal() {
   if(MAIN_HANDLER.worldx + MAIN_HANDLER.width/2 > level.goal.lx &&
      MAIN_HANDLER.worldy + MAIN_HANDLER.height/2 > level.goal.ly &&
      MAIN_HANDLER.worldx + MAIN_HANDLER.width/2 < level.goal.rx &&
      MAIN_HANDLER.worldy + MAIN_HANDLER.height/2 < level.goal.ry) {
      return true;
   } else {
      return false;
   }
}

void nextLevel() {
   if(currLevel >= maxLevel) {
      SetMode(MODE_3 | BG2_ENABLE);
      loadFullscreenBitmap(win_Bitmap);
   	while(!key_hit(KEY_START)) {
         ButtonPoll();
      }
      resetAfterLoop();
   } else {
      SetMode(MODE_3 | BG2_ENABLE);
      loadFullscreenBitmap(loading_Bitmap);
      while(!key_hit(KEY_START)) {
         ButtonPoll();
      }
      SetMode(0x0);
      currLevel++;
      Initialize();
      WaitVBlank();
      EnableScreen();
      LoadContent();
   }
}

void moveViewport() {
	int nx = MAIN_HANDLER.worldx - 120 + MAIN_HANDLER.width/2;
  	if(MAIN_HANDLER.worldx < level.pixelXMax - level.rightOffset
      && MAIN_HANDLER.worldx > 119 + MAIN_HANDLER.width/2)
	{
		level.dx += nx - level.x;
     	bg.dx += (nx/3 - bg.x);
		level.x = nx;
		bg.x += (nx/3 - bg.x);
	}
	int ny = MAIN_HANDLER.worldy - 80 + MAIN_HANDLER.height/2;
	if(MAIN_HANDLER.worldy < level.pixelYMax - level.botOffset
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
        if((!SampleLength || key_was_up(KEY_LEFT)) && (MAIN_HANDLER.mode == GROUND) && !missileFire)
            PlaySound(&s_move);

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
        if((!SampleLength || key_was_up(KEY_RIGHT)) && MAIN_HANDLER.mode == GROUND && !missileFire)
            PlaySound(&s_move);
        
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
			
    if(MAIN_HANDLER.totalFuel < 440)
    {
        MAIN_HANDLER.fuel += 1;
        MAIN_HANDLER.totalFuel += 1;
    }
}

void fuelUpdate()
{
    //FUEL BAR INCREASES
    if(MAIN_HANDLER.fuel > 40)
    {
        MAIN_HANDLER.fuel -= 40;

        if (FUELBAR_HANDLER.idle.currFrame == 0)
        {// Already showing full bar
        }
        else
        {
            // Load the previous health bar frame
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

    //FUEL BAR DECREASES
    if(MAIN_HANDLER.fuel < 0 && MAIN_HANDLER.totalFuel > 0)
    {
        MAIN_HANDLER.fuel += 40;
		if (FUELBAR_HANDLER.idle.currFrame == FUELBAR_HANDLER.idle.numFrames)
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
            if(FUELBAR_HANDLER.idle.currFrame == 11)
            {
                MAIN_HANDLER.fuel = 0;
                MAIN_HANDLER.totalFuel = 0;
            }				
        }
    }
}

void Update() {
   if(needReset)
      reset();
   if(withinGoal())
      nextLevel();

   fuelUpdate();
   ButtonPoll();
   moveXDir();
   if(MAIN_HANDLER.mode == GROUND)
   {
      if(MAIN_HANDLER.totalFuel < 400)
      {
          MAIN_HANDLER.fuel += 1;
          MAIN_HANDLER.totalFuel += 1;
      }

   	if(key_hit(KEY_A))
   	{
   		MAIN_HANDLER.mode = AIR;
   		MAIN_HANDLER.yspd -= MAIN_HANDLER.jumpStr;
   		
   		PlaySound(&s_jump);
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
           MAIN_SPRITE.attribute2=NextFrameLocation(&(MAIN_HANDLER.running));
           if(MAIN_HANDLER.yspd > -2)
               MAIN_HANDLER.yspd -= 0.25;
            MAIN_HANDLER.fuel -= 4;
            MAIN_HANDLER.totalFuel -= 4;
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
   
   if(!SampleLength || key_released(KEY_RIGHT) || key_released(KEY_LEFT))
	   PlaySound(&s_idle);
   
   move(&MAIN_HANDLER,MAIN_HANDLER.worldx + MAIN_HANDLER.xspd,
   	MAIN_HANDLER.worldy + MAIN_HANDLER.yspd);
   if(MAIN_HANDLER.x < 240 && MAIN_HANDLER.x >= 0)
   	MAIN_SPRITE.attribute1 = SET_X(MAIN_SPRITE.attribute1, MAIN_HANDLER.x);
   if(MAIN_HANDLER.y >= 0 && MAIN_HANDLER.y < 160)
   	MAIN_SPRITE.attribute0 = SET_Y(MAIN_SPRITE.attribute0, MAIN_HANDLER.y);
   else if(MAIN_HANDLER.y >= 160)
      gameOver();

   updateMissile();
   updateVampire();
   moveViewport();
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


/*	if (missile)
	{
		MISSILE_SPRITE.attribute0 -= level.y - oldVOFFS;
		MISSILE_HANDLER.y -= level.y -oldVOFFS;
		MISSILE_SPRITE.attribute1 -= level.x - oldHOFFS;
		MISSILE_HANDLER.x -= level.x - oldHOFFS;
	}
*/
	if (recentlyDied)
	{
		spriteHandlers[4].y -= level.y - oldVOFFS;
		sprites[4].attribute1 -= level.y - oldVOFFS;
		spriteHandlers[4].x -= level.x - oldHOFFS;
		sprites[4].attribute1 -= level.x - oldHOFFS;
	}
	
	
	REG_BG0VOFS = level.y;
	REG_BG0HOFS = level.x;
	REG_BG1VOFS = bg.y;
	REG_BG1HOFS = bg.x;
	
	oldVOFFS = level.y;
	oldHOFFS = level.x;
	
	
}

int main()
{
   EnableScreen();
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
	else if (MISSILE_HANDLER.dir == 1 && missile && checkCDSensors(&MISSILE_HANDLER, MISSILE_HANDLER.worldx + MISSILE_HANDLER.xspd, MISSILE_HANDLER.worldy) != -1)
	{// Check horizontal collision with map.
		recentlyHit = 15;
	}
	else if (MISSILE_HANDLER.dir == -1 && missile && checkCDSensors(&MISSILE_HANDLER, MISSILE_HANDLER.worldx + MISSILE_HANDLER.xspd - 30, MISSILE_HANDLER.worldy) != -1)
	{// Check horizontal collision with map.
		recentlyHit = 15;
	}
	else if (missile && checkSpriteCollision(&MISSILE_HANDLER, &spriteHandlers[4]))
	{// Check to see if missile hit vampire
	
		recentlyHit = 15;
		
		spriteHandlers[4].health -= 50;
		
		if (spriteHandlers[4].health == 0)
		{
			recentlyDied = 30;
			spriteHandlers[4].alive = false;
		}
	}
	else if (key_hit(KEY_B) && missile == false)
	{// Shoot a missile if B is hit
        PlaySound(&s_shoot);
        missileFire = true;
	
		MAIN_SPRITE.attribute2 = SPRITE_CHUNKS32_SQUARE * 8;
		recentlyShot = 8;
		missile = true;
		missileDX = 0;
		MISSILE_HANDLER.xspd = 0;
		
			
		// "Spawn" the missile
		MISSILE_SPRITE.attribute0 = SET_MODE(MISSILE_SPRITE.attribute0, SPRITE_ENABLE);
		
		MISSILE_HANDLER.worldx = MAIN_HANDLER.worldx + 27;
		MISSILE_HANDLER.worldy = MAIN_HANDLER.worldy + 7;
		
		
		if (MAIN_HANDLER.flipped == 1)
		{// Robot facing left
				
			// Adjust spawn
			MISSILE_HANDLER.worldx -= 27;
				
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
		int goodX, goodY;
		


		MISSILE_HANDLER.x += MISSILE_HANDLER.xspd;
		MISSILE_HANDLER.worldx += MISSILE_HANDLER.xspd;
			
	
	
		setSpriteLoc(&MISSILE_HANDLER, MISSILE_HANDLER.worldx, MISSILE_HANDLER.worldy);
	
	
	
	
		if(MISSILE_HANDLER.x < 240 && MISSILE_HANDLER.x > 0)
		{
			MISSILE_SPRITE.attribute1 = SET_X(MISSILE_SPRITE.attribute1, MISSILE_HANDLER.x);
			goodX = true;
		}
		else
		{
			goodX = false;
		}
		if(MISSILE_HANDLER.y >= 0 && MISSILE_HANDLER.y < 160)
		{
			MISSILE_SPRITE.attribute0 = SET_Y(MISSILE_SPRITE.attribute0, MISSILE_HANDLER.y);		
			goodY = true;
		}
		else
		{
			goodY = false;
		}
		
		
		if (goodX && goodY)
		{
			MISSILE_SPRITE.attribute0 = SET_MODE(MISSILE_SPRITE.attribute0, SPRITE_ENABLE);
		}
		else
		{	
			MISSILE_SPRITE.attribute0 = SET_MODE(MISSILE_SPRITE.attribute0, SPRITE_DISABLE);
			despawnMissile();
		}
	
		if (missileDX % 8 == 0)
		{
			if (MISSILE_HANDLER.dir == 1)
			{
				MISSILE_HANDLER.xspd++;
			}
			else
			{
				MISSILE_HANDLER.xspd--;
			}
			MISSILE_SPRITE.attribute2=NextFrameLocation(&(MISSILE_HANDLER.running));
		}	
		missileDX++;
		
		
		
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
	missileDX = 0;
}



void updateVampire()
{
	if (spriteHandlers[4].alive == true)
	{
		if(spriteHandlers[4].mode == AIR)
		{
			if(spriteHandlers[4].yspd < 10)
				spriteHandlers[4].yspd = spriteHandlers[4].yspd + .25;
		}
	

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
					spriteHandlers[4].xspd = 1;
					
				}
				else
				{//Go left
					sprites[4].attribute2 = NextFrameLocation(&(spriteHandlers[4].running));
					spriteHandlers[4].xspd = -1;
				}
			}
		}
		else
		{//Idle
			sprites[4].attribute2 = spriteHandlers[4].idle.frameLocation[0];
			spriteHandlers[4].xspd = 0;
		}
		
		boolean goodX, goodY;

		move(&spriteHandlers[4], spriteHandlers[4].worldx + spriteHandlers[4].xspd, spriteHandlers[4].worldy + spriteHandlers[4].yspd);
		
		if(spriteHandlers[4].x < 240 && spriteHandlers[4].x >= 0)
		{
			sprites[4].attribute1 = SET_X(sprites[4].attribute1, spriteHandlers[4].x);
			goodX = true;
		}
		else
		{
			goodX = false;
		}
		if(spriteHandlers[4].y >= 0 && spriteHandlers[4].y < 160)
		{
			sprites[4].attribute0 = SET_Y(sprites[4].attribute0, spriteHandlers[4].y);		
			goodY = true;
		}
		else
		{
			goodY = false;
		}
		
		
		if (goodX && goodY)
		{
			spriteHandlers[4].onScreen = true;
			sprites[4].attribute0 = SET_MODE(sprites[4].attribute0, SPRITE_ENABLE);
		}
		else
		{
			spriteHandlers[4].onScreen = false;
			
			sprites[4].attribute0 = SET_MODE(sprites[4].attribute0, SPRITE_DISABLE);
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




