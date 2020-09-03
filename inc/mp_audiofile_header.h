#ifndef _MP_AUDIO_HEADER_H_
#define _MP_AUDIO_HEADER_H_

#include "mp_types.h"

typedef struct  
{
	// RIFF string
	MP_BYTE ucRiff[4];   
  
	// overall size of file in bytes
	MP_UINT32 uiOverallSize; 

	// WAVE string
	MP_BYTE ucWave[4];     

	// fmt string with trailing null char
	MP_BYTE ucFmtChunkMarker[4]; 

	// length of the format data
	MP_UINT32 uiLengthOfFmt;

	// format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law 
	MP_UINT32 uiFormatType;

	// no.of channels  
	MP_UINT32 uiChannels;	

	// sampling rate (blocks per second)
	MP_UINT32 uiSampleRate;

	// SampleRate * NumChannels * BitsPerSample/8 
	MP_UINT32 uiByterate;	

	// NumChannels * BitsPerSample/8
	MP_UINT32 uiBlockAlign; 

	// bits per sample, 8- 8bits, 16- 16 bits etc
	MP_UINT32 uiBitsPerSample; 

	// DATA string or FLLR string
	MP_BYTE ucDataChunkHeader[4];

	// NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
	MP_UINT32 uiDataSize;

	// Duration in seconds
	MP_FLOAT fDuration;
}HEADER;

#endif
