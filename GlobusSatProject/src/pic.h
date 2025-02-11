#ifndef PIC_H_
#define PIC_H_


#include <hal/Timing/Time.h>
#include <string.h>
#include <stdlib.h>

#include "GlobalStandards.h"
#include "SatCommandHandler.h"
#include "SPL.h"
#include "utils.h"
//fix includes if i want to use this file
#define MaxChunkSize NULL 
#define MAX_AMOUNT_OF_CHUNKS_FOR_IMAGE NULL 
// spl packet
// _____________________________________________________________
//|																|
//|sat-id|type|sub-type|length|		data	|pic-id|chunk number|
//|_____________________________________________________________|
//size in bytes: |sat-id 4|type 1|sub-type 1|length 2|data sizeof(length)|pic-id 2|chunck num 2|
//sizeof(data) has to be MaxChunkSize - sizeof(spl packet) which here is 12 bytes

typedef struct imageInfo
{
	unsigned short imageID;
	unsigned short numberChunks;
	//char imageType;
} imageInfo_t;

int UploadImageToSat();

int DownloadImageFromSat();

#endif /* pic_h */
