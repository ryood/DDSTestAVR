// DDSTestAVR.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <io.h>
#include <fcntl.h>

#define _USE_MATH_DEFINES
#include <math.h>

/*********************************************************
phaseRegister   : 16bit
tunigWord       : 16bit
lookupTable     : 16bit (use 12bit)
**********************************************************/

#define SAMPLE_CLOCK	15625 // 15.625kHz = 16MHz / 8 / 128
#define TABLE_SIZE	    1024  // Lookup Table Size = 1024

uint16_t phaseRegister;
uint16_t tuningWord;
uint16_t *lookupTable;

// frequency > SAMPLE_CLOCK / 2^16 (about 0.24Hz)
double frequency = 1000;          
//int period = 5 * SAMPLE_CLOCK;
int period = 128;

int _tmain(int argc, _TCHAR* argv[])
{
	_setmode(_fileno(stdout), _O_BINARY);

	// Lookup Table の作成
	lookupTable = (uint16_t*)malloc(TABLE_SIZE * sizeof(uint16_t));

	// Sine Wave
	for (int i = 0; i < TABLE_SIZE; i++) {
		lookupTable[i] = (sin(2.0 * M_PI * i / TABLE_SIZE) / 2 + 0.5) * 0x0FFF;
		//printf("%d\n", lookupTable[i]);
	}
	//printf("\n*********************\n");

	tuningWord = frequency * pow(2.0, 16) / SAMPLE_CLOCK;
	//printf("tuningWord: %x\n", tuningWord);

	phaseRegister = 0x0000;
	for (int i = 0; i < period; i++) {
		int index;

		phaseRegister += tuningWord;
		//printf("phr: %x\n", phaseRegister);

		index = phaseRegister >> 6;
		//printf("index: %d\n", index);

		printf("%d\n", lookupTable[index]);
	}

	free(lookupTable);

	return 0;
}


