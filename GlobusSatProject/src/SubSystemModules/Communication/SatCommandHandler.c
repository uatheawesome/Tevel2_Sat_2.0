#include <satellite-subsystems/isis_vu_e.h>
#include <hal/Timing/Time.h>
#include <string.h>
#include <stdlib.h>

#include "GlobalStandards.h"
#include "SatCommandHandler.h"
#include "SPL.h"
#include "utils.h"


typedef struct __attribute__ ((__packed__)) delayed_cmd_t
{
	time_unix exec_time;	///< the execution time of the cmd in unix time
	sat_packet_t cmd;		///< command data
} delayed_cmd_t;


int ParseDataToCommand(unsigned char * data, sat_packet_t *cmd)
{
	//ask if i should add *void err
	if(data == NULL|| cmd == NULL){return null_pointer_error;}
	//copy the id of the data packet
	unsigned int id;
	char type, subtype;
	unsigned short length;
	int offset;
	
	//fill in the correct fields
	memcpy(&id,data,sizeof(id));
	offset += sizeof(id);
	memcpy(&type,data+offset,sizeof(type));
	offset += sizeof(type);
	memcpy(&subtype,data+offset,sizeof(subtype));
	offset += sizeof(subtype);
	memcpy(&length,data+offset,sizeof(length));
	offset += sizeof(length);
	//if not our sat dont parse data
	if (id>>24 != YCUBE_SAT_ID && id>>24 != ALL_SAT_ID)
	{return invalid_sat_id;}
	
	return AssembleCommand(data+offset,length,type,subtype,id,cmd);
}

int AssembleCommand(unsigned char *data, unsigned short length, char type,
		char subtype, unsigned int id, sat_packet_t *cmd)
{
	//PLS PLS PLS change err name to success 
	//if spl is changed change here
	//am i supposed to remove id type sub and length from data?
	if(cmd == NULL){return null_pointer_error;}
	cmd->ID = id;
	cmd->type = type;
	cmd->subtype = subtype;
	if(data == NULL)
	{
	cmd->length = 0; 
		return command_succsess;
	}
	if(length > MAX_COMMAND_DATA_LENGTH)
	{
	logError(SPL_DATA_TOO_BIG , "AssembleCommand");
		return execution_error;
	}
	else
	{
	 cmd->length = length;
	 //add void* err?
	 memcpy(cmd->data, data, length); 
	}
	return command_succsess;
	
}

// checks if a cmd time is valid for execution -> execution time has passed and command not expired
// @param[in] cmd_time command execution time to check
// @param[out] expired if command is expired the flag will be raised
Boolean isDelayedCommandDue(time_unix cmd_time, Boolean *expired)
{
	return FALSE;
}

//TOOD: move delayed cmd logic to the SD and write 'checked/uncheked' bits in the FRAM
int GetDelayedCommand(sat_packet_t *cmd)
{
	return 0;
}

int AddDelayedCommand(sat_packet_t *cmd)
{
	return 0;
}

int GetDelayedCommandBufferCount()
{
	unsigned char frame_count = 0;
	int err = FRAM_read(&frame_count, DELAYED_CMD_FRAME_COUNT_ADDR,
	DELAYED_CMD_FRAME_COUNT_SIZE);
	return err ? -1 : frame_count;
}


int GetDelayedCommandByIndex(unsigned int index, sat_packet_t *cmd)
{
	return 0;
}

int DeleteDelayedCommandByIndex(unsigned int index)
{
	return 0;
}

int DeleteDelayedBuffer()
{
	return 0;
}

int ClearDelayedCMD_FromBuffer(unsigned int start_addr, unsigned int end_addr)
{
	return 0;
}



int ActUponCommand(sat_packet_t *cmd)
{
	int err = 0;
	if (NULL == cmd){
		return E_NOT_INITIALIZED;
	}

	char buffer [50];
	sprintf (buffer, "ActUponCommand, cmd id: %d", cmd->ID);
	logError(INFO_MSG ,buffer);

	switch (cmd->cmd_type)
	{
	case trxvu_cmd_type:
		err = trxvu_command_router(cmd);
		break;
	case eps_cmd_type:
		err = eps_command_router(cmd);
		break;
	case telemetry_cmd_type:
		err = telemetry_command_router(cmd);
		break;
	case filesystem_cmd_type:
		err = filesystem_command_router(cmd);
		break;
	case managment_cmd_type:
		err = managment_command_router(cmd);
		break;
	}
	return err;
}
