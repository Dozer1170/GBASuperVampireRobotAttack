// Defines
#define MAIN_HANDLER spriteHandlers[0]
#define MAIN_SPRITE sprites[0]

#define HEALTHBAR_HANDLER spriteHandlers[1]
#define HEALTHBAR_SPRITE sprites[1]

#define FUELBAR_HANDLER spriteHandlers[2]
#define FUELBAR_SPRITE sprites[2]

#define MISSILE_HANDLER spriteHandlers[3]
#define MISSILE_SPRITE sprites[3]

#define VAMPIRE_HANDLER spriteHandlers[4]
#define VAMPIRE_SPRITE sprites[4]


#define GROUND 0
#define AIR 1

// Typedefs
typedef struct tagSprite
{
	u16 attribute0;
	u16 attribute1;
	u16 attribute2;
	s16 attribute3;
} ALIGN4 Sprite,*pSprite;



typedef struct tagAnimationHandler {
	int numFrames, currFrame;
	int frameLocation[10];
} AnimationHandler;



typedef struct tagHitBox
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
} HitBox;



typedef struct tagAngleInfo {
	float cosAngle, sinAngle;
	float slopeFactor;
} AngleInfo;



typedef struct tagSpriteHandler
{
	int x, y;
	int mode;
	int worldx, worldy;
	float acc, dec, xspd, yspd;
	float gspd, maxGspd;
    float jumpStr;
	int width, height;
	int alive;
	int dir;
	int flipped;
	int fuel;
	int totalFuel;
	int health;
	boolean onScreen;
	boolean passed;
	AngleInfo angle;
	AnimationHandler idle;
	AnimationHandler standing;
	AnimationHandler running;
	AnimationHandler jumpUp;
	AnimationHandler jumpDown;
	HitBox hitBox;
} SpriteHandler;



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

// Global Variables
// Copy of OAM
Sprite sprites[128];

// Sprite Handler array
SpriteHandler spriteHandlers[128];

void gameOver();

void takeDamage(SpriteHandler *sprite, int damage) 
{
	static int dmgDX;
	int x, y, n;
	
	if(sprite->health - damage < 1)
		gameOver();
	else
    {
		sprite->health -= damage;
		dmgDX += damage;
		
		
		while (dmgDX - 10 > 0)
		{
			//Load lower health bar
			HEALTHBAR_HANDLER.idle.currFrame++;
					
			x = SPRITE_DATA32_SQUARE * 9;
			y = HEALTHBAR_HANDLER.idle.currFrame * SPRITE_DATA64_TALL;
			for (n = 0; n < SPRITE_DATA64_TALL; n++)
			{
				SpriteData[x] = barsData[n + y];
				x++;
			}
			dmgDX -= 10;
		}
	}
}

int NextFrameLocation(AnimationHandler *handler) {
	if(handler->currFrame >= handler->numFrames - 1)
	    handler->currFrame = -1;
	return handler->frameLocation[++handler->currFrame];
}

int PrevFrameLocation(AnimationHandler *handler) {
	if(handler->currFrame <= 0)
	    handler->currFrame = handler->numFrames;
	return handler->frameLocation[--handler->currFrame];
}

void setSpriteLoc(SpriteHandler *sprite, int x, int y) {
	if(x >= 0 && x < level.pixelXMax) {
	    sprite->worldx = x;
	    sprite->x = sprite->worldx - level.x;
	}

	if(y >= 0 && y < level.pixelYMax) {
	    sprite->worldy = y;
	    sprite->y = sprite->worldy - level.y;
	}
}



//Checks a single pixel if there is a solid object
bool checkSolidCollision(SpriteHandler *sprite, int x, int y) {
	bool rval = 0;
	int tileStart = hitmap.srcMap[(x/8) + (level.levelWidth * (y/8))] * 64;
	int pixel = hitmap_Tiles[tileStart + (x%8) + (8*(y%8))];
   int color = level_Palette[pixel];
	if(RED == color || DGREEN == color || YELLOW == color || BLUE == color ||
      PURPLE == color || LGREEN == color) {
		rval = 1;
	} else if( PINK == color ) {
      rval = 1;
      takeDamage(sprite, 10);
   }
	return rval;
}



