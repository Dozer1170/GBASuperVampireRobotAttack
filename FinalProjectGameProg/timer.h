#ifndef timer
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
