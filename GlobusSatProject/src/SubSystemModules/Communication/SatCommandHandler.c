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

}

int AssembleCommand(unsigned char *data, unsigned short data_length, char type,
		char subtype, unsigned int id, sat_packet_t *cmd)
{

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