//Checks a single pixel if there is a solid object and sets sprites
//angle values based upon what it it
bool checkSolidPixelCollisionSet(SpriteHandler *sprite, int x, int y) {
	bool rval = 0;
	int srcIndex = (x/8) + (level.levelWidth * (y/8));
	if(srcIndex >= hitmap.mapSize)
      return 0;
	int tileStart = hitmap.srcMap[srcIndex] * 64;
	int pixel = hitmap_Tiles[tileStart + (x%8) + (8*(y%8))];
	int color = level_Palette[pixel];
	if(RED == color || BLUE == color || PURPLE == color) {
		rval = 1;
		sprite->angle.sinAngle = 0;
		sprite->angle.cosAngle = 1;
		sprite->angle.slopeFactor = 0;
	}
	if(DGREEN == color) {
		rval = 1;
		sprite->angle.sinAngle = .7;
		sprite->angle.cosAngle = .7;
		sprite->angle.slopeFactor = -.15;
	}
	if(YELLOW == color) {
		rval = 1;
		sprite->angle.sinAngle = .7;
		sprite->angle.cosAngle = .7;
		sprite->angle.slopeFactor = .15;
	}
	if(LGREEN == color) {
		rval = 1;
		sprite->angle.sinAngle = .44;
		sprite->angle.cosAngle =  .89;
		sprite->angle.slopeFactor = -.08;
	}
	if(PINK == color) {
      rval = 1;
      takeDamage(sprite, 10);
   }
	return rval;
}



//returns the 'topmost' y value where there is a collision
//on the left and right side of the sprite
int checkABSensors(SpriteHandler *sprite, int nextX, int nextY) {
	int i;
	int leftX = nextX + sprite->hitBox.xOffset;
	int rightX = nextX + sprite->width - sprite->hitBox.negXOffset - 4;
	int y = nextY + sprite->height/2;
	int max = sprite->height/2 + 4;
	for(i = 0; i < max; i++) {
		if(checkSolidPixelCollisionSet(sprite,leftX,y+i)) {
			return y+i-sprite->height;
		}
  		if(checkSolidPixelCollisionSet(sprite,rightX,y+i)) {
			return y+i-sprite->height;
		}
	}
	return -1;

}



//Checks for map collision in a single horizontal line in the middle
//of the sprite
//Returns xvalue to set sprite at if there is a collision
//else returns -1 for no collision
int checkCDSensors(SpriteHandler *sprite, int nextX, int nextY) {
   int i;
   int topY = nextY + sprite->height / 3;
   int y = nextY + (sprite->height/3) * 2;
   int max = sprite->width - (sprite->hitBox.negXOffset + sprite->hitBox.xOffset);
   if(sprite->dir == 1) {
      int begin = nextX + sprite->hitBox.xOffset;
      for(i = 0; i < max; i++) {
            if(checkSolidCollision(sprite,begin + i,y)) {
                  sprite->gspd = 0;
                  return begin + i + sprite->hitBox.negXOffset - sprite->width;
            }
            if(checkSolidCollision(sprite,begin + i,topY)) {
                  sprite->gspd = 0;
                  return begin + i + sprite->hitBox.negXOffset - sprite->width;
            }
        }
   } else {
      int begin = nextX + sprite->width - sprite->hitBox.xOffset;
      for(i = 0; i < max; i++) {
          if(checkSolidCollision(sprite,begin - i,y)) {
               sprite->gspd = 0;
               return begin - i - sprite->hitBox.negXOffset + 2;
          }
          if(checkSolidCollision(sprite,begin - i,topY)) {
               sprite->gspd = 0;
               return begin - i - sprite->hitBox.negXOffset + 2;
          }
      }
   }
   return -1;
}



//Checks above the sprite, if there is a collision, set yspd to 0
//if no collision, returns the place asked to move (nextY)
int checkEFSensors(SpriteHandler *sprite, int nextX, int nextY) {
   int i;
	int leftX = nextX + sprite->hitBox.xOffset + 3;
	int rightX = nextX + sprite->width - sprite->hitBox.negXOffset - 3;
	int y = nextY + sprite->height/2;
	int max = sprite->height/2;
	for(i = 0; i < max; i++) {
		if(checkSolidCollision(sprite,leftX,y-i)) {
         if(sprite->yspd < 0)
            sprite->yspd = 0;
			return y-i;
		}
  		if(checkSolidCollision(sprite,rightX,y-i)) {
         if(sprite->yspd < 0)
            sprite->yspd = 0;
			return y-i;
		}
	}
	return nextY;
}



void move(SpriteHandler *sprite, int x, int y) {
   int newX;
	if(sprite->mode == GROUND) {
      int newY = sprite->worldy;
		if((newX = checkCDSensors(sprite, x, y)) == -1) {
	        newY = checkABSensors(sprite,x,y);
	        if(newY == -1) {
				sprite->mode = AIR;
				sprite->yspd = sprite->yspd + 1;
	        }
		} else {
         x = newX;
      }
		setSpriteLoc(sprite,x,newY);
	} else if(sprite->mode == AIR) {
      int newY =  checkABSensors(sprite,x,y);
		if(newY == -1 || sprite->yspd < 0) { //in air
         newY = checkEFSensors(sprite,x,y);
         if((newX = checkCDSensors(sprite,x,newY)) != -1) {
			   x = newX;
		   }
			setSpriteLoc(sprite,x,newY);
		} else { //landed
			sprite->mode = GROUND;
			sprite->yspd = 0;
			setSpriteLoc(sprite,x,newY);
		}
	}
}

