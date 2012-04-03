
//Defines
#define FACTORY_TILE_SET_SIZE 7232
#define FACTORY_PALETTE_SIZE 256


#define GROUND 0
#define AIR 1

#define LEVEL_WIDTH 300
#define LEVEL_HEIGHT 100



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
