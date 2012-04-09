#include "backgroundSource/factoryhitmap.map.c"
#include "backgroundSource/factoryhitmap.raw.c"
#include "backgroundSource/factorylevel.map.c"
#include "backgroundSource/factorylevel.pal.c"
#include "backgroundSource/factorylevel.raw.c"
//Defines
#define FACTORY_TILE_SET_SIZE 7232
#define FACTORY_PALETTE_SIZE 256

#define GROUND 0
#define AIR 1

#define LEVEL_WIDTH 300
#define LEVEL_HEIGHT 100

#define MAP_PIXEL_X_MAX 2400
#define MAP_PIXEL_Y_MAX 800
#define LEVEL_WIDTH 300
#define LEVEL_HEIGHT 100
#define LANDSCAPE_WIDTH 30
#define FACTORY_MAP_SIZE 30000

//extern const u16 levellandscape_Map[];
extern const u16 factoryhitmap_Map[];
extern const u16 factorylevel_Map[];
extern const u16 factorylevel_Palette[];
extern const u8 factorylevel_Tiles[];

const u16 *levelhitmap_Map;
const u16 *level_Map;
const u16 *level_Palette;
const u8 *level_Tiles;

u16* levelMap =(u16*)ScreenBaseBlock(30);
u16* levelHitMap = (u16*)ScreenBaseBlock(20);
u16* levelLandscapeMap = (u16*)ScreenBaseBlock(15);

// Typedefs
typedef struct BgInfo {
   int x, y ;
	int dx, dy;
	int backgroundNextCol, backgroundPrevCol;
	int backgroundNextRow, backgroundPrevRow;
	int xNextCol;
	int xPrevCol;
	int yNextRow;
	int yPrevRow;
} BgInfo;

BgInfo bg, level;

//ScreenXCol: the column in our 256x256 background to copy to
//bgColumn: the column from the background map we would like to copy from
void copyColumn(int screenXCol, int screenYOff, int bgYOff, int bgColumn,
	u16* toMap, const u16* fromMap, int mapWidth)
{
	screenXCol = screenXCol % 32;
	int i;
	for(i = 0; i < 32; i++)
	{
		toMap[screenXCol + ((i+screenYOff)%32) * 32] =
			fromMap[bgColumn+(mapWidth*(i+bgYOff))];
	}
}
//ScreenYRow: the row in our 256x256 background to copy to
//bgRow: the row from the background map we would like to copy from
void copyRow(int screenXOff, int screenYRow, int bgXOff, int bgRow,
	u16* toMap, const u16* fromMap, int mapWidth)
{
	int i;
	for(i = 0; i < 32; i++)
	{
		toMap[((i+screenXOff)%32) + screenYRow * 32] =
			fromMap[(bgXOff + i) + (mapWidth*bgRow)];
	}
}

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