bool checkSpriteCollision(SpriteHandler *sprite1, SpriteHandler *sprite2)
{
	if (sprite1->x == 240 || sprite2->x == 240)
	{// One of the sprites is offscreen
		return false;
	}
	
	int sprite1_ytop = sprite1->y + sprite1->hitBox.yOffset;
	int sprite1_ybot = sprite1_ytop + sprite1->hitBox.height;
	int sprite2_ytop = sprite2->y + sprite2->hitBox.yOffset;
	int sprite2_ybot = sprite2_ytop + sprite2->hitBox.height;
	
	
	if (sprite1_ybot < sprite2_ytop || sprite2_ybot < sprite1_ytop)
	{
		// Vertical miss
		return false;
	}
	else if (sprite1->x < sprite2->x)
	{// Sprite 1 on left
		if (sprite1->x + sprite1->hitBox.xOffset + sprite1->hitBox.width > sprite2->x + sprite2->hitBox.xOffset)
		{
			return true;
		}
	}
	else
	{// Sprite 2 on left
		if (sprite2->x + sprite2->hitBox.xOffset + sprite2->hitBox.width > sprite1->x - sprite1->width)
		{
			return true;
		}
	}
	
	return false;
}



void InitSprites() {
    int n;
	
	// Move all sprites off-screen
	for(n = 0; n < 128; n++)
	{
		sprites[n].attribute0 = 160;
		sprites[n].attribute1 = 240;
	}
	
	// Initialize robot sprite
	MAIN_HANDLER.alive = 1;
	MAIN_HANDLER.flipped = 0;
	MAIN_HANDLER.width = 32;
	MAIN_HANDLER.height = 32;
	MAIN_HANDLER.standing.frameLocation[0] = 0;
	MAIN_HANDLER.standing.frameLocation[1] = 32;
	MAIN_HANDLER.running.frameLocation[0] = 64;
	MAIN_HANDLER.running.frameLocation[1] = 96;
	MAIN_HANDLER.jumpUp.frameLocation[0] = 128;
	MAIN_HANDLER.jumpUp.frameLocation[1] = 160;
	MAIN_HANDLER.jumpDown.frameLocation[0] = 192;
	MAIN_HANDLER.jumpDown.frameLocation[1] = 224;
	MAIN_HANDLER.standing.currFrame = 0;
	MAIN_HANDLER.running.currFrame = 0;
	MAIN_HANDLER.jumpUp.currFrame = 0;
	MAIN_HANDLER.jumpDown.currFrame = 0;
	MAIN_HANDLER.standing.numFrames = 2;
	MAIN_HANDLER.running.numFrames = 2;
	MAIN_HANDLER.jumpUp.numFrames = 2;
	MAIN_HANDLER.jumpDown.numFrames = 2;
	MAIN_HANDLER.x = 100;
	MAIN_HANDLER.y = 0;
	MAIN_HANDLER.xspd = 0;
	MAIN_HANDLER.yspd = 0;
	MAIN_HANDLER.gspd = 0;
	MAIN_HANDLER.maxGspd = 3;
	MAIN_HANDLER.acc = .20;
	MAIN_HANDLER.dec = 0;
	MAIN_HANDLER.mode = AIR;
	MAIN_HANDLER.dir = 1;
	MAIN_HANDLER.angle.cosAngle = 1;
	MAIN_HANDLER.angle.sinAngle = 0;
	MAIN_HANDLER.angle.slopeFactor = 0;
	MAIN_HANDLER.jumpStr = 5;
	MAIN_HANDLER.hitBox.xOffset = 8;
	MAIN_HANDLER.hitBox.yOffset = 0;
	MAIN_HANDLER.hitBox.negYOffset = 0;
	MAIN_HANDLER.hitBox.negXOffset = 6;
	MAIN_HANDLER.worldx = MAIN_HANDLER.x + level.x;
	MAIN_HANDLER.worldy = MAIN_HANDLER.y + level.y;
	MAIN_HANDLER.fuel = 40;
	MAIN_HANDLER.totalFuel = 440;
	MAIN_HANDLER.health = 100;
	MAIN_SPRITE.attribute0 = COLOR_256 | SQUARE | MAIN_HANDLER.x;
	MAIN_SPRITE.attribute1 = SIZE_32 | MAIN_HANDLER.y;
	MAIN_SPRITE.attribute2 = MAIN_HANDLER.standing.frameLocation[0];
	
	
	
	
	// Initialize health bar sprite
	sprites[1].attribute0 = TALL | COLOR_256 | 0;
	sprites[1].attribute1 = SIZE_64 | 0;
	sprites[1].attribute2 = 288;

   spriteHandlers[1].idle.currFrame = 0;
   spriteHandlers[1].idle.numFrames = 11;
	
	// Initialize fuel bar sprite
	sprites[2].attribute0 = TALL | COLOR_256 | 0;
	sprites[2].attribute1 = SIZE_64 | 210;
	sprites[2].attribute2 = sprites[1].attribute2 + SPRITE_CHUNKS64_TALL;
	
	spriteHandlers[2].idle.currFrame = 0;
	spriteHandlers[2].idle.numFrames = 11;
	
	
	
	// Initialize missile sprite
	sprites[3].attribute0 = SQUARE | COLOR_256 | 160;
	sprites[3].attribute1 = SIZE_16 | 240;
	sprites[3].attribute2 = sprites[2].attribute2 + SPRITE_CHUNKS64_TALL;
	
	spriteHandlers[3].running.numFrames = 2;
	spriteHandlers[3].running.frameLocation[0] = sprites[3].attribute2;
	spriteHandlers[3].running.frameLocation[1] = spriteHandlers[3].running.frameLocation[0] + SPRITE_CHUNKS16_SQUARE;
	
	spriteHandlers[3].idle.numFrames = 2;
	spriteHandlers[3].idle.frameLocation[0] = spriteHandlers[3].running.frameLocation[1] + SPRITE_CHUNKS16_SQUARE;
	spriteHandlers[3].idle.frameLocation[1] = spriteHandlers[3].idle.frameLocation[0] + SPRITE_CHUNKS16_SQUARE;
	
	spriteHandlers[3].hitBox.xOffset = 0;
	spriteHandlers[3].hitBox.yOffset = 3;
	spriteHandlers[3].hitBox.negXOffset = 0;
	spriteHandlers[3].hitBox.negYOffset = 3;
	spriteHandlers[3].hitBox.width = 16;
	spriteHandlers[3].hitBox.height = 9;
	spriteHandlers[3].height = 16;
	spriteHandlers[3].width = 16;
	
	spriteHandlers[3].dir = 1;
	spriteHandlers[3].flipped = 0;
	spriteHandlers[3].xspd = 0;

    initVampires( 200, 10, 4);
    initVampires( 150, 10, 5);
}

