/***********************************************************************************************************************
PicoMite MMBasic

mmc_stm32.c

<COPYRIGHT HOLDERS>  Geoff Graham, Peter Mather
Copyright (c) 2021, <COPYRIGHT HOLDERS> All rights reserved. 
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: 
1.	Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2.	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
    in the documentation and/or other materials provided with the distribution.
3.	The name MMBasic be used when referring to the interpreter in any documentation and promotional material and the original copyright message be displayed 
    on the console at startup (additional copyright messages may be added).
4.	All advertising materials mentioning features or use of this software must display the following acknowledgement: This product includes software developed 
    by the <copyright holder>.
5.	Neither the name of the <copyright holder> nor the names of its contributors may be used to endorse or promote products derived from this software 
    without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDERS> AS IS AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDERS> BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 

************************************************************************************************************************/


/*------------------------------------------------------------------------/
/  MMCv3/SDv1/SDv2 (in SPI mode) control module
/-------------------------------------------------------------------------/
/
/  Copyright (C) 2014, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------*/

#include "Hardware_Includes.h"
#include <stddef.h>
#include "diskio.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
//#include "integer.h"
int SPISpeed=0xFF;
//#define SD_CS_PIN Option.SD_CS
//#define SPI_MOSI_PIN Option.SYSTEM_MOSI
//#define SPI_CLK_PIN Option.SYSTEM_CLK
//define SPI_MISO_PIN Option.SYSTEM_MISO
int SPI_CLK_PIN,SPI_MOSI_PIN,SPI_MISO_PIN;
int SD_CLK_PIN,SD_MOSI_PIN,SD_MISO_PIN, SD_CS_PIN;
#define CD	MDD_SDSPI_CardDetectState()	/* Card detected   (yes:true, no:false, default:true) */
#define WP	MDD_SDSPI_WriteProtectState()		/* Write protected (yes:true, no:false, default:false) */
/* SPI bit rate controls */
int SD_SPI_SPEED=SD_SLOW_SPI_SPEED;
//#define	FCLK_SLOW()		{SD_SPI_SPEED=SD_SLOW_SPI_SPEED;}	/* Set slow clock (100k-400k) */
//#define	FCLK_FAST()		{SD_SPI_SPEED=SD_FAST_SPI_SPEED;}/* Set fast clock (depends on the CSD) */
extern volatile BYTE SDCardStat;
volatile int diskcheckrate=0;
const uint8_t high[512]={[0 ... 511]=0xFF};
int CurrentSPISpeed=NONE_SPI_SPEED;
#define slow_clock 200000
#define SPI_FAST 18000000
int AUDIO_L_PIN, AUDIO_R_PIN, AUDIO_SLICE;
uint16_t AUDIO_WRAP=0;
BYTE (*xchg_byte)(BYTE data_out)= NULL;
void (*xmit_byte_multi)(const BYTE *buff, int cnt)= NULL;
void (*rcvr_byte_multi)(BYTE *buff, int cnt)= NULL;
int (*SET_SPI_CLK)(int speed, int polarity, int edge)=NULL;
const int mapping[101]={0,4,11,18,25,33,41,49,57,66,75,84,93,103,113,123,134,145,156,167,179,191,203,216,228,241,255,268,
282,296,311,325,340,355,371,387,403,419,436,453,470,487,505,523,541,560,578,597,617,636,656,676,
697,718,738,760,781,803,825,847,870,893,916,940,963,987,1012,1036,1061,1086,1111,1137,1163,1189,1216,
1242,1269,1297,1324,1352,1380,1408,1437,1466,1495,1525,1554,1584,1615,1645,1676,1707,1739,1770,1802,1834,1867,1900,1933,1966,2000};
int I2C0locked=0;
int I2C1locked=0;
int SPI0locked=0;
int SPI1locked=0;
int BacklightSlice=-1;
int BacklightChannel=-1;
/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/

/* Definitions for MMC/SDC command */
#define CMD0   (0)			/* GO_IDLE_STATE */
#define CMD1   (1)			/* SEND_OP_COND */
#define ACMD41 (41|0x80)	/* SEND_OP_COND (SDC) */
#define CMD6   (6)			/* SEND_IF_COND */
#define CMD8   (8)			/* SEND_IF_COND */
#define CMD9   (9)			/* SEND_CSD */
#define CMD10  (10)			/* SEND_CID */
#define CMD12  (12)			/* STOP_TRANSMISSION */
#define ACMD13 (13|0x80)	/* SD_STATUS (SDC) */
#define CMD16  (16)			/* SET_BLOCKLEN */
#define CMD17  (17)			/* READ_SINGLE_BLOCK */
#define CMD18  (18)			/* READ_MULTIPLE_BLOCK */
#define CMD23  (23)			/* SET_BLOCK_COUNT */
#define ACMD23 (23|0x80)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24  (24)			/* WRITE_BLOCK */
#define CMD25  (25)			/* WRITE_MULTIPLE_BLOCK */
#define CMD41  (41)			/* SEND_OP_COND (ACMD) */
#define CMD55  (55)			/* APP_CMD */
#define CMD58  (58)			/* READ_OCR */
unsigned char __not_in_flash_func(CRC7)(const unsigned char message[], const unsigned int length) {
  const unsigned char poly = 0b10001001;
  unsigned char crc = 0;
  for (unsigned i = 0; i < length; i++) {
     crc ^= message[i];
     for (int j = 0; j < 8; j++) {
      crc = (crc & 0x80u) ? ((crc << 1) ^ (poly << 1)) : (crc << 1);
    }
  }
  return crc >> 1;
}

