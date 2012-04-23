// Function Prototypes
void moveViewport();


// Defines
#define MAIN_HANDLER spriteHandlers[0]
#define MAIN_SPRITE sprites[0]

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
	int startChunk;
	int fuel;
	int totalFuel;
	int health;
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
	u16 tileStart = hitmap.srcMap[(x/8) + (level.levelWidth * (y/8))] * 64;
	u8 pixel = level_Tiles[tileStart + (x%8) + (8*(y%8))];
	if(1 == pixel || 2 == pixel || 3 == pixel || 4 == pixel || 5 == pixel || 6 == pixel) {
		rval = 1;
	}
	return rval;
}



//Checks a single pixel if there is a solid object and sets sprites
//angle values based upon what it it
bool checkSolidPixelCollisionSet(SpriteHandler *sprite, int x, int y) {
	bool rval = 0;
	u16 tileStart = hitmap.srcMap[(x/8) + (level.levelWidth * (y/8))] * 64;
	u8 pixel = level_Tiles[tileStart + (x%8) + (8*(y%8))];
	if(1 == pixel || 2 == pixel || 3 == pixel) {
		rval = 1;
		sprite->angle.sinAngle = 0;
		sprite->angle.cosAngle = 1;
		sprite->angle.slopeFactor = 0;
	}
	if(4 == pixel) {
		rval = 1;
		sprite->angle.sinAngle = .7;
		sprite->angle.cosAngle = .7;
		sprite->angle.slopeFactor = -.15;
	}
	if(5 == pixel) {
		rval = 1;
		sprite->angle.sinAngle = .7;
		sprite->angle.cosAngle = .7;
		sprite->angle.slopeFactor = .15;
	}
	if(6 == pixel) {
		rval = 1;
		sprite->angle.sinAngle = .44;
		sprite->angle.cosAngle =  .89;
		sprite->angle.slopeFactor = -.08;
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
	   moveViewport();
		setSpriteLoc(sprite,x,newY);
	} else if(sprite->mode == AIR) {
      int newY =  checkABSensors(sprite,x,y);
		if(newY == -1 || sprite->yspd < 0) { //in air
         newY = checkEFSensors(sprite,x,y);
         if((newX = checkCDSensors(sprite,x,newY)) != -1) {
			   x = newX;
		   }
			moveViewport();
			setSpriteLoc(sprite,x,newY);
		} else { //landed
			sprite->mode = GROUND;
			sprite->yspd = 0;
			moveViewport();
			setSpriteLoc(sprite,x,newY);
		}
	}
}



bool spriteCollision(sprite1, sprite1VelX, sprite1VelY, sprite2, sprite2VelX, sprite2VelY)
{
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
	MAIN_HANDLER.x = 50;
	MAIN_HANDLER.y = 50;
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
	MAIN_HANDLER.totalFuel = 400;
	MAIN_HANDLER.health = 10;
	MAIN_SPRITE.attribute0 = COLOR_256 | SQUARE | MAIN_HANDLER.x;
	MAIN_SPRITE.attribute1 = SIZE_32 | MAIN_HANDLER.y;
	MAIN_SPRITE.attribute2 = MAIN_HANDLER.standing.frameLocation[0];
	
	
	// Initialize health bar sprite
	sprites[1].attribute0 = TALL | COLOR_256 | 0;
	sprites[1].attribute1 = SIZE_64 | 0;
	sprites[1].attribute2 = 256;

    spriteHandlers[1].idle.currFrame = 0;
    spriteHandlers[1].idle.numFrames = 10;
	spriteHandlers[1].idle.frameLocation[0] = 256;
	spriteHandlers[1].idle.frameLocation[1] = spriteHandlers[1].idle.frameLocation[0] + SPRITE_CHUNKS64_TALL;
	spriteHandlers[1].idle.frameLocation[2] = spriteHandlers[1].idle.frameLocation[1] + SPRITE_CHUNKS64_TALL;
	spriteHandlers[1].idle.frameLocation[3] = spriteHandlers[1].idle.frameLocation[2] + SPRITE_CHUNKS64_TALL;
	spriteHandlers[1].idle.frameLocation[4] = spriteHandlers[1].idle.frameLocation[3] + SPRITE_CHUNKS64_TALL;
	spriteHandlers[1].idle.frameLocation[5] = spriteHandlers[1].idle.frameLocation[4] + SPRITE_CHUNKS64_TALL;
	spriteHandlers[1].idle.frameLocation[6] = spriteHandlers[1].idle.frameLocation[5] + SPRITE_CHUNKS64_TALL;
	spriteHandlers[1].idle.frameLocation[7] = spriteHandlers[1].idle.frameLocation[6] + SPRITE_CHUNKS64_TALL;
	spriteHandlers[1].idle.frameLocation[8] = spriteHandlers[1].idle.frameLocation[7] + SPRITE_CHUNKS64_TALL;
	spriteHandlers[1].idle.frameLocation[9] = spriteHandlers[1].idle.frameLocation[8] + SPRITE_CHUNKS64_TALL;


	
	// Initialize vampire sprite
	sprites[2].attribute2 = 896;
}

