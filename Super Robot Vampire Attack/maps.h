#include "BackgroundSource/factoryhitmap.map.c"
#include "BackgroundSource/factoryhitmap.raw.c"
#include "BackgroundSource/factorylevel.map.c"
#include "BackgroundSource/factorylevel.pal.c"
#include "BackgroundSource/factorylevel.raw.c"
#include "BackgroundSource/factorybg.map.c"
#include "BackgroundSource/factorybg.raw.c"
#include "BackgroundSource/outdoorhitmap.map.c"
#include "BackgroundSource/outdoorhitmap.raw.c"
#include "BackgroundSource/outdoorlevel.map.c"
#include "BackgroundSource/outdoorlevel.pal.c"
#include "BackgroundSource/outdoorlevel.raw.c"
#include "BackgroundSource/outdoorlandscape.map.c"
#include "BackgroundSource/outdoorlandscape.raw.c"
#include "BackgroundSource/gameover.raw.c"
#include "BackgroundSource/win.raw.c"
#include "BackgroundSource/loading.raw.c"

//Nuclear bomb image from: brookwoodhistory.wikispaces.com

//Defines
#define FACTORY_TILE_SET_SIZE 9280

const u16 *level_Palette;
const u8 *level_Tiles;
const u8 *hitmap_Tiles;
const u8 *bg_Tiles;

u16* levelMap =(u16*)ScreenBaseBlock(30);
u16* levelBgMap = (u16*)ScreenBaseBlock(15);

int currLevel = 0;

typedef struct GoalBox {
   int lx;
   int ly;
   int rx;
   int ry;
} GoalBox;

// Typedefs
typedef struct BgInfo {
   int x, y;
	int dx, dy;
	int backgroundNextCol, backgroundPrevCol;
	int backgroundNextRow, backgroundPrevRow;
	int xNextCol;
	int xPrevCol;
	int yNextRow;
	int yPrevRow;
	int levelWidth; //Num of 8x8 tiles in one row of map
	int levelHeight; //Num of 8x8 tiles in one column of map
	int pixelXMax; //Max distance the camera can move in x direction
	int pixelYMax; //Max distance the camera can move in y direction
	int botOffset;
	int rightOffset;
	int mapSize; //Num entries in array in a *.map.c
	int tileSize; //Num entries in tile .raw
   const u16* srcMap; //Where values will be copied from
   u16* destMap; //Where values will be copied to
   GoalBox goal; //Where the goal of the level is
} BgInfo;

BgInfo bg, level, hitmap;

//ScreenXCol: the column in our 256x256 background to copy to
//bgColumn: the column from the background map we would like to copy from
void copyColumn(int screenXCol, int screenYOff, int bgYOff, int bgColumn,
	u16* toMap, const u16* fromMap, int mapWidth)
{
	screenXCol = screenXCol % 32;
	int i, bgIndex;
	for(i = 0; i < 32; i++)
	{
      bgIndex = bgColumn+(mapWidth*(i+bgYOff));
      if(bgIndex < level.mapSize) {
   		toMap[screenXCol + ((i+screenYOff)%32) * 32] =
   			fromMap[bgIndex];
      } else {
         toMap[screenXCol + ((i+screenYOff)%32) * 32] =
   			fromMap[2];
      }
	}
}
//ScreenYRow: the row in our 256x256 background to copy to
//bgRow: the row from the background map we would like to copy from
void copyRow(int screenXOff, int screenYRow, int bgXOff, int bgRow,
	u16* toMap, const u16* fromMap, int mapWidth)
{
	int i, bgIndex;
	for(i = 0; i < 32; i++)
	{
      bgIndex = (bgXOff + i) + (mapWidth*bgRow);
      if(bgIndex < level.mapSize) {
         toMap[((i+screenXOff)%32) + screenYRow * 32] =
            fromMap[bgIndex];
      } else {
         toMap[((i+screenXOff)%32) + screenYRow * 32] =
            fromMap[2];
      }
	}
}

