// Defines
#define MAIN_HANDLER spriteHandlers[0]
#define MAIN_SPRITE sprites[0]

#define MAP_PIXEL_X_MAX 2160
#define MAP_PIXEL_Y_MAX 740

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

