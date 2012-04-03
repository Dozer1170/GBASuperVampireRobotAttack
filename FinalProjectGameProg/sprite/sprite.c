// Function prototypes
bool checkSolidCollision(int x, int y);
bool checkSolidCollisionSet(SpriteHandler *sprite, int x, int y);
int checkABSensors(SpriteHandler *sprite, int nextX, int nextY);
void moveViewport();

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


void move(SpriteHandler *sprite, int x, int y) {
	if(sprite->mode == GROUND) {
		if(!checkSolidCollision(((sprite->flipped) ? x :
		    	 x + sprite->width - 4), y + 4)) {
	        int newY;
	        newY = checkABSensors(sprite,x,y);
	     	moveViewport();
			setSpriteLoc(sprite,x,newY);
		}
	} else if(sprite->mode == AIR) {
     	if(checkSolidCollision(((sprite->flipped) ? x :
		    	 x + sprite->width - 4), y + 4)) {
			x = sprite->worldx;
		}
        int newY =  checkABSensors(sprite,x,y);
		if(newY == y || sprite->yspd < 0) { //in air
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