void InitMaps() {
   REG_BG0CNT = BG_COLOR256 | TEXTBG_SIZE_256x256 | (30 << SCREEN_SHIFT);
   REG_BG1CNT = BG_COLOR256 | TEXTBG_SIZE_256x256 | (15 << SCREEN_SHIFT)  | (2 << CHAR_SHIFT);

   if(currLevel == 0) {
   	level_Palette = factorylevel_Palette;
   	level_Tiles = factorylevel_Tiles;
      hitmap_Tiles = factoryhitmap_Tiles;
      bg_Tiles = factorybg_Tiles;

    	level.x = 16, level.y = 16;
   	level.dx = 0, level.dy = 0;
   	level.backgroundNextCol = 33, level.backgroundPrevCol = 0;
   	level.backgroundNextRow = 33, level.backgroundPrevRow = 0;
   	level.xNextCol = 1;
   	level.xPrevCol = 0;
   	level.yNextRow = 1;
   	level.yPrevRow = 0;
   	level.levelWidth = 300;
   	level.levelHeight = 100;
   	level.pixelXMax = 2400;
   	level.pixelYMax = 800;
   	level.botOffset = 96;
   	level.rightOffset = 136;
   	level.mapSize = 30000;
   	level.tileSize = 4160;
   	level.srcMap = factorylevel_Map;
   	level.destMap = levelMap;
   	level.goal.lx = 2330;
      level.goal.ly = 736;
      level.goal.rx = 2367;
      level.goal.ry = 767;

   	hitmap.levelWidth = 300;
   	hitmap.levelHeight = 100;
   	hitmap.pixelXMax = 2400;
   	hitmap.pixelYMax = 800;
   	hitmap.mapSize = 30000;
   	hitmap.srcMap = factoryhitmap_Map;

   	bg.x = 16, bg.y = 16;
   	bg.dx = 0, bg.dy = 0;
   	bg.backgroundNextCol = 33, bg.backgroundPrevCol = 0;
   	bg.backgroundNextRow = 33, bg.backgroundPrevRow = 0;
   	bg.xNextCol = 1;
   	bg.xPrevCol = 0;
   	bg.yNextRow = 1;
   	bg.yPrevRow = 0;
   	bg.levelWidth = 150;
   	bg.levelHeight = 50;
   	bg.tileSize = 2112;
      bg.srcMap = factorybg_Map;
      bg.destMap = levelBgMap;
   	//PixelXMax and PixelYMax not important for far background
   } else if( currLevel == 1) {
      level_Palette = outdoorlevel_Palette;
   	level_Tiles = outdoorlevel_Tiles;
   	hitmap_Tiles = outdoorhitmap_Tiles;
   	bg_Tiles = outdoorlandscape_Tiles;

    	level.x = 16, level.y = 16;
   	level.dx = 0, level.dy = 0;
   	level.backgroundNextCol = 33, level.backgroundPrevCol = 0;
   	level.backgroundNextRow = 33, level.backgroundPrevRow = 0;
   	level.xNextCol = 1;
   	level.xPrevCol = 0;
   	level.yNextRow = 1;
   	level.yPrevRow = 0;
   	level.levelWidth = 300;
   	level.levelHeight = 100;
   	level.pixelXMax = 2400;
   	level.pixelYMax = 800;
   	level.botOffset = 96;
   	level.rightOffset = 136;
   	level.mapSize = 30000;
   	level.tileSize = 3776;
   	level.srcMap = outdoorlevel_Map;
   	level.destMap = levelMap;
   	level.goal.lx = 730;
   	level.goal.ly = 160;
   	level.goal.rx = 780;
   	level.goal.ry = 200;

   	hitmap.levelWidth = 300;
   	hitmap.levelHeight = 100;
   	hitmap.pixelXMax = 2400;
   	hitmap.pixelYMax = 800;
   	hitmap.mapSize = 30000;
   	hitmap.srcMap = outdoorhitmap_Map;

   	bg.x = 16, bg.y = 16;
   	bg.dx = 0, bg.dy = 0;
   	bg.backgroundNextCol = 33, bg.backgroundPrevCol = 0;
   	bg.backgroundNextRow = 33, bg.backgroundPrevRow = 0;
   	bg.xNextCol = 1;
   	bg.xPrevCol = 0;
   	bg.yNextRow = 1;
   	bg.yPrevRow = 0;
   	bg.levelWidth = 300;
   	bg.levelHeight = 100;
   	bg.tileSize = 3880;
      bg.srcMap = outdoorlandscape_Map;
      bg.destMap = levelBgMap;
   	//PixelXMax and PixelYMax not important for far background
   }

	REG_BG0VOFS = level.y;
	REG_BG0HOFS = level.x;
	REG_BG1VOFS = bg.y;
	REG_BG1HOFS = bg.x;

	int i;

	for(i = 1; i < 33; i++)
	{
		copyColumn(i, level.yPrevRow + 1, level.backgroundPrevCol + 1, i,
			level.destMap, level.srcMap, level.levelWidth);
	}
	for(i = 1; i < 33; i++)
	{
		copyColumn(i, bg.yPrevRow + 1, bg.backgroundPrevCol + 1, i,
			bg.destMap, bg.srcMap, bg.levelWidth);
	}
}

