// tunes.h

#include "sound.h"


/******************************************************************************
*
*   A song is defined by a table of notes. The first byte sets the tempo. A 
*   high byte will give a low tempo, and opposite. Each tone consists of two 
*   bytes. The first gives the length of the tone, the other gives the frequency. 
*   The frequencies for each tone are defined in the "sound.h". Timer0 controls 
*   the tempo and the length of each tone, while Timer1 with PWM gives the 
*   frequency. The second last byte is a "0" which indicates the end, and the
*   very last byte makes the song loop if it's "1", and not loop if it's "0".
*
******************************************************************************/

const char TEXT_SONG1[] PROGMEM      = "Fur Elise";

const int FurElise[] PROGMEM=   
        {
            3, 
            8,e2, 8,xd2, 8,e2, 8,xd2, 8,e2, 8,b1, 8,d2, 8,c2, 4,a1, 8,p, 
            8,c1, 8,e1, 8,a1, 4,b1, 8,p, 8,e1, 8,xg1, 8,b1, 4,c2, 8,p, 8,e1, 
            8,e2, 8,xd2, 8,e2, 8,xd2, 8,e2, 8,b1, 8,d2, 8,c2, 4,a1, 8,p, 8,c1, 
            8,e1, 8,a1, 4,b1, 8,p, 8,e1, 8,c2, 8,b1, 4,a1, 
            0, 1
        };


const char TEXT_SONG2[] PROGMEM  = "Turkey march";

const int Mozart[] PROGMEM = 
        {
            3, 
            16,xf1, 16,e1, 16,xd1, 16,e1, 4,g1, 16,a1, 16,g1, 16,xf1, 16,g1,
            4,b1, 16,c2, 16,b1, 16,xa1, 16,b1, 16,xf2, 16,e2, 16,xd2, 16,e2, 
            16,xf2, 16,e2, 16,xd2, 16,e2, 4,g2, 8,e2, 8,g2, 32,d2, 32,e2, 
            16,xf2, 8,e2, 8,d2, 8,e2, 32,d2, 32,e2, 16,xf2, 8,e2, 8,d2, 8,e2, 
            32,d2, 32,e2, 16,xf2, 8,e2, 8,d2, 8,xc2, 4,b1, 
            0, 1
        };

const char TEXT_SONG3[] PROGMEM      = "Minuet";

const int Minuet[] PROGMEM = 
        {
            2, 
            4,d2, 8,g1, 8,a1, 8,b1, 8,c2, 4,d2, 4,g1, 4,g1, 4,e2, 8,c2, 
            8,d2, 8,e2, 8,xf2, 4,g2, 4,g1, 4,g1, 4,c2, 8,d2, 8,c2, 8,b1, 
            8,a1, 4,b1, 8,c2, 8,b1, 8,a1, 8,g1, 4,xf1, 8,g1, 8,a1, 8,b1, 
            8,g1, 4,b1, 2,a1, 
            0, 1
        };


char TEXT_SONG4[] PROGMEM    = "Auld Lang Syne";

int AuldLangSyne[] PROGMEM = 
        {  
            3, 
            4,g2, 2,c3, 8,c3, 4,c3, 4,e3, 2,d3, 8,c3, 4,d3, 8,e3, 8,d3, 2,c3, 
            8,c3, 4,e3, 4,g3, 2,a3, 8,p, 4,a3, 2,g3, 8,e3, 4,e3, 4,c3, 2,d3, 
            8,c3, 4,d3, 8,e3, 8,d3, 2,c3, 8,a2, 4,a2, 4,g2, 2,c3, 4,p,
            0, 1
        };


const char TEXT_SONG5[] PROGMEM =   "Sirene1";

const int Sirene1[] PROGMEM = 
        {
            0,
            32,400, 32,397, 32,394, 32,391, 32,388, 32,385, 32,382, 32,379,
            32,376, 32,373, 32,370, 32,367, 32,364, 32,361, 32,358, 32,355,
            32,352, 32,349, 32,346, 32,343, 32,340, 32,337, 32,334, 32,331, 
            32,328, 32,325, 32,322, 32,319, 32,316, 32,313, 32,310, 32,307, 
            32,304, 32,301, 32,298, 32,298, 32,301, 32,304, 32,307, 32,310, 
            32,313, 32,316, 32,319, 32,322, 32,325, 32,328, 32,331, 32,334, 
            32,337, 32,340, 32,343, 32,346, 32,349, 32,352, 32,355, 32,358, 
            32,361, 32,364, 32,367, 32,370, 32,373, 32,376, 32,379, 32,382, 
            32,385, 32,388, 32,391, 32,394, 32,397, 32,400,
            0, 1
        };

const char TEXT_SONG6[] PROGMEM =   "Sirene2";

const int Sirene2[] PROGMEM = 
        {
            3, 
            4,c2, 4,g2, 
            0, 1
        };


const char TEXT_SONG7[] PROGMEM      =   "Whistle";

const int Whistle[] PROGMEM = 
        {
            0, 
            32,200, 32,195, 32,190, 32,185, 32,180, 32,175, 32,170, 32,165,  
            32,160, 32,155, 32,150, 32,145, 32,140, 32,135, 32,130, 32,125,              
            32,120, 32,115, 32,110, 32,105, 32,100, 8,p, 32,200, 32,195, 
            32,190, 32,185, 32,180, 32,175, 32,170, 32,165, 32,160, 32,155, 
            32,150, 32,145, 32,140, 32,135, 32,130, 32,125, 32,125, 32,130, 
            32,135, 32,140, 32,145, 32,150, 32,155, 32,160, 32,165, 32,170, 
            32,175, 32,180, 32,185, 32,190, 32,195, 32,200, 
            0, 0
        };


// pointer-array with pointers to the song arrays
const int *Songs[] PROGMEM   = { FurElise, Mozart, Minuet, AuldLangSyne, Sirene1, Sirene2, Whistle, 0};

const char *TEXT_SONG_TBL[]    = { TEXT_SONG1, TEXT_SONG2, TEXT_SONG3, TEXT_SONG4, TEXT_SONG5, TEXT_SONG6, TEXT_SONG7, 0};

const int *pSong;	// mt point to a ram location (pointer array Songs)

static char Volume = 250;
static char Duration = 0;
static char Tone = 0;
static char Tempo;


