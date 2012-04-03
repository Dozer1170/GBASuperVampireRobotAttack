#include "BackgroundSource/factoryhitmap.map.c"
#include "BackgroundSource/factoryhitmap.raw.c"

#include "BackgroundSource/factorylevel.map.c"
#include "BackgroundSource/factorylevel.raw.c"

#include "BackgroundSource/factorylevel.pal.c"

// Variables
const u16 *level_Palette;
const u8 *level_Tiles;

const u16 *levelhitmap_Map;
const u16 *level_Map;

BgInfo bg, level;


u16* levelMap =(u16*)ScreenBaseBlock(30);
u16* levelHitMap = (u16*)ScreenBaseBlock(20);
u16* levelLandscapeMap = (u16*)ScreenBaseBlock(15);


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



// Initializes the maps
void InitMaps() {
    REG_BG0CNT = BG_COLOR256 | TEXTBG_SIZE_256x256 | (30 << SCREEN_SHIFT);
    REG_BG1CNT = BG_COLOR256 | TEXTBG_SIZE_256x256 | (15 << SCREEN_SHIFT);
	REG_BG2CNT = BG_COLOR256 | TEXTBG_SIZE_256x256 | (20 << SCREEN_SHIFT);

	levelhitmap_Map = factoryhitmap_Map;
	level_Map = factorylevel_Map;
	level_Palette = factorylevel_Palette;
	level_Tiles = factorylevel_Tiles;

 	level.x = 16, level.y = 16;
	level.dx = 0, level.dy = 0;
	level.backgroundNextCol = 33, level.backgroundPrevCol = 0;
	level.backgroundNextRow = 33, level.backgroundPrevRow = 0;
	level.xNextCol = 1;
	level.xPrevCol = 0;
	level.yNextRow = 1;
	level.yPrevRow = 0;
	
	REG_BG0VOFS = level.y;
	REG_BG0HOFS = level.x;
	REG_BG1VOFS = 0;
	REG_BG1HOFS = 0;

	int i;

	for(i = 1; i < 33; i++)
	{
		copyColumn(i, level.yPrevRow + 1, level.backgroundPrevCol + 1, i,
			levelHitMap, levelhitmap_Map, LEVEL_WIDTH);
	}
	for(i = 1; i < 33; i++)
	{
		copyColumn(i, level.yPrevRow + 1, level.backgroundPrevCol + 1, i,
			levelMap, level_Map, LEVEL_WIDTH);
	}
/*	for(i = 0; i < 32; i++)
	{
		copyColumn(i, 0, 0, i,
			levelLandscapeMap, levellandscape_Map, LANDSCAPE_WIDTH);
	}*/
}



// Initializes the sprites
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
	MAIN_HANDLER.maxGspd = 4;
	MAIN_HANDLER.acc = .25;
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
