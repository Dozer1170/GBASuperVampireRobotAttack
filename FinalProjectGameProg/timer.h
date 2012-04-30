#ifndef timer
//Sound Header File

//Control Registers
//#define REG_TM0CNT     *(volatile unsigned short*)0x4000102
//#define REG_TM1CNT     *(volatile unsigned short*)0x4000106
//#define REG_TM2CNT     *(volatile unsigned short*)0x400010A
//#define REG_TM3CNT     *(volatile unsigned short*)0x400010E

//Data Registers
#define REG_TM0D       *(volatile unsigned short*)0x4000100
//#define REG_TM1D       *(volatile unsigned short*)0x4000104
//#define REG_TM2D       *(volatile unsigned short*)0x4000108
//#define REG_TM3D       *(volatile unsigned short*)0x400010C

//Used to set Timers
#define TIMER_FREQUENCY_SYSTEM 	0x0
#define TIMER_FREQUENCY_64 		0x1
#define TIMER_FREQUENCY_256 	0x2
#define TIMER_FREQUENCY_1024 	0x3
#define TIMER_OVERFLOW 			0x4
#define TIMER_ENABLE 			0x80

#endif

//Set Custom Interrupt
void SetInterupt()
{
    REG_IME = 0x00;
    REG_INTERRUPT = (unsigned int)MyHandler;
    REG_IE |= INT_VBLANK;
    REG_DISPSTAT |= 0x08;
    REG_IME = 0x01;
}
