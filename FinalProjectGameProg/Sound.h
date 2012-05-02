//FinalProjectGroupAwesome
//Chris McLaughlin
//Header file for sound


//define some interrupt registers
#define REG_IME 		*(unsigned short*)0x4000208
#define REG_IE 			*(unsigned short*)0x4000200
#define REG_IF 			*(unsigned short*)0x4000202
#define REG_INTERRUPT 	*(unsigned int*) 0x3007FFC
#define REG_DISPSTAT 	*(unsigned short*)0x4000004
#define INT_VBLANK 		0x0001

//define some timer and DMA registers/values
#define REG_DMA1SAD 	*(volatile unsigned int*)0x40000BC
#define REG_DMA1DAD 	*(volatile unsigned int*)0x40000C0
#define REG_DMA1CNT_H 	*(volatile unsigned short*)0x40000C6
#define TIMER_ENABLE 	0x80
#define DMA_DEST_FIXED 	0x40 //64
#define DMA_REPEAT 		0x200 //512
#define DMA_32_NEW 		1024
#define DMA_ENABLE_NEW 	32768
#define DMA_TIMING_SYNC_TO_DISPLAY 0x1000 | 0x2000 //4096 8192

//define some sound hardware registers/values
#define REG_SGCNT0_H 			*(volatile unsigned short*)0x4000082
#define REG_SGCNT1 				*(volatile unsigned short*)0x4000084
#define DSOUND_A_RIGHT_CHANNEL 	0x100 //256
#define DSOUND_A_LEFT_CHANNEL 	0x200 //512
#define DSOUND_A_FIFO_RESET 	0x800 //2048
#define SOUND_MASTER_ENABLE 	0x80 //128

#define SND_ENABLED 			0x00000080
#define SND_OUTPUT_RATIO_25 	0x0000
#define SND_OUTPUT_RATIO_50 	0x0001
#define SND_OUTPUT_RATIO_100 	0x0002
#define DSA_OUTPUT_RATIO_50 	0x0000
#define DSA_OUTPUT_RATIO_100 	0x0004
#define DSA_OUTPUT_TO_RIGHT 	0x0100
#define DSA_OUTPUT_TO_LEFT 		0x0200
#define DSA_OUTPUT_TO_BOTH 		0x0300
#define DSA_TIMER0 				0x0000
#define DSA_TIMER1 				0x0400
#define DSA_FIFO_RESET 			0x0800
#define DSB_OUTPUT_RATIO_50 	0x0000
#define DSB_OUTPUT_RATIO_100 	0x0008
#define DSB_OUTPUT_TO_RIGHT 	0x1000
#define DSB_OUTPUT_TO_LEFT 		0x2000
#define DSB_OUTPUT_TO_BOTH 		0x3000
#define DSB_TIMER0 				0x0000
#define DSB_TIMER1 				0x4000
#define DSB_FIFO_RESET 			0x8000

short SamplePosition = 0;
unsigned short SampleLength = 0;
bool missileFire = false;

//structure to keep track of sounds
typedef struct tagSoung
{
    void* pBuffer;
    unsigned short samplerate;
    unsigned int length;
}sound;

//playSound
//call this to play a sound
void PlaySound(sound *theSound)
{
    unsigned short samples;
    
    //output to both channels and reset the FIFO
    REG_SGCNT0_H = DSOUND_A_RIGHT_CHANNEL | DSOUND_A_LEFT_CHANNEL| DSOUND_A_FIFO_RESET;
    
    //enable all sound
    REG_SGCNT1 = SOUND_MASTER_ENABLE;
    
    //DMA1 source address
    REG_DMA1SAD = (unsigned int)theSound->pBuffer;
    
    //DMA1 destination address
    REG_DMA1DAD = 0x40000A0;
    
    //write 32 bits into destination every vblank
    REG_DMA1CNT_H = DMA_DEST_FIXED | DMA_REPEAT | DMA_32_NEW|
    	DMA_TIMING_SYNC_TO_DISPLAY | DMA_ENABLE_NEW;
    
    //set the sample rate
    samples = 16777216 / theSound->samplerate;
    REG_TM0D = 65536 - samples;
    
    //keep track of the playback position and length
    SampleLength = theSound->length / samples * 15.57;
    SamplePosition = 0;
    
    //enable the timer
    REG_TM0CNT = TIMER_ENABLE;
}

void MyHandler(void)
{
    unsigned short Int_Flag;
    //disable interrupts
    REG_IME = 0x00;
    
    //backup the interrupt flags
    Int_Flag = REG_IF;
    
    //look for vertical refresh
    if((REG_IF & INT_VBLANK) == INT_VBLANK)
    {
        //is a sample currently playing?
        if (SampleLength)
        {
            SamplePosition++;
            if ( (SamplePosition > SampleLength) || SamplePosition == -1)
            {
                //stop playback: disable the timer and DMA
                REG_TM0CNT = 0;
                REG_DMA1CNT_H = 0;
                //reset length
                SampleLength = 0;

				    missileFire = false;
            }
        }
    }
    //restore the interrupt flags
    REG_IF = Int_Flag;
    
    //enable interrupts
    REG_IME = 0x01;
}