static
UINT CardType;
BYTE MDD_SDSPI_CardDetectState(void){
            return 1;
}
BYTE MDD_SDSPI_WriteProtectState(void)
{
	return 0;
}
void on_pwm_wrap() {
	static int repeatcount=1;
    // play a tone
    pwm_clear_irq(AUDIO_SLICE);
    if(CurrentlyPlaying == P_TONE){
        if(!SoundPlay){
        	CloseAudio(1);
            WAVcomplete = true;
        } else {
        	SoundPlay--;
			pwm_set_both_levels(AUDIO_SLICE,
			(((((SineTable[(int)PhaseAC_left]-2000)  * mapping[vol_left]) / 2000)+2000)*AUDIO_WRAP)>>12,
			(((((SineTable[(int)PhaseAC_right]-2000)  * mapping[vol_right]) / 2000)+2000)*AUDIO_WRAP)>>12);
        	PhaseAC_left = PhaseAC_left + PhaseM_left;
        	PhaseAC_right = PhaseAC_right + PhaseM_right;
        	if(PhaseAC_left>=4096.0)PhaseAC_left-=4096.0;
        	if(PhaseAC_right>=4096.0)PhaseAC_right-=4096.0;
        }
    } else if(CurrentlyPlaying == P_WAV) {
		if(--repeatcount)return;
		repeatcount=audiorepeat;
        if(bcount[1]==0 && bcount[2]==0 && playreadcomplete==1){
//        	HAL_TIM_Base_Stop_IT(&htim4);
    		pwm_set_irq_enabled(AUDIO_SLICE, false);
        }
        if(swingbuf){ //buffer is primed
        	if(swingbuf==1)playbuff=(uint16_t *)sbuff1;
        	else playbuff=(uint16_t *)sbuff2;
        	if(CurrentlyPlaying == P_WAV && mono){
				pwm_set_both_levels(AUDIO_SLICE,playbuff[ppos],playbuff[ppos]);
				ppos++;
        	} else {
				if(ppos<bcount[swingbuf]){
				pwm_set_both_levels(AUDIO_SLICE,playbuff[ppos],playbuff[ppos+1]);
				ppos+=2;
				}
        	}
        	if(ppos==bcount[swingbuf]){
        		int psave=ppos;
        		bcount[swingbuf]=0;
        		ppos=0;
        		if(swingbuf==1)swingbuf=2;
        		else swingbuf=1;
        		if(bcount[swingbuf]==0 && !playreadcomplete){ //nothing ready yet so flip back
            		if(swingbuf==1){
            			swingbuf=2;
            			nextbuf=1;
            		}
            		else {
            			swingbuf=1;
            			nextbuf=2;
            		}
            		bcount[swingbuf]=psave;
            		ppos=0;
        		}
        	}
        }
    } else if(CurrentlyPlaying == P_SOUND) {
    	static int noisedwellleft[MAXSOUNDS]={0}, noisedwellright[MAXSOUNDS]={0};
    	static uint32_t noiseleft[MAXSOUNDS]={0}, noiseright[MAXSOUNDS]={0};
    	int i,j;
    	int leftv=0, rightv=0;
    	for(i=0;i<MAXSOUNDS;i++){ //first update the 8 sound pointers
    		if(sound_mode_left[i]!=nulltable){
				sound_PhaseAC_left[i] = sound_PhaseAC_left[i] + sound_PhaseM_left[i];
				if(sound_PhaseAC_left[i]>=4096.0)sound_PhaseAC_left[i]-=4096.0;
				j = (int)sound_mode_left[i][(int)sound_PhaseAC_left[i]];
				j= (j-2000)*mapping[sound_v_left[i]]/2000;
				leftv+=j;
    		}
    		if(sound_mode_right[i]!=nulltable){
				sound_PhaseAC_right[i] = sound_PhaseAC_right[i] + sound_PhaseM_right[i];
				if(sound_PhaseAC_right[i]>=4096.0)sound_PhaseAC_right[i]-=4096.0;
				j = (int)sound_mode_right[i][(int)sound_PhaseAC_right[i]];
				j= (j-2000)*mapping[sound_v_right[i]]/2000;
				rightv += j;
   			}
    	}
		leftv+=2000;
		rightv+=2000;
		leftv=(leftv*AUDIO_WRAP)>>12;
		rightv=(rightv*AUDIO_WRAP)>>12;
		pwm_set_both_levels(AUDIO_SLICE,leftv,rightv);
//    	HAL_DAC_SetValue(&hdac1,DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint16_t)leftv);
//    	HAL_DAC_SetValue(&hdac1,DAC_CHANNEL_2, DAC_ALIGN_12B_R, (uint16_t)rightv);
    } else {
        // play must be paused
		pwm_set_both_levels(AUDIO_SLICE,AUDIO_WRAP>>1,AUDIO_WRAP>>1);
    }
}

void __not_in_flash_func(BitBangSendSPI)(const BYTE *buff, int cnt){
	int i, SPICount;
	BYTE SPIData;
    if(SD_SPI_SPEED==SD_SLOW_SPI_SPEED){
    	for(i=0;i<cnt;i++){
    		SPIData=buff[i];
    		for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock out the Address byte
    		{
    			if (SPIData & 0x80)                                 // Check for a 1
    				gpio_put(SD_MOSI_PIN,GPIO_PIN_SET);
    			else
    				gpio_put(SD_MOSI_PIN,GPIO_PIN_RESET);
    			busy_wait_us_32(20);
    			gpio_put(SD_CLK_PIN,GPIO_PIN_SET);
    			busy_wait_us_32(20);
    			gpio_put(SD_CLK_PIN,GPIO_PIN_RESET);
    			SPIData <<= 1;                                      // Rotate to get the next bit
    		}  // and loop back to send the next bit
    	}
    } else if(Option.CPU_Speed<=slow_clock){
    	for(i=0;i<cnt;i++){
    		SPIData=buff[i];
    		for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock out the Address byte
    		{
    			if (SPIData & 0x80)                                 // Check for a 1
    				gpio_put(SD_MOSI_PIN,GPIO_PIN_SET);
    			else
    				gpio_put(SD_MOSI_PIN,GPIO_PIN_RESET);
				asm("NOP");
    			gpio_put(SD_CLK_PIN,GPIO_PIN_SET);
    			SPIData <<= 1;                                      // Rotate to get the next bit
    			gpio_put(SD_CLK_PIN,GPIO_PIN_RESET);
    		}  // and loop back to send the next bit
    	}
	} else {
    	for(i=0;i<cnt;i++){
    		SPIData=buff[i];
    		for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock out the Address byte
    		{
    			if (SPIData & 0x80)                                 // Check for a 1
    				gpio_put(SD_MOSI_PIN,GPIO_PIN_SET);
    			else
    				gpio_put(SD_MOSI_PIN,GPIO_PIN_RESET);
				asm("NOP");asm("NOP");asm("NOP");
    			gpio_put(SD_CLK_PIN,GPIO_PIN_SET);
    			SPIData <<= 1;   asm("NOP");                                   // Rotate to get the next bit
    			gpio_put(SD_CLK_PIN,GPIO_PIN_RESET);
    		}  // and loop back to send the next bit
    	}
	}
}
void __not_in_flash_func(BitBangReadSPI)(BYTE *buff, int cnt){
	int i, SPICount;
	BYTE SPIData;
	gpio_put(SD_CLK_PIN,GPIO_PIN_RESET);
    if(SD_SPI_SPEED==SD_SLOW_SPI_SPEED){
    	for(i=0;i<cnt;i++){
    		SPIData = 0;
    		for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock in the data to be fread
    		{
    			SPIData <<=1;                                       // Rotate the data
    			gpio_put(SD_CLK_PIN,GPIO_PIN_SET);
    			busy_wait_us_32(20);
    			SPIData += gpio_get(SD_MISO_PIN);                       // Read the data bit
    			gpio_put(SD_CLK_PIN,GPIO_PIN_RESET);
    			busy_wait_us_32(20);
    		}                                                     // and loop back
    		buff[i]=SPIData;
    	}
    } else if(Option.CPU_Speed<=slow_clock){
    	for(i=0;i<cnt;i++){
    		SPIData = 0;
    		for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock in the data to be fread
    		{
    			SPIData <<=1;                                       // Rotate the data
    			gpio_put(SD_CLK_PIN,GPIO_PIN_SET);
				asm("NOP");
    			SPIData += gpio_get(SD_MISO_PIN);                       // Read the data bit
   				gpio_put(SD_CLK_PIN,GPIO_PIN_RESET);
				asm("NOP");
     		}                                                     // and loop back
    		buff[i]=SPIData;
    	}
    } else {
    	for(i=0;i<cnt;i++){
    		SPIData = 0;
    		for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock in the data to be fread
    		{
    			SPIData <<=1;                                       // Rotate the data
    			gpio_put(SD_CLK_PIN,GPIO_PIN_SET);
				asm("NOP");asm("NOP");asm("NOP");
    			SPIData += gpio_get(SD_MISO_PIN);                       // Read the data bit
   				gpio_put(SD_CLK_PIN,GPIO_PIN_RESET);
				asm("NOP");asm("NOP");asm("NOP");
     		}                                                     // and loop back
    		buff[i]=SPIData;
    	}
    }
}

