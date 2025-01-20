#ifndef PIC_H_
#define PIC_H_

#define MaxChunkSize null 
// spl packet
// _____________________________________________________________
//|																|
//|sat-id|type|sub-type|length|		data	|pic-id|chunk number|
//|_____________________________________________________________|
//size in bytes: |sat-id 4|type 1|sub-type 1|length 2|data sizeof(length)|pic-id 2|chunck num 2|
//sizeof(data) has to be MaxChunkSize - sizeof(spl packet) which here is 12 bytes
int UploadImageToSat();

int DownloadImageFromSat();