void moveBackgroundRight(BgInfo *currLevel) {
   int inc = currLevel->dx / 8;
	currLevel->dx = currLevel->dx % 8;
	int i;
	for(i = 0; i < inc; i++) {
		copyColumn(currLevel->xNextCol, currLevel->yPrevRow + 1,
			currLevel->backgroundPrevRow + 1, currLevel->backgroundNextCol,
			currLevel->destMap, currLevel->srcMap, currLevel->levelWidth);
		currLevel->backgroundPrevCol++;
		currLevel->backgroundNextCol++;
		currLevel->xNextCol = (currLevel->xNextCol + 1) % 32;
		currLevel->xPrevCol = (currLevel->xPrevCol + 1) % 32;
	}
}

void moveBackgroundLeft(BgInfo *currLevel) {
   int inc = abs(currLevel->dx / 8);
	currLevel->dx = currLevel->dx % 8;
	int i;
	for(i = 0; i < inc; i++) {
		copyColumn(currLevel->xPrevCol, currLevel->yPrevRow + 1,
			currLevel->backgroundPrevRow + 1, currLevel->backgroundPrevCol,
			currLevel->destMap, currLevel->srcMap, currLevel->levelWidth);
		currLevel->backgroundNextCol--;
		currLevel->backgroundPrevCol--;
		if(currLevel->xPrevCol <= 0)
			currLevel->xPrevCol = 31;
		else
		    currLevel->xPrevCol--;

		if(currLevel->xNextCol <= 0)
		    currLevel->xNextCol = 31;
		else
		    currLevel->xNextCol--;
	}
}

void moveBackgroundDown(BgInfo *currLevel) {
   int i;
	int inc = currLevel->dy / 8;
	currLevel->dy = currLevel->dy % 8;
   for(i = 0; i < inc; i++) {
   	copyRow(currLevel->xPrevCol + 1,currLevel->yNextRow,
   		currLevel->backgroundPrevCol + 1, currLevel->backgroundNextRow,
   	   currLevel->destMap, currLevel->srcMap, currLevel->levelWidth);
   	currLevel->backgroundPrevRow++;
   	currLevel->backgroundNextRow++;
   	currLevel->yNextRow = (currLevel->yNextRow + 1) % 32;
   	currLevel->yPrevRow = (currLevel->yPrevRow + 1) % 32;
   }
}

void moveBackgroundUp(BgInfo *currLevel) {
   int i;
	int inc = abs(currLevel->dy / 8);
	currLevel->dy = currLevel->dy % 8;
	for(i = 0; i < inc; i++) {
		copyRow(currLevel->xPrevCol + 1,currLevel->yPrevRow,
			currLevel->backgroundPrevCol + 1, currLevel->backgroundPrevRow,
			currLevel->destMap, currLevel->srcMap, currLevel->levelWidth);
		currLevel->backgroundNextRow--;
		currLevel->backgroundPrevRow--;
		if(currLevel->yPrevRow <= 0)
			currLevel->yPrevRow = 31;
		else
		    currLevel->yPrevRow--;

		if(currLevel->yNextRow <= 0)
		    currLevel->yNextRow = 31;
		else
		    currLevel->yNextRow--;
	}
}