BYTE __not_in_flash_func(BitBangSwapSPI)(BYTE data_out){
	BYTE data_in=0;
	int SPICount;
	if(SD_SPI_SPEED==SD_SLOW_SPI_SPEED){
		for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock in the data to be fread
		{
			if (data_out & 0x80)                                 // Check for a 1
				gpio_put(SD_MOSI_PIN,GPIO_PIN_SET);
			else
				gpio_put(SD_MOSI_PIN,GPIO_PIN_RESET);
	    	busy_wait_us_32(20);
	    	data_in <<=1;                                       // Rotate the data
	    	gpio_put(SD_CLK_PIN,GPIO_PIN_SET);
	    	busy_wait_us_32(20);
	    	data_in += gpio_get(SD_MISO_PIN);                       // Read the data bit
	    	gpio_put(SD_CLK_PIN,GPIO_PIN_RESET);
	    	data_out <<= 1;
		}                                                     // and loop back
    } else if(Option.CPU_Speed<=slow_clock){
		for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock in the data to be fread
		{
			if (data_out & 0x80)                                 // Check for a 1
				gpio_put(SD_MOSI_PIN,GPIO_PIN_SET);
			else
				gpio_put(SD_MOSI_PIN,GPIO_PIN_RESET);
			asm("NOP");
	    	data_in <<=1;                                       // Rotate the data
	    	gpio_put(SD_CLK_PIN,GPIO_PIN_SET);
	    	data_out <<= 1;
	    	data_in += gpio_get(SD_MISO_PIN);                       // Read the data bit
	    	gpio_put(SD_CLK_PIN,GPIO_PIN_RESET);
		}                                                     // and loop back
	} else {
		for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock in the data to be fread
		{
			if (data_out & 0x80)                                 // Check for a 1
				gpio_put(SD_MOSI_PIN,GPIO_PIN_SET);
			else
				gpio_put(SD_MOSI_PIN,GPIO_PIN_RESET);
			asm("NOP");asm("NOP");
	    	data_in <<=1;                                       // Rotate the data
	    	gpio_put(SD_CLK_PIN,GPIO_PIN_SET);
	    	data_out <<= 1;asm("NOP");
	    	data_in += gpio_get(SD_MISO_PIN);                       // Read the data bit
	    	gpio_put(SD_CLK_PIN,GPIO_PIN_RESET);
		}                                                     // and loop back
	}
	return data_in;
}
int BitBangSetClk(int speed, int polarity, int edge){
	gpio_init(SD_CLK_PIN);
	gpio_put(SD_CLK_PIN,GPIO_PIN_RESET);
	gpio_set_dir(SD_CLK_PIN, GPIO_OUT);
	gpio_init(SD_MOSI_PIN);
	gpio_put(SD_MOSI_PIN,GPIO_PIN_RESET);
	gpio_set_dir(SD_MOSI_PIN, GPIO_OUT);
	gpio_init(SD_MISO_PIN);
	gpio_set_pulls(SD_MISO_PIN,true,false);
	gpio_set_dir(SD_MISO_PIN, GPIO_IN);
	gpio_set_drive_strength(SD_MOSI_PIN,GPIO_DRIVE_STRENGTH_12MA);
	gpio_set_drive_strength(SD_CLK_PIN,GPIO_DRIVE_STRENGTH_12MA);
	gpio_set_input_hysteresis_enabled(SD_MISO_PIN,true);
	SD_SPI_SPEED=speed;
}
BYTE __not_in_flash_func(HW0SwapSPI)(BYTE data_out){
	BYTE data_in=0;
	spi_write_read_blocking(spi0,&data_out,&data_in,1);
	return data_in;
}
void __not_in_flash_func(HW0SendSPI)(const BYTE *buff, int cnt){
	spi_write_blocking(spi0,buff,cnt);
}
void __not_in_flash_func(HW0ReadSPI)(BYTE *buff, int cnt){
	spi_read_blocking(spi0,0xff,buff,cnt);

}
int HW0Clk(int speed, int polarity, int edge){
	xchg_byte= HW0SwapSPI;
	xmit_byte_multi=HW0SendSPI;
	rcvr_byte_multi=HW0ReadSPI;
	gpio_set_function(SPI_CLK_PIN, GPIO_FUNC_SPI);
	gpio_set_function(SPI_MOSI_PIN, GPIO_FUNC_SPI);
	gpio_set_function(SPI_MISO_PIN, GPIO_FUNC_SPI);
	spi_init(spi0, speed);
	spi_set_format(spi0, 8, polarity,edge, SPI_MSB_FIRST);
	gpio_set_drive_strength(SPI_MOSI_PIN,GPIO_DRIVE_STRENGTH_12MA);
	gpio_set_drive_strength(SPI_CLK_PIN,GPIO_DRIVE_STRENGTH_12MA);
	gpio_set_input_hysteresis_enabled(SPI_MISO_PIN,true);
	return speed;
}
BYTE __not_in_flash_func(HW1SwapSPI)(BYTE data_out){
	BYTE data_in=0;
	spi_write_read_blocking(spi1,&data_out,&data_in,1);
	return data_in;
}
void __not_in_flash_func(HW1SendSPI)(const BYTE *buff, int cnt){
	spi_write_blocking(spi1,buff,cnt);
}
void __not_in_flash_func(HW1ReadSPI)(BYTE *buff, int cnt){
	spi_read_blocking(spi1,0xff,buff,cnt);
}
int HW1Clk(int speed, int polarity, int edge){
	xchg_byte= HW1SwapSPI;
	xmit_byte_multi=HW1SendSPI;
	rcvr_byte_multi=HW1ReadSPI;
	gpio_set_function(SPI_CLK_PIN, GPIO_FUNC_SPI);
	gpio_set_function(SPI_MOSI_PIN, GPIO_FUNC_SPI);
	gpio_set_function(SPI_MISO_PIN, GPIO_FUNC_SPI);
	spi_init(spi1, speed);
	spi_set_format(spi1, 8, polarity,edge, SPI_MSB_FIRST);
	gpio_set_drive_strength(SPI_MOSI_PIN,GPIO_DRIVE_STRENGTH_12MA);
	gpio_set_drive_strength(SPI_CLK_PIN,GPIO_DRIVE_STRENGTH_12MA);
	gpio_set_input_hysteresis_enabled(SPI_MISO_PIN,true);
	return speed;
}


/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/

static
int wait_ready (void)
{
	BYTE d;

	Timer2 = 500;	/* Wait for ready in timeout of 500ms */
	do{
		d = xchg_byte(0xFF);
        busy_wait_us_32(5);
	} while ((d != 0xFF) && Timer2);

	return (d == 0xFF) ? 1 : 0;
}



/*-----------------------------------------------------------------------*/
/* Deselect the card and release SPI bus                                 */
/*-----------------------------------------------------------------------*/

static
void __not_in_flash_func(deselect)(void)
{
//	asm("NOP");asm("NOP");//asm("NOP");
	gpio_put(SD_CS_PIN,GPIO_PIN_SET);
//	asm("NOP");//asm("NOP");
	xchg_byte(0xFF);		/* Dummy clock (force DO hi-z for multiple slave SPI) */
}



/*-----------------------------------------------------------------------*/
/* Select the card and wait ready                                        */
/*-----------------------------------------------------------------------*/


int __not_in_flash_func(selectSD)(void)	/* 1:Successful, 0:Timeout */
{
	if(SD_SPI_SPEED==SD_SLOW_SPI_SPEED)	SPISpeedSet(SDSLOW);
	else SPISpeedSet(SDFAST);
	gpio_put(SD_CS_PIN,GPIO_PIN_RESET);
    busy_wait_us_32(5);
    xchg_byte(0xFF);		/* Dummy clock (force DO enabled) */

	if (wait_ready()) return 1;	/* Wait for card ready */

	deselect();
	return 0;	/* Timeout */
}



/*-----------------------------------------------------------------------*/
/* Receive a data packet from MMC                                        */
/*-----------------------------------------------------------------------*/


