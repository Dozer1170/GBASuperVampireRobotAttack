#define NULL 0

#define REG_DISPCNT *(unsigned long*) 0x4000000
#define REG_VCOUNT *(volatile unsigned short*)0x04000006

#define MODE_3 0x3
#define BG0_ENABLE 0x100
#define BG1_ENABLE 0x200
#define BG2_ENABLE 0x400

#define REG_BG0HOFS *(volatile unsigned short*)0x4000010
#define REG_BG0VOFS *(volatile unsigned short*)0x4000012
#define REG_BG1HOFS *(volatile unsigned short*)0x4000014
#define REG_BG1VOFS *(volatile unsigned short*)0x4000016
#define REG_BG2HOFS *(volatile unsigned short*)0x4000018
#define REG_BG2VOFS *(volatile unsigned short*)0x400001A
#define REG_BG3HOFS *(volatile unsigned short*)0x400001C
#define REG_BG3VOFS *(volatile unsigned short*)0x400001E
#define REG_BG0CNT *(volatile unsigned short*)0x4000008
#define REG_BG1CNT *(volatile unsigned short*)0x400000A
#define REG_BG2CNT *(volatile unsigned short*)0x400000C
#define REG_BG3CNT *(volatile unsigned short*)0x400000E

#define BG_COLOR256 0x80
#define CHAR_SHIFT 2
#define SCREEN_SHIFT 8
#define WRAPAROUND 0x1

//Palette for Backgrounds
#define BGPaletteMem ((unsigned short*)0x5000000)

//background tile bitmap sizes
#define TEXTBG_SIZE_256x256 0x0
#define TEXTBG_SIZE_256x512	0x8000
#define TEXTBG_SIZE_512x256	0x4000
#define TEXTBG_SIZE_512x512	0xC000

//background memory offset macros
#define CharBaseBlock(n) (((n)*0x4000)+0x6000000)
#define ScreenBaseBlock(n) (((n)*0x800)+0x6000000)

#define REG_DMA3SAD *(volatile unsigned int*)0x40000D4
#define REG_DMA3DAD *(volatile unsigned int*)0x40000D8
#define REG_DMA3CNT *(volatile unsigned int*)0x40000DC
#define DMA_ENABLE 0x80000000
#define DMA_TIMING_IMMEDIATE 0x00000000
#define DMA_16 0x00000000
#define DMA_32 0x04000000
#define DMA_32NOW (DMA_ENABLE | DMA_TIMING_IMMEDIATE | DMA_32)
#define DMA_16NOW (DMA_ENABLE | DMA_TIMING_IMMEDIATE | DMA_16)

#define SetMode(mode) REG_DISPCNT = (mode)

#define RGB(r,g,b) (unsigned short)((r)+((g)<<5)+((b)<<10))

#define FREE_MODE 0
#define LINE_MODE 1
#define BOX_MODE 2
#define CIRCLE_MODE 3

#define WHITE 0
#define RED 1
#define YELLOW 2
#define GREEN 3
#define AQUA 4
#define BLUE 5
#define PURPLE 6

#define BLACK_RGB RGB(0,0,0)
#define GREY_RGB RGB(15,15,15)
#define WHITE_RGB RGB(31,31,31)
#define RED_RGB RGB(31,0,0)
#define YELLOW_RGB RGB(31,31,0)
#define GREEN_RGB RGB(0,31,0)
#define AQUA_RGB RGB(0,31,31)
#define BLUE_RGB RGB(0,0,31)
#define PURPLE_RGB RGB(31,0,31)

#define HEIGHT 160
#define WIDTH 240

#define KEY_A        0x0001
#define KEY_B        0x0002
#define KEY_SELECT   0x0004
#define KEY_START    0x0008
#define KEY_RIGHT    0x0010
#define KEY_LEFT     0x0020
#define KEY_UP       0x0040
#define KEY_DOWN     0x0080
#define KEY_R        0x0100
#define KEY_L        0x0200

#define KEY_MASK     0x03FF

#define bool int
#define true 1
#define false 0

//OAM
#define SpriteMem ((unsigned short*)0x7000000)
//Sprite Image data
#define SpriteData ((unsigned short*) 0x6010000)
//Sprite Palette
#define SpritePal ((unsigned short*) 0x5000200)
//other constants
#define OBJ_MAP_2D 0x0
#define OBJ_MAP_1D 0x40
#define OBJ_ENABLE 0x1000
//attribute0 stuff
#define ROTATION_FLAG 		0x100
#define SIZE_DOUBLE			0x200
#define AFFINE_DOUBLE       0x300
#define MODE_NORMAL     	0x0
#define MODE_TRANSPARENT    0x400
#define MODE_WINDOWED		0x800
#define MOSAIC				0x1000
#define COLOR_16			0x0000
#define COLOR_256			0x2000
#define SQUARE              0x0
#define TALL                0x8000
#define WIDE                0x4000

//attribute1 stuff
#define ROTDATA(n)          ((n) << 9)
#define HORIZONTAL_FLIP		0x1000
#define VERTICAL_FLIP		0x2000
#define SIZE_8              0x0
#define SIZE_16             0x4000
#define SIZE_32             0x8000
#define SIZE_64             0xC000

//attribute2 stuff
#define PRIORITY(n)		((n) << 10)
#define PALETTE(n)		((n) << 12)

#define SET_X(n,x)      (((n) & 0xFE00) | x)
#define SET_Y(n,y)      (((n) & 0xFF00) | y)
#define FLIP_LEFT(n)    ((n) | HORIZONTAL_FLIP)
#define FLIP_RIGHT(n)   ((n) & 0xCFFF)

#define ALIGN4 __attribute__((aligned(4)))

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef short s16;



void DMAFastCopy(void* source, void* dest, u32 count,
    u32 mode)
{
    if (mode == DMA_16NOW || mode == DMA_32NOW)
    {
    	REG_DMA3SAD = (u32)source;
        REG_DMA3DAD = (u32)dest;
        REG_DMA3CNT = count | mode;
    }
}



// Wait for the vertical refresh
void WaitVBlank(void)
{
	while(REG_VCOUNT >= 160);
	while(REG_VCOUNT < 160);
}


// Check the status of button pushes
inline void ButtonPoll()
{
	__key_prev = __key_curr;
	__key_curr = ~*BUTTONS & KEY_MASK;
}

