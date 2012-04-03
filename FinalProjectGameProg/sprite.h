void moveViewport();

// Defines
#define MAIN_HANDLER spriteHandlers[0]
#define MAIN_SPRITE sprites[0]

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
	int frameLocation[2];
} AnimationHandler;

int NextFrameLocation(AnimationHandler *handler) {
	if(handler->currFrame >= handler->numFrames - 1)
	    handler->currFrame = -1;
	return handler->frameLocation[++handler->currFrame];
}

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


// Copy of OAM
Sprite sprites[128];

// Sprite Handler array
SpriteHandler spriteHandlers[128];

// Spawn a sprite on the screen
void spawnSprite()
{
}



void setSpriteLoc(SpriteHandler *sprite, int x, int y) {
	if(x >= 0 && x < MAP_PIXEL_X_MAX) {
	    sprite->worldx = x;
	    sprite->x = sprite->worldx - level.x;
	}

	if(y >= 0 && y < MAP_PIXEL_Y_MAX) {
	    sprite->worldy = y;
	    sprite->y = sprite->worldy - level.y;
	}
}

//Checks a single pixel if there is a solid object
bool checkSolidCollision(SpriteHandler *sprite, int x, int y) {
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
	int leftX = nextX + 9;
	int rightX = nextX + sprite->width - 13;
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
	return -1;

}

void move(SpriteHandler *sprite, int x, int y) {
	if(sprite->mode == GROUND) {
		if(!checkSolidCollision(sprite,((sprite->flipped) ? x :
		    	 x + sprite->width - 4), y + 4)) {
	        int newY;
	        newY = checkABSensors(sprite,x,y);
	        if(newY == -1) {
				sprite->mode = AIR;
				sprite->yspd = sprite->yspd + 1;
			}
	     	moveViewport();
			setSpriteLoc(sprite,x,newY);
		}
	} else if(sprite->mode == AIR) {
     	if(checkSolidCollision(sprite,((sprite->flipped) ? x :
		    	 x + sprite->width - 4), y + 4)) {
			x = sprite->worldx;
		}
        int newY =  checkABSensors(sprite,x,y);
		if(newY == -1 || sprite->yspd < 0) { //in air
			moveViewport();
			setSpriteLoc(sprite,x,y);
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
	for(n = 0; n < 128; n++)
	{
		sprites[n].attribute0 = 160;
		sprites[n].attribute1 = 240;
	}

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
	MAIN_HANDLER.worldx = MAIN_HANDLER.x + level.x;
	MAIN_HANDLER.worldy = MAIN_HANDLER.y + level.y;
	MAIN_SPRITE.attribute0 = COLOR_256 | SQUARE | MAIN_HANDLER.x;
	MAIN_SPRITE.attribute1 = SIZE_32 | MAIN_HANDLER.y;
	MAIN_SPRITE.attribute2 = MAIN_HANDLER.standing.frameLocation[0];
}