int __not_in_flash_func(rcvr_datablock)(	/* 1:OK, 0:Failed */
	BYTE *buff,			/* Data buffer to store received data */
	UINT btr			/* Byte count (must be multiple of 4) */
)
{
	BYTE token;


	Timer1 = 100;
	do {							/* Wait for data packet in timeout of 100ms */
		token = xchg_byte(0xFF);
	} while ((token == 0xFF) && Timer1);

	if(token != 0xFE) return 0;		/* If not valid data token, retutn with error */

	rcvr_byte_multi(buff, btr);		/* Receive the data block into buffer */
	xchg_byte(0xFF);					/* Discard CRC */
	xchg_byte(0xFF);
	diskcheckrate=1; //successful read so reset check
	return 1;						/* Return with success */
}



/*-----------------------------------------------------------------------*/
/* Send a data packet to MMC                                             */
/*-----------------------------------------------------------------------*/



int __not_in_flash_func(xmit_datablock)(	/* 1:OK, 0:Failed */
	const BYTE *buff,	/* 512 byte data block to be transmitted */
	BYTE token			/* Data token */
)
{
	BYTE resp;


	if (!wait_ready()) return 0;

	xchg_byte(token);		/* Xmit a token */
	if (token != 0xFD) {	/* Not StopTran token */
		xmit_byte_multi(buff, 512);	/* Xmit the data block to the MMC */
		xchg_byte(0xFF);				/* CRC (Dummy) */
		xchg_byte(0xFF);
		resp = xchg_byte(0xFF);		/* Receive a data response */
		if ((resp & 0x1F) != 0x05)	/* If not accepted, return with error */
			return 0;
	}
	diskcheckrate=1; //successful write so reset check

	return 1;
}


/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/
BYTE __not_in_flash_func(send_cmd)(
	BYTE cmd,		/* Command byte */
	DWORD arg		/* Argument */
)
{
	BYTE n, res;

	if (cmd & 0x80) {	/* ACMD<n> is the command sequense of CMD55-CMD<n> */
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) return res;
	}

	/* Select the card and wait for ready except to stop multiple block read */
	if (cmd != CMD12) {
		deselect();
		if (!selectSD()){
            return 0xFF;
        }
	}
	/* Send command packet */
		BYTE command[6];
		command[0]=(0x40 | cmd);			// Start + Command index
		command[1]=(arg >> 24);	// Argument[31..24]
		command[2]=(arg >> 16);	// Argument[23..16]
		command[3]=(arg >> 8);		// Argument[15..8]
		command[4]=(arg);			// Argument[7..0]
		command[5]=(CRC7(command, 5)<<1) | 1;
		xchg_byte(command[0]);			/* Start + Command index */
		xchg_byte(command[1]);	/* Argument[31..24] */
		xchg_byte(command[2]);	/* Argument[23..16] */
		xchg_byte(command[3]);		/* Argument[15..8] */
		xchg_byte(command[4]);			/* Argument[7..0] */
		xchg_byte(command[5]);
	/* Receive command response */
	if (cmd == CMD12) xchg_byte(0xFF);	/* Skip a stuff byte on stop to read */
	n = 100;							/* Wait for a valid response in timeout of 10 attempts */
	do
		res = xchg_byte(0xFF);
	while ((res & 0x80) && --n);

	return res;			/* Return with the response value */
}



/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0) */
)
{
	if (pdrv != 0) return STA_NOINIT;	/* Supports only single drive */

	return SDCardStat;
}

