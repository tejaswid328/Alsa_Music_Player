#include <stdlib.h>
#include "mp_interface.h"

RET_STATUS mp_extract_header_info(MP_CHAR *filepath, HEADER *fileheader)
{
    MP_UCHAR buffer4[4];
    MP_UCHAR buffer2[2];
    FILE *fptr = fopen(filepath, "rb");
    if (fptr == NULL) 
    {
        exit(1);
    }
    MP_INT32 read;
    read =fread(fileheader->ucRiff, sizeof(fileheader->ucRiff), 1, fptr);
    read = fread(buffer4, sizeof(buffer4), 1, fptr);
    if (read > 0)
    {
        fileheader->uiOverallSize  = buffer4[0] | 
				(buffer4[1]<<8) | 
				(buffer4[2]<<16) | 
				(buffer4[3]<<24);
    }
    read = fread(fileheader->ucWave, sizeof(fileheader->ucWave), 1, fptr);
    read = fread(fileheader->ucFmtChunkMarker, sizeof(fileheader->ucFmtChunkMarker), 1, fptr);
    read = fread(buffer4, sizeof(buffer4), 1, fptr);
    fileheader->uiLengthOfFmt = buffer4[0] |
				(buffer4[1] << 8) |
				(buffer4[2] << 16) |
				(buffer4[3] << 24);
    read = fread(buffer2, sizeof(buffer2), 1, fptr); 
    fileheader->uiFormatType = buffer2[0] | (buffer2[1] << 8);
    read = fread(buffer2, sizeof(buffer2), 1, fptr);
    fileheader->uiChannels = buffer2[0] | (buffer2[1] << 8);
    read = fread(buffer4, sizeof(buffer4), 1, fptr);
    fileheader->uiSampleRate = buffer4[0] |
				(buffer4[1] << 8) |
				(buffer4[2] << 16) |
				(buffer4[3] << 24);
    read = fread(buffer4, sizeof(buffer4), 1, fptr);
    fileheader->uiByterate  = buffer4[0] | 
				(buffer4[1] << 8) | 
				(buffer4[2] << 16) | 
				(buffer4[3] << 24);
    read = fread(buffer2, sizeof(buffer2), 1, fptr);
    fileheader->uiBlockAlign = buffer2[0] | 
				(buffer2[1] << 8); 
    read = fread(buffer2, sizeof(buffer2), 1, fptr);
    fileheader->uiBitsPerSample = buffer2[0] |
				  (buffer2[1] << 8);
    read = fread(fileheader->ucDataChunkHeader, sizeof(fileheader->ucDataChunkHeader), 1, fptr);
    read = fread(buffer4, sizeof(buffer4), 1, fptr);
    if (read > 0)
    {
        fileheader->uiDataSize = buffer4[0] |
					(buffer4[1] << 8) |
					(buffer4[2] << 16) | 
					(buffer4[3] << 24 );
    }
    fileheader->fDuration = (MP_FLOAT) fileheader->uiOverallSize / fileheader->uiByterate;
    fclose(fptr);
    return MP_SUCCESS;
}
