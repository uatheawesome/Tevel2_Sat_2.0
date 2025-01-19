#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <hal/Timing/Time.h>
#include <hal/errors.h>

#include <satellite-subsystems/isis_vu_e.h>
#include <satellite-subsystems/isis_ants.h>
#include <satellite-subsystems/isis_ants_types.h>
#include <satellite-subsystems/isismepsv2_ivid7_piu.h>
#include <satellite-subsystems/isismepsv2_ivid7_piu_types.h>


#include <stdlib.h>
#include <string.h>

#include "GlobalStandards.h"
#include "TRXVU.h"
#include "AckHandler.h"
#include "SubsystemCommands/TRXVU_Commands.h"
#include "SatCommandHandler.h"
#include "TLM_management.h"

#include "SubSystemModules/PowerManagment/EPS.h"
#include "SubSystemModules/Maintenance/Maintenance.h"
#include "SubSystemModules/Housekepping/TelemetryCollector.h"
#ifdef TESTING_TRXVU_FRAME_LENGTH
#include <hal/Utility/util.h>
#endif

#include "HashSecuredCMD.h"

int SetRSSITransponder(short rssiValue);
{
	// put set rssi + rssi in fram together?
}
Boolean CheckTransmitionAllowed();
{
	Boolean  low_voltage_flag = TRUE;
	low_voltage_flag = EpsGetLowVoltageFlag();
	if(low_voltage_flag){return FALSE;}
	//add if tx mute flag is up to return FALSE
	return TRUE;
}
// make dump better (-res?)









///------------------------------------------///
void Hash256(char* text, BYTE* outputHash)
{
    BYTE buf[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;

    // Initialize SHA256 context
    sha256_init(&ctx);

    // Hash the user input (text)
    sha256_update(&ctx, (BYTE*)text, strlen(text));
    sha256_final(&ctx, buf);

    // Copy the hash into the provided output buffer
    memcpy(outputHash, buf, SHA256_BLOCK_SIZE);
}
char error_hash[8] = {0};
int CMD_Hash256(sat_packet_t *cmd)
{
	unsigned int lastid, currId, code;
    char plsHashMe[50];
    char code_to_str[50];
    char cmpHash[Max_Hash_size], temp[Max_Hash_size];
	
    currId = cmd->ID;

	if (cmd == NULL || cmd->data == NULL) {
		return E_INPUT_POINTER_NULL;
	}

    //get code from FRAM
    FRAM_read((unsigned char*)&code, CMD_PASSWORD_ADDR, CMD_PASSWORD_SIZE);

    //get the last id from FRAM and save it into var lastid then add new id to the FRAM (as new lastid)
    FRAM_read((unsigned char*)&lastid, CMD_ID_ADDR, CMD_ID_SIZE);
    FRAM_write((unsigned char*)&currId, CMD_ID_ADDR, CMD_ID_SIZE);

    //check if curr ID is bigger than lastid
    if(currId <= lastid)
    {
        return E_UNAUTHORIZED;//bc bool FALSE needed?
    }

    //combine lastid(as str) into plshashme
    sprintf(plsHashMe, "%u", currId);

    // turn code into str
    sprintf(code_to_str, "%u", code);

    //add (passcode)
    strcat(plsHashMe, code_to_str);

    // Initialize buffer for hashed output
    BYTE hashed[SHA256_BLOCK_SIZE];

    // Hash the combined string
    Hash256(plsHashMe, hashed);

    //cpy byte by byte to temp (size of otherhashed = 8 bytes *2 (all bytes are saved by twos(bc its in hex))+1 for null)
    char otherhashed[Max_Hash_size * 2 + 1]; // Array to store 8 bytes in hex, plus a null terminator

    /*for (int i = 0; i < Max_Hash_size; i++) {
        sprintf(&otherhashed[i * 2], "%02x", hashed[i]);
    }
    otherhashed[16] = '\0'; // Add Null*/

    //cpy first 8 bytes to temp 
    memcpy(temp, hashed, Max_Hash_size);

	//add temp to globle var 
	memcpy(error_hash, temp, Max_Hash_size);
    //cpy first 8 bytes of the data
    memcpy(cmpHash, cmd -> data, Max_Hash_size);

	if(cmd -> length < Max_Hash_size)
		return E_MEM_ALLOC;

	//fix cmd.data
	cmd -> length = cmd -> length - Max_Hash_size;//8 bytes are removed from the data this must be reflected in the length
	//note: the memove is crucial to the command though can be changed to memcpy this is so the command that comes after can use the cmd. data correctly (taken out bc of error) 
	//memmove(cmd->data, cmd->data + Max_Hash_size,cmd->length - Max_Hash_size);
	
    //cmp hash from command centre to internal hash
    if(memcmp(temp, cmpHash, Max_Hash_size) == 0)
    {   
        printf("success!\n");//for test
		SendAckPacket(ACK_COMD_EXEC,cmd,NULL,0);
        return E_NO_SS_ERR;
    }
    else
	{
		return E_UNAUTHORIZED;
	}
}