/*int CMD0send(void){
    char response,trys=100, responsetrys=10;
    do {
    	deselect();
		gpio_put(SD_CS_PIN,GPIO_PIN_RESET);
		asm("NOP");asm("NOP");asm("NOP");
        trys--;
        xchg_byte(0x40);
        xchg_byte(0x0);
        xchg_byte(0x0);
        xchg_byte(0x0);
        xchg_byte(0x0);
        xchg_byte(0x95);
        do{
            response=xchg_byte(0xFF);
            responsetrys--;
        } while((responsetrys !=0) && (response !=1));
    } while((trys !=0) && (response !=1));
    return response;
}*/

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv		/* Physical drive nmuber (0) */
)
{
	BYTE n, cmd, ty, ocr[4];


	if (pdrv != 0) return STA_NOINIT;	/* Supports only single drive */
//	if (SDCardStat & STA_NODISK) return SDCardStat;	/* No card in the socket */
	SD_SPI_SPEED=SD_SLOW_SPI_SPEED;
	SPISpeedSet(SDSLOW);
	deselect();							/* Initialize memory card interface */
	for (n = 10; n; n--) xchg_byte(0xFF);	/* 80 dummy clocks */
	ty = 0;

	if (send_cmd(CMD0, 0) == 1) {			/* Enter Idle state */
		Timer1 = 1000;						/* Initialization timeout of 1000 msec */
		if (send_cmd(CMD8, 0x1AA) == 1) {	/* SDv2? */
//			MMPrintString("sdv2\r\n");
			for (n = 0; n < 4; n++) ocr[n] = xchg_byte(0xFF);			/* Get trailing return value of R7 resp */

			if (ocr[2] == 0x01 && ocr[3] == 0xAA) {				/* The card can work at vdd range of 2.7-3.6V */
				while (Timer1 && send_cmd(ACMD41, 0x40000000));	/* Wait for leaving idle state (ACMD41 with HCS bit) */
				if (Timer1 && send_cmd(CMD58, 0) == 0) {			/* Check CCS bit in the OCR */
					for (n = 0; n < 4; n++) ocr[n] = xchg_byte(0xFF);
					ty = (ocr[0] & 0x40) ? CT_SD2|CT_BLOCK : CT_SD2;	/* SDv2 */
				}
			}
		} else {							/* SDv1 or MMCv3 */
//			MMPrintString("sdv1\r\n");
			if (send_cmd(ACMD41, 0) <= 1) 	{
				ty = CT_SD1; cmd = ACMD41;	/* SDv1 */
			} else {
				ty = CT_MMC; cmd = CMD1;	/* MMCv3 */
			}
			while (Timer1 && send_cmd(cmd, 0));		/* Wait for leaving idle state */
			if (!Timer1 || send_cmd(CMD16, 512) != 0)	/* Set read/write block length to 512 */
				ty = 0;
		}
	}
	CardType = ty;
	deselect();
	if (ty) {		/* Function succeded */
		SDCardStat &= ~STA_NOINIT;	/* Clear STA_NOINIT */
		SD_SPI_SPEED=Option.SDspeed*1000000;
		SPISpeedSet(SDFAST);
//		SET_SPI_CLK(SD_SPI_SPEED, false, false);
	}

	return SDCardStat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT __not_in_flash_func(disk_read)(BYTE pdrv, BYTE* buff, LBA_t sector, UINT count)
{
	if (pdrv || !count) return RES_PARERR;
	if (SDCardStat & STA_NOINIT) return RES_NOTRDY;

	if (!(CardType & CT_BLOCK)) sector *= 512;	/* Convert to byte address if needed */

	if (count == 1) {		/* Single block read */
		if ((send_cmd(CMD17, sector) == 0)	/* READ_SINGLE_BLOCK */
			&& rcvr_datablock(buff, 512))
			count = 0;
	}
	else {				/* Multiple block read */
		if (send_cmd(CMD18, sector) == 0) {	/* READ_MULTIPLE_BLOCK */
			do {
				if (!rcvr_datablock(buff, 512)) break;
				buff += 512;
			} while (--count);
			send_cmd(CMD12, 0);				/* STOP_TRANSMISSION */
		}
	}
	deselect();

	return count ? RES_ERROR : RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/


DRESULT __not_in_flash_func(disk_write)(BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count)
{
	if (pdrv || !count) return RES_PARERR;
	if (SDCardStat & STA_NOINIT) return RES_NOTRDY;
	if (SDCardStat & STA_PROTECT) return RES_WRPRT;

	if (!(CardType & CT_BLOCK)) sector *= 512;	/* Convert to byte address if needed */

	if (count == 1) {		/* Single block write */
		if ((send_cmd(CMD24, sector) == 0)	/* WRITE_BLOCK */
			&& xmit_datablock(buff, 0xFE))
			count = 0;
	}
	else {				/* Multiple block write */
		if (CardType & CT_SDC) send_cmd(ACMD23, count);
		if (send_cmd(CMD25, sector) == 0) {	/* WRITE_MULTIPLE_BLOCK */
			do {
				if (!xmit_datablock(buff, 0xFC)) break;
				buff += 512;
			} while (--count);
			if (!xmit_datablock(0, 0xFD))	/* STOP_TRAN token */
				count = 1;
		}
	}
	deselect();

	return count ? RES_ERROR : RES_OK;
}




/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/


DRESULT __not_in_flash_func(disk_ioctl)(
	BYTE pdrv,		/* Physical drive nmuber (0) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive data block */
)
{
	DRESULT res;
	BYTE n, csd[16], *ptr = buff;
	DWORD csz;

	if (pdrv) return RES_PARERR;
	if (SDCardStat & STA_NOINIT) return RES_NOTRDY;

	res = RES_ERROR;
	switch (cmd) {
	case CTRL_SYNC :	/* Flush write-back cache, Wait for end of internal process */
		if (selectSD()) res = RES_OK;
		break;

	case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (WORD) */
		if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {
			if ((csd[0] >> 6) == 1) {	/* SDv2? */
				csz = csd[9] + ((WORD)csd[8] << 8) + ((DWORD)(csd[7] & 63) << 16) + 1;
				*(DWORD*)buff = csz << 10;
			} else {					/* SDv1 or MMCv3 */
				n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
				csz = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
				*(DWORD*)buff = csz << (n - 9);
			}
			res = RES_OK;
		}
		break;

	case GET_BLOCK_SIZE :	/* Get erase block size in unit of sectors (DWORD) */
		if (CardType & CT_SD2) {	/* SDv2? */
			if (send_cmd(ACMD13, 0) == 0) {		/* Read SD status */
				xchg_byte(0xFF);
				if (rcvr_datablock(csd, 16)) {				/* Read partial block */
					for (n = 64 - 16; n; n--) xchg_byte(0xFF);	/* Purge trailing data */
					*(DWORD*)buff = 16UL << (csd[10] >> 4);
					res = RES_OK;
				}
			}
		} else {					/* SDv1 or MMCv3 */
			if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) {	/* Read CSD */
				if (CardType & CT_SD1) {	/* SDv1 */
					*(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
				} else {					/* MMCv3 */
					*(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
				}
				res = RES_OK;
			}
		}
		break;

	case MMC_GET_TYPE :		/* Get card type flags (1 byte) */
		*ptr = CardType;
		res = RES_OK;
		break;

	case MMC_GET_CSD :	/* Receive CSD as a data block (16 bytes) */
		if ((send_cmd(CMD9, 0) == 0)	/* READ_CSD */
			&& rcvr_datablock(buff, 16))
			res = RES_OK;
		break;

	case MMC_GET_CID :	/* Receive CID as a data block (16 bytes) */
		if ((send_cmd(CMD10, 0) == 0)	/* READ_CID */
			&& rcvr_datablock(buff, 16))
			res = RES_OK;
		break;

	case MMC_GET_OCR :	/* Receive OCR as an R3 resp (4 bytes) */
		if (send_cmd(CMD58, 0) == 0) {	/* READ_OCR */
			for (n = 0; n < 4; n++)
				*((BYTE*)buff+n) = xchg_byte(0xFF);
			res = RES_OK;
		}
		break;

	case MMC_GET_SDSTAT :	/* Receive SD statsu as a data block (64 bytes) */
		if ((CardType & CT_SD2) && send_cmd(ACMD13, 0) == 0) {	/* SD_STATUS */
			xchg_byte(0xFF);
			if (rcvr_datablock(buff, 64))
				res = RES_OK;
		}
		break;

	case CTRL_POWER :	/* Power off */
		SDCardStat |= STA_NOINIT;
		res = RES_OK;
		break;

	default:
		res = RES_PARERR;
	}

	deselect();

	return res;
}




/*-----------------------------------------------------------------------*/
/* Device Timer Driven Procedure                                         */
/*-----------------------------------------------------------------------*/
/* This function must be called by timer interrupt in period of 1ms      */

void __not_in_flash_func(disk_timerproc)(void)
{
	BYTE s;
	UINT n;


	n = Timer1;					/* 1000Hz decrement timer with zero stopped */
	if (n) Timer1 = --n;
	n = Timer2;
	if (n) Timer2 = --n;


	/* Update socket status */

	s = SDCardStat;

	if (WP) s |= STA_PROTECT;
	else	s &= ~STA_PROTECT;

	if (CD) s &= ~STA_NODISK;
	else	s |= (STA_NODISK | STA_NOINIT);

	SDCardStat = s;
}

DWORD __not_in_flash_func(get_fattime)(void){
    DWORD i;
    i = ((year-1980) & 0x7F)<<25;
    i |= (month & 0xF)<<21;
    i |= (day & 0x1F)<<16;
    i |= (hour & 0x1F)<<11;
    i |= (minute & 0x3F)<<5;
    i |= (second/2 & 0x1F);
    return i;
}
void InitReservedIO(void) {
#ifdef PICOMITEVGA
		ExtCfg(21, EXT_BOOT_RESERVED, 0);
		ExtCfg(22, EXT_BOOT_RESERVED, 0);
		ExtCfg(24, EXT_BOOT_RESERVED, 0);
		ExtCfg(25, EXT_BOOT_RESERVED, 0);
		ExtCfg(26, EXT_BOOT_RESERVED, 0);
		ExtCfg(27, EXT_BOOT_RESERVED, 0);
#else
	if(Option.DISPLAY_TYPE>=SSDPANEL){
		ExtCfg(SSD1963_DC_PIN, EXT_BOOT_RESERVED, 0);gpio_init(SSD1963_DC_GPPIN);gpio_put(SSD1963_DC_GPPIN,GPIO_PIN_SET);gpio_set_dir(SSD1963_DC_GPPIN, GPIO_OUT);
		ExtCfg(SSD1963_RESET_PIN, EXT_BOOT_RESERVED, 0);gpio_init(SSD1963_RESET_GPPIN);gpio_put(SSD1963_RESET_GPPIN,GPIO_PIN_SET);gpio_set_dir(SSD1963_RESET_GPPIN, GPIO_OUT);
		ExtCfg(SSD1963_WR_PIN, EXT_BOOT_RESERVED, 0);gpio_init(SSD1963_WR_GPPIN);gpio_put(SSD1963_WR_GPPIN,GPIO_PIN_SET);gpio_set_dir(SSD1963_WR_GPPIN, GPIO_OUT);
		ExtCfg(SSD1963_RD_PIN, EXT_BOOT_RESERVED, 0);gpio_init(SSD1963_RD_GPPIN);gpio_put(SSD1963_RD_GPPIN,GPIO_PIN_SET);gpio_set_dir(SSD1963_RD_GPPIN, GPIO_OUT);
		ExtCfg(SSD1963_DAT1, EXT_BOOT_RESERVED, 0);gpio_init(SSD1963_GPDAT1);gpio_put(SSD1963_GPDAT1,GPIO_PIN_SET);gpio_set_dir(SSD1963_GPDAT1, GPIO_OUT);
		ExtCfg(SSD1963_DAT2, EXT_BOOT_RESERVED, 0);gpio_init(SSD1963_GPDAT2);gpio_put(SSD1963_GPDAT2,GPIO_PIN_SET);gpio_set_dir(SSD1963_GPDAT2, GPIO_OUT);
		ExtCfg(SSD1963_DAT3, EXT_BOOT_RESERVED, 0);gpio_init(SSD1963_GPDAT3);gpio_put(SSD1963_GPDAT3,GPIO_PIN_SET);gpio_set_dir(SSD1963_GPDAT3, GPIO_OUT);
		ExtCfg(SSD1963_DAT4, EXT_BOOT_RESERVED, 0);gpio_init(SSD1963_GPDAT4);gpio_put(SSD1963_GPDAT4,GPIO_PIN_SET);gpio_set_dir(SSD1963_GPDAT4, GPIO_OUT);
		ExtCfg(SSD1963_DAT5, EXT_BOOT_RESERVED, 0);gpio_init(SSD1963_GPDAT5);gpio_put(SSD1963_GPDAT5,GPIO_PIN_SET);gpio_set_dir(SSD1963_GPDAT5, GPIO_OUT);
		ExtCfg(SSD1963_DAT6, EXT_BOOT_RESERVED, 0);gpio_init(SSD1963_GPDAT6);gpio_put(SSD1963_GPDAT6,GPIO_PIN_SET);gpio_set_dir(SSD1963_GPDAT6, GPIO_OUT);
		ExtCfg(SSD1963_DAT7, EXT_BOOT_RESERVED, 0);gpio_init(SSD1963_GPDAT7);gpio_put(SSD1963_GPDAT7,GPIO_PIN_SET);gpio_set_dir(SSD1963_GPDAT7, GPIO_OUT);
		ExtCfg(SSD1963_DAT8, EXT_BOOT_RESERVED, 0);gpio_init(SSD1963_GPDAT8);gpio_put(SSD1963_GPDAT8,GPIO_PIN_SET);gpio_set_dir(SSD1963_GPDAT8, GPIO_OUT);
	}
	if(Option.LCD_CD){
		ExtCfg(Option.LCD_CD, EXT_BOOT_RESERVED, 0);
		ExtCfg(Option.LCD_CS, EXT_BOOT_RESERVED, 0);
		ExtCfg(Option.LCD_Reset, EXT_BOOT_RESERVED, 0);
		LCD_CD_PIN=PinDef[Option.LCD_CD].GPno;
		LCD_CS_PIN=PinDef[Option.LCD_CS].GPno;
		LCD_Reset_PIN=PinDef[Option.LCD_Reset].GPno;
		gpio_init(LCD_CD_PIN);
		gpio_put(LCD_CD_PIN,GPIO_PIN_SET);
		gpio_set_dir(LCD_CD_PIN, GPIO_OUT);
		gpio_init(LCD_CS_PIN);
		gpio_set_drive_strength(LCD_CS_PIN,GPIO_DRIVE_STRENGTH_12MA);
		gpio_put(LCD_CS_PIN,Option.DISPLAY_TYPE!=ST7920 ? GPIO_PIN_SET : GPIO_PIN_RESET);
		gpio_set_dir(LCD_CS_PIN, GPIO_OUT);
		gpio_init(LCD_Reset_PIN);
		gpio_put(LCD_Reset_PIN,GPIO_PIN_RESET);
		gpio_set_dir(LCD_Reset_PIN, GPIO_OUT);
		if(Option.E_INKbusy) {
			ExtCfg(Option.E_INKbusy, EXT_BOOT_RESERVED, 0);
			LCD_E_INKbusy=PinDef[Option.E_INKbusy].GPno;
			gpio_init(LCD_E_INKbusy);
			gpio_pull_up(LCD_E_INKbusy);
			gpio_set_dir(LCD_E_INKbusy, GPIO_IN);
		}
		CurrentSPISpeed=NONE_SPI_SPEED;
		if(Option.DISPLAY_BL){
			ExtCfg(Option.DISPLAY_BL, EXT_BOOT_RESERVED, 0);
			int pin=Option.DISPLAY_BL,value, slice;
			if(PinDef[pin].mode & PWM0A){PWM0Apin=pin;slice=0;}
			else if(PinDef[pin].mode & PWM0B){PWM0Bpin=pin;slice=0;}
			else if(PinDef[pin].mode & PWM1A){PWM1Apin=pin;slice=1;}
			else if(PinDef[pin].mode & PWM1B){PWM1Bpin=pin;slice=1;}
			else if(PinDef[pin].mode & PWM2A){PWM2Apin=pin;slice=2;}
			else if(PinDef[pin].mode & PWM2B){PWM2Bpin=pin;slice=2;}
			else if(PinDef[pin].mode & PWM3A){PWM3Apin=pin;slice=3;}
			else if(PinDef[pin].mode & PWM3B){PWM3Bpin=pin;slice=3;}
			else if(PinDef[pin].mode & PWM4A){PWM4Apin=pin;slice=4;}
			else if(PinDef[pin].mode & PWM4B){PWM4Bpin=pin;slice=4;}
			else if(PinDef[pin].mode & PWM5A){PWM5Apin=pin;slice=5;}
			else if(PinDef[pin].mode & PWM5B){PWM5Bpin=pin;slice=5;}
			else if(PinDef[pin].mode & PWM6A){PWM6Apin=pin;slice=6;}
			else if(PinDef[pin].mode & PWM6B){PWM6Bpin=pin;slice=6;}
			else if(PinDef[pin].mode & PWM7A){PWM7Apin=pin;slice=7;}
			else if(PinDef[pin].mode & PWM7B){PWM7Bpin=pin;slice=7;}
    		gpio_init(PinDef[pin].GPno); 
			gpio_set_function(PinDef[pin].GPno, GPIO_FUNC_PWM);
			MMFLOAT frequency=1000.0,duty=Option.DefaultBrightness;
			int wrap=(Option.CPU_Speed*1000)/frequency;
			int high=(int)((MMFLOAT)Option.CPU_Speed/frequency*duty*10.0);
			int div=1;
			while(wrap>65535){
				wrap>>=1;
				if(duty>=0.0)high>>=1;
				div<<=1;
			}
			wrap--;
			if(div!=1)pwm_set_clkdiv(slice,(float)div);
			pwm_set_wrap(slice, wrap);
			BacklightSlice=slice;
			if(slice==0 && PWM0Apin!=99){
				pwm_set_chan_level(slice, PWM_CHAN_A, high);
				BacklightChannel=PWM_CHAN_A;
			}
			if(slice==0 && PWM0Bpin!=99){
				pwm_set_chan_level(slice, PWM_CHAN_B, high);
				BacklightChannel=PWM_CHAN_B;
			}
			if(slice==1 && PWM1Apin!=99){
				pwm_set_chan_level(slice, PWM_CHAN_A, high);
				BacklightChannel=PWM_CHAN_A;
			}
			if(slice==1 && PWM1Bpin!=99){
				pwm_set_chan_level(slice, PWM_CHAN_B, high);
				BacklightChannel=PWM_CHAN_B;
			}
			if(slice==2 && PWM2Apin!=99){
				pwm_set_chan_level(slice, PWM_CHAN_A, high);
				BacklightChannel=PWM_CHAN_A;
			}
			if(slice==2 && PWM2Bpin!=99){
				pwm_set_chan_level(slice, PWM_CHAN_B, high);
				BacklightChannel=PWM_CHAN_B;
			}
			if(slice==3 && PWM3Apin!=99){
				pwm_set_chan_level(slice, PWM_CHAN_A, high);
				BacklightChannel=PWM_CHAN_A;
			}
			if(slice==3 && PWM3Bpin!=99){
				pwm_set_chan_level(slice, PWM_CHAN_B, high);
				BacklightChannel=PWM_CHAN_B;
			}
			if(slice==4 && PWM4Apin!=99){
				pwm_set_chan_level(slice, PWM_CHAN_A, high);
				BacklightChannel=PWM_CHAN_A;
			}
			if(slice==4 && PWM4Bpin!=99){
				pwm_set_chan_level(slice, PWM_CHAN_B, high);
				BacklightChannel=PWM_CHAN_B;
			}
			if(slice==5 && PWM5Apin!=99){
				pwm_set_chan_level(slice, PWM_CHAN_A, high);
				BacklightChannel=PWM_CHAN_A;
			}
			if(slice==5 && PWM5Bpin!=99){
				pwm_set_chan_level(slice, PWM_CHAN_B, high);
				BacklightChannel=PWM_CHAN_B;
			}
			if(slice==6 && PWM6Apin!=99){
				pwm_set_chan_level(slice, PWM_CHAN_A, high);
				BacklightChannel=PWM_CHAN_A;
			}
			if(slice==6 && PWM6Bpin!=99){
				pwm_set_chan_level(slice, PWM_CHAN_B, high);
				BacklightChannel=PWM_CHAN_B;
			}
			if(slice==7 && PWM7Apin!=99){
				pwm_set_chan_level(slice, PWM_CHAN_A, high);
				BacklightChannel=PWM_CHAN_A;
			}
			if(slice==7 && PWM7Bpin!=99){
				pwm_set_chan_level(slice, PWM_CHAN_B, high);
				BacklightChannel=PWM_CHAN_B;
			}
			if(slice==0){
				pwm_set_enabled(slice, true);
			}
			if(slice==1){
				pwm_set_enabled(slice, true);
			}
			if(slice==2){
				pwm_set_enabled(slice, true);
			}
			if(slice==3){
				pwm_set_enabled(slice, true);
			}
			if(slice==4){
				pwm_set_enabled(slice, true);
			}
			if(slice==5){
				pwm_set_enabled(slice, true);
			}
			if(slice==6){
				pwm_set_enabled(slice, true);
			}
			if(slice==7){
				pwm_set_enabled(slice, true);
			}
		}
	}
	if(Option.TOUCH_CS){
		ExtCfg(Option.TOUCH_CS, EXT_BOOT_RESERVED, 0);
		ExtCfg(Option.TOUCH_IRQ, EXT_BOOT_RESERVED, 0);
		TOUCH_CS_PIN=PinDef[Option.TOUCH_CS].GPno;
		TOUCH_IRQ_PIN=PinDef[Option.TOUCH_IRQ].GPno;
		gpio_init(TOUCH_CS_PIN);
		gpio_set_drive_strength(TOUCH_CS_PIN,GPIO_DRIVE_STRENGTH_12MA);
		gpio_put(TOUCH_CS_PIN,GPIO_PIN_SET);
		gpio_set_dir(TOUCH_CS_PIN, GPIO_OUT);
		gpio_set_slew_rate(TOUCH_CS_PIN, GPIO_SLEW_RATE_SLOW);
		gpio_init(TOUCH_IRQ_PIN);
		gpio_pull_up(TOUCH_IRQ_PIN);
		gpio_set_dir(TOUCH_IRQ_PIN, GPIO_IN);
		gpio_set_input_hysteresis_enabled(TOUCH_IRQ_PIN,true);
		if(Option.TOUCH_Click){
			ExtCfg(Option.TOUCH_Click, EXT_BOOT_RESERVED, 0);
			TOUCH_Click_PIN=PinDef[Option.TOUCH_Click].GPno;
			gpio_init(TOUCH_Click_PIN);
			gpio_put(TOUCH_Click_PIN,GPIO_PIN_RESET);
			gpio_set_dir(TOUCH_Click_PIN, GPIO_OUT);
		}
	}
#endif
	if(Option.SYSTEM_I2C_SDA){
		ExtCfg(Option.SYSTEM_I2C_SCL, EXT_BOOT_RESERVED, 0);
		ExtCfg(Option.SYSTEM_I2C_SDA, EXT_BOOT_RESERVED, 0);
		gpio_set_function(PinDef[Option.SYSTEM_I2C_SCL].GPno, GPIO_FUNC_I2C);
		gpio_set_function(PinDef[Option.SYSTEM_I2C_SDA].GPno, GPIO_FUNC_I2C);
		if(PinDef[Option.SYSTEM_I2C_SDA].mode & I2C0SDA){
			I2C0locked=1;
			i2c_init(i2c0, 100000);
			gpio_pull_up(PinDef[Option.SYSTEM_I2C_SCL].GPno);
			gpio_pull_up(PinDef[Option.SYSTEM_I2C_SDA].GPno);
			I2C_enabled=1;
			I2C0SDApin=Option.SYSTEM_I2C_SDA;
			I2C0SCLpin=Option.SYSTEM_I2C_SCL;
			I2C_Timeout=100;
		} else {
			I2C1locked=1;
			i2c_init(i2c1, 100000);
			gpio_pull_up(PinDef[Option.SYSTEM_I2C_SCL].GPno);
			gpio_pull_up(PinDef[Option.SYSTEM_I2C_SDA].GPno);
			I2C2_enabled=1;	
			I2C1SDApin=Option.SYSTEM_I2C_SDA;
			I2C1SCLpin=Option.SYSTEM_I2C_SCL;
			I2C2_Timeout=100;
		}
		if(Option.RTC)RtcGetTime(1);
	}
	if(Option.SYSTEM_CLK){
		SPI_CLK_PIN=PinDef[Option.SYSTEM_CLK].GPno;
		SPI_MOSI_PIN=PinDef[Option.SYSTEM_MOSI].GPno;
		SPI_MISO_PIN=PinDef[Option.SYSTEM_MISO].GPno;
		ExtCfg(Option.SYSTEM_CLK, EXT_BOOT_RESERVED, 0);
		ExtCfg(Option.SYSTEM_MOSI, EXT_BOOT_RESERVED, 0);
		ExtCfg(Option.SYSTEM_MISO, EXT_BOOT_RESERVED, 0);
		if(PinDef[Option.SYSTEM_CLK].mode & SPI0SCK && PinDef[Option.SYSTEM_MOSI].mode & SPI0TX  && PinDef[Option.SYSTEM_MISO].mode & SPI0RX  ){
			SET_SPI_CLK=HW0Clk;
			SPI0locked=1;
		} else if(PinDef[Option.SYSTEM_CLK].mode & SPI1SCK && PinDef[Option.SYSTEM_MOSI].mode & SPI1TX  && PinDef[Option.SYSTEM_MISO].mode & SPI1RX  ){
			SET_SPI_CLK=HW1Clk;
			SPI1locked=1;
		} else {
			SET_SPI_CLK=BitBangSetClk; 
		}
		gpio_init(SPI_CLK_PIN);
		gpio_set_drive_strength(SD_CLK_PIN,GPIO_DRIVE_STRENGTH_12MA);
		gpio_put(SPI_CLK_PIN,GPIO_PIN_RESET);
		gpio_set_dir(SPI_CLK_PIN, GPIO_OUT);
		gpio_set_slew_rate(SPI_CLK_PIN, GPIO_SLEW_RATE_FAST);
		gpio_init(SPI_MOSI_PIN);
		gpio_set_drive_strength(SD_MOSI_PIN,GPIO_DRIVE_STRENGTH_12MA);
		gpio_put(SPI_MOSI_PIN,GPIO_PIN_RESET);
		gpio_set_dir(SPI_MOSI_PIN, GPIO_OUT);
		gpio_set_slew_rate(SPI_MOSI_PIN, GPIO_SLEW_RATE_FAST);
		gpio_init(SPI_MISO_PIN);
		gpio_set_pulls(SPI_MISO_PIN,true,false);
		gpio_set_dir(SPI_MISO_PIN, GPIO_IN);
		gpio_set_input_hysteresis_enabled(SPI_MISO_PIN,true);
		xchg_byte= BitBangSwapSPI;
		xmit_byte_multi=BitBangSendSPI;
		rcvr_byte_multi=BitBangReadSPI;
	}
	if(Option.SD_CS){
		ExtCfg(Option.SD_CS, EXT_BOOT_RESERVED, 0);
		SD_CS_PIN=PinDef[Option.SD_CS].GPno;
		gpio_init(SD_CS_PIN);
		gpio_set_drive_strength(SD_CS_PIN,GPIO_DRIVE_STRENGTH_12MA);
		gpio_put(SD_CS_PIN,GPIO_PIN_SET);
		gpio_set_dir(SD_CS_PIN, GPIO_OUT);
		gpio_set_slew_rate(SD_CS_PIN, GPIO_SLEW_RATE_SLOW);
		CurrentSPISpeed=NONE_SPI_SPEED;
		if(Option.SD_CLK_PIN){
			SD_CLK_PIN=PinDef[Option.SD_CLK_PIN].GPno;
			SD_MOSI_PIN=PinDef[Option.SD_MOSI_PIN].GPno;
			SD_MISO_PIN=PinDef[Option.SD_MISO_PIN].GPno;
			ExtCfg(Option.SD_CLK_PIN, EXT_BOOT_RESERVED, 0);
			ExtCfg(Option.SD_MOSI_PIN, EXT_BOOT_RESERVED, 0);
			ExtCfg(Option.SD_MISO_PIN, EXT_BOOT_RESERVED, 0);
			gpio_init(SD_CLK_PIN);
			gpio_set_drive_strength(SD_CLK_PIN,GPIO_DRIVE_STRENGTH_12MA);
			gpio_put(SD_CLK_PIN,GPIO_PIN_RESET);
			gpio_set_dir(SD_CLK_PIN, GPIO_OUT);
			gpio_set_slew_rate(SD_CLK_PIN, GPIO_SLEW_RATE_FAST);
			gpio_init(SD_MOSI_PIN);
			gpio_set_drive_strength(SD_MOSI_PIN,GPIO_DRIVE_STRENGTH_12MA);
			gpio_put(SD_MOSI_PIN,GPIO_PIN_RESET);
			gpio_set_dir(SD_MOSI_PIN, GPIO_OUT);
			gpio_set_slew_rate(SD_MOSI_PIN, GPIO_SLEW_RATE_FAST);
			gpio_init(SD_MISO_PIN);
			gpio_set_pulls(SD_MISO_PIN,true,false);
			gpio_set_dir(SD_MISO_PIN, GPIO_IN);
			gpio_set_input_hysteresis_enabled(SD_MISO_PIN,true);
			xchg_byte= BitBangSwapSPI;
			xmit_byte_multi=BitBangSendSPI;
			rcvr_byte_multi=BitBangReadSPI;
		} else {
			SD_CLK_PIN=SPI_CLK_PIN;
			SD_MOSI_PIN=SPI_MOSI_PIN;
			SD_MISO_PIN=SPI_MISO_PIN;
		}
	}
	if(Option.AUDIO_L){ //enable the audio system
		ExtCfg(Option.AUDIO_L, EXT_BOOT_RESERVED, 0);
		ExtCfg(Option.AUDIO_R, EXT_BOOT_RESERVED, 0);
		AUDIO_L_PIN=PinDef[Option.AUDIO_L].GPno;
		AUDIO_R_PIN=PinDef[Option.AUDIO_R].GPno;
		AUDIO_SLICE=Option.AUDIO_SLICE;
		gpio_set_function(AUDIO_L_PIN, GPIO_FUNC_PWM);
		gpio_set_function(AUDIO_R_PIN, GPIO_FUNC_PWM);
		gpio_set_slew_rate(AUDIO_L_PIN, GPIO_SLEW_RATE_SLOW);
		gpio_set_slew_rate(AUDIO_R_PIN, GPIO_SLEW_RATE_SLOW);
        AUDIO_WRAP=(Option.CPU_Speed*10)/441  - 1 ;
		pwm_set_wrap(AUDIO_SLICE, AUDIO_WRAP);
        pwm_set_chan_level(AUDIO_SLICE, PWM_CHAN_A, AUDIO_WRAP>>1);
        pwm_set_chan_level(AUDIO_SLICE, PWM_CHAN_B, AUDIO_WRAP>>1);
	    pwm_clear_irq(AUDIO_SLICE);
//    	pwm_set_irq_enabled(AUDIO_SLICE, true);
    	irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    	irq_set_enabled(PWM_IRQ_WRAP, true);
		irq_set_priority(PWM_IRQ_WRAP,255);
		pwm_set_enabled(AUDIO_SLICE, true);
	}
	if(Option.PWM){
		gpio_init(23);
		gpio_put(23,GPIO_PIN_SET);
		gpio_set_dir(23, GPIO_OUT);
	} else {
		gpio_init(23);
		gpio_put(23,GPIO_PIN_RESET);
		gpio_set_dir(23, GPIO_OUT);
	}
	if(Option.SerialConsole){
//		printf("here\r\n");
		ExtCfg(Option.SerialTX, EXT_BOOT_RESERVED, 0);
		ExtCfg(Option.SerialRX, EXT_BOOT_RESERVED, 0);
		gpio_set_function(PinDef[Option.SerialTX].GPno, GPIO_FUNC_UART);
		gpio_set_function(PinDef[Option.SerialRX].GPno, GPIO_FUNC_UART);		
		uart_init(Option.SerialConsole==1 ? uart0: uart1, Option.Baudrate);
		uart_set_hw_flow(Option.SerialConsole==1 ? uart0: uart1, false, false);
		uart_set_format(Option.SerialConsole==1 ? uart0: uart1, 8, 1, UART_PARITY_NONE);
		uart_set_fifo_enabled(Option.SerialConsole==1 ? uart0: uart1,  false);
		irq_set_exclusive_handler(Option.SerialConsole==1 ? UART0_IRQ : UART1_IRQ, Option.SerialConsole==1 ? on_uart_irq0 : on_uart_irq1);
		irq_set_enabled(Option.SerialConsole==1 ? UART0_IRQ : UART1_IRQ, true);
		uart_set_irq_enables(Option.SerialConsole==1 ? uart0: uart1, true, false);
	}
	if(Option.KeyboardConfig!=NO_KEYBOARD){
		ExtCfg(KEYBOARD_CLOCK, EXT_BOOT_RESERVED, 0);
    	ExtCfg(KEYBOARD_DATA, EXT_BOOT_RESERVED, 0);
		gpio_init(PinDef[KEYBOARD_CLOCK].GPno);
		gpio_set_pulls(PinDef[KEYBOARD_CLOCK].GPno,true,false);
		gpio_set_dir(PinDef[KEYBOARD_CLOCK].GPno, GPIO_IN);
		gpio_set_input_hysteresis_enabled(PinDef[KEYBOARD_CLOCK].GPno,true);
		gpio_init(PinDef[KEYBOARD_DATA].GPno);
		gpio_set_pulls(PinDef[KEYBOARD_DATA].GPno,true,false);
		gpio_set_dir(PinDef[KEYBOARD_DATA].GPno, GPIO_IN);
		gpio_set_input_hysteresis_enabled(PinDef[KEYBOARD_DATA].GPno,true);
	}
}



