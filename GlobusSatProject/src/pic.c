#include "pic.h"

//download to ground
//this has a few steps
//first lets asssume we have a pic backed up onto the sat we have to turn it into chuncks i think bc as far as ik there is a max chunck size
//this is sorta like dump so maybe i should learn beforehand?
//first in this func we can check if the file is .jpg or whatever we will use
//then we will f_tell the file if it is bigger than max chuncks we will save the first maxchunckssize into an array than transmit it with a 1
//this will make us enter a loop of saving chunck to array then transmiting til the last one 

//on the ground we will have to go through each transmition and look for missing 1.chuncks 2.chunks with missing bits(dont fit the lenght)
//ground control sends up an array and file size and the sat will check if the file is still the same size else returns E_FILE_CHANGED
//now a diff func that goes through the file chunck by chunck and check if it is in the array if so transmits

//upload to sat
//unless i come up with a better idea i can have a reverse version of what is above 
//but here i have to write a func that checks if it receved everything and send back what it finds this will have to be saved in the fram as an array
//a new cmd will have to be added GetMissingFrames() which sends back this array from the fram
//before it starts to get the pic frames makes new file name [currtime].JPG

//idea the last number in the array is the chunk id
//diff idea there is a cmd that opens a file and says how many chunks its sending this would get rid of pic id
int OpenFileUsingEpochTime() // Returns file name (epoch time)
{
    FILE *f_source;
    char filename[20];

    // Save current time
    time_unix current_time = 0;
    Time_getUnixEpoch(&current_time);

    sprintf(filename, "%ld.jpg", current_time);  // Format as "epoch_time.jpg"

    f_source = f_open(filename, "a");

    if (f_source) {
        f_close(f_source);
        return current_time;
    } else {
        return 0;  // Don't call f_close(NULL)
    }
}

void Clean_Received_Chunks(unsigned int Received_Chunks[])
{
    for (size_t i = 0; i < MAX_AMOUNT_OF_CHUNKS_FOR_IMAGE; i++)
    {
        Received_Chunks[i]=0;
    }  
}
int DownloadImageFromSat()
{

}
imageInfo_t ParseDataForImage(sat_packet_t *cmd,unsigned char cmd_data[])
{
imageInfo_t data;
if (!cmd || MAX_COMMAND_DATA_LENGTH <= 4 || (cmd && cmd->length == 0))
{return E_NOT_INITIALIZED;}

memcpy(cmd_data, cmd->data, cmd->length - sizeof(data.numberChunks));//cpys data except for end which is the chunk number
unsigned char last_bytes[4];
last_bytes[0] = cmd_data[MAX_COMMAND_DATA_LENGTH - 4];  // imageID (LSB)
last_bytes[1] = cmd_data[MAX_COMMAND_DATA_LENGTH - 3];  // imageID (MSB)
last_bytes[2] = cmd_data[MAX_COMMAND_DATA_LENGTH - 2];  // numberChunks (LSB)
last_bytes[3] = cmd_data[MAX_COMMAND_DATA_LENGTH - 1];  // numberChunks (MSB)

//cpys last two bytes (short) to numberchunks
memcpy(&data.imageID, last_bytes, sizeof(data.imageID));
memcpy(&data.numberChunks, last_bytes + 2, sizeof(data.numberChunks));

return data;
}
int UploadImageToSat(sat_packet_t *cmd)
{
//how can i check if this info is in the correct place (maybe its not a pic)?
//open new file call it [currtime].JPG 
//or maybe add pic id because how will the sat know if the file for this image is open already?
//add data to file without last int (should be addToFile(length-sizeof(int))) or maybe with
//each place in Received_Chunks is first image id then chunk number
//add curr image id in fram?
imageInfo_t data;
unsigned char cmd_data[MAX_COMMAND_DATA_LENGTH];
unsigned int Received_Chunks[MAX_AMOUNT_OF_CHUNKS_FOR_IMAGE];//add clean received_chunks right before first memcpy (is array in fram)
unsigned int Received_Chunks_Index = 0;
time_unix filename = OpenFileUsingEpochTime();
//if file was now opened for first time clean Received_Chunks
if(filename != 0)
{
    Clean_Received_Chunks(Received_Chunks);
    //fram write Received_Chunks
}
data = ParseDataForImage(cmd,cmd_data);
// here what i have to do is add func that writes data to file with image id and number chunk then adds these(id+chunk)to Received_Chunks

}