void initVampires( int worldx, int worldy, int spriteNumber )
{
	// Initialize vampire sprite
	sprites[spriteNumber].attribute0 = SQUARE | COLOR_256 | 240;
	sprites[spriteNumber].attribute1 = SIZE_32 | 160;
	sprites[spriteNumber].attribute2 = sprites[3].attribute2 + SPRITE_CHUNKS16_SQUARE * 4;

	spriteHandlers[spriteNumber].idle.frameLocation[0] = sprites[spriteNumber].attribute2;
	spriteHandlers[spriteNumber].idle.numFrames = 1;
	
	spriteHandlers[spriteNumber].running.numFrames = 2;
	spriteHandlers[spriteNumber].running.frameLocation[0] = spriteHandlers[spriteNumber].idle.frameLocation[0] + SPRITE_CHUNKS32_SQUARE;
	spriteHandlers[spriteNumber].running.frameLocation[1] = spriteHandlers[spriteNumber].running.frameLocation[0] + SPRITE_CHUNKS32_SQUARE;
	
	spriteHandlers[spriteNumber].hitBox.xOffset = 7;
	spriteHandlers[spriteNumber].hitBox.yOffset = 2;
	spriteHandlers[spriteNumber].hitBox.negXOffset = 7;
	spriteHandlers[spriteNumber].hitBox.negYOffset = 2;
	spriteHandlers[spriteNumber].hitBox.width = 14;
	spriteHandlers[spriteNumber].hitBox.height = 29;
	
	spriteHandlers[spriteNumber].height = 32;
	spriteHandlers[spriteNumber].width = 32;
	
	spriteHandlers[spriteNumber].alive = true;
	spriteHandlers[spriteNumber].health = 100;
	
	spriteHandlers[spriteNumber].dir = 1;
	spriteHandlers[spriteNumber].x = 240;
	spriteHandlers[spriteNumber].worldx = worldx;
	spriteHandlers[spriteNumber].y = 160;	
	spriteHandlers[spriteNumber].worldy = worldy;
	
	spriteHandlers[spriteNumber].yspd = 0;
	spriteHandlers[spriteNumber].xspd = 0;
	
	
	spriteHandlers[spriteNumber].onScreen = true;
	spriteHandlers[spriteNumber].mode = AIR;
}

