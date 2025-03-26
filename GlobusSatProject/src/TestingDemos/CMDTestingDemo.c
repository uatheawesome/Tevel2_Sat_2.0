#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include "CMDTestingDemo.h"

#include <hal/Utility/util.h>

#include <SubSystemModules/PowerManagment/EPS.h>
#include <SubSystemModules/PowerManagment/EPSOperationModes.h>
#include <stdlib.h>

#ifdef ISISEPS
	#include <satellite-subsystems/isismepsv2_ivid7_piu.h>
#endif
#ifdef GOMEPS
	#include <satellite-subsystems/GomEPS.h>
#endif

Boolean CMD_GetExtraData_Demo()
{
	sat_packet_t cmd;
	cmd.ID = 1;
	cmd.cmd_type = telemetry_cmd_type;
	cmd.cmd_subtype = GET_TLM_INFO;
	cmd.length = sizeof(TLM_Info_Data_t);
	int err;
	err = ActUponCommand(&cmd);
	printf("result: %d", err);

	return TRUE;
}

Boolean CMD_Payload_Operations_Demo()//check
{
	unsigned char path;
	sat_packet_t cmd;
	printf("turn the payload: 1-on 2-off 3-restart else return");
	scanf("%d", &path);
	cmd.ID = 2;
	cmd.cmd_type = eps_cmd_type;
	cmd.cmd_subtype = PAYLOAD_OPERATIONS;
	cmd.length = sizeof(path);
	memcpy(&cmd.data,&path,1);
	if(path > 3){return TRUE;}
	int err;
	err = ActUponCommand(&cmd);
	printf("result: %d", err);

	return TRUE;
}

Boolean CMD_DumpRamTLM_Demo()//check with yishay
{
	unsigned char dump_type;
	unsigned int count;
	sat_packet_t cmd;
	printf("what type of tlm (eps=0,tx=1,ant=2,wod=4,radfet=5,sel-seu=7,rx=9,log=16)if not one of these err will be returned");
	scanf("%hhu", &dump_type);
	printf("how much tlm to retrieve from ram");
	scanf("%u", &count);
	cmd.ID = 3;
	cmd.cmd_type = telemetry_cmd_type;
	cmd.cmd_subtype = DUMP_RAM_TLM;
	cmd.length = (sizeof(count)+sizeof(dump_type));
	memcpy(&cmd.data,&dump_type,sizeof(dump_type));
	memcpy(cmd.data + sizeof(dump_type),&count,sizeof(count));
	int err;
	err = ActUponCommand(&cmd);
	printf("result: %d", err);

	return TRUE;
}






Boolean selectAndExecuteCMDDemoTest()
{
	unsigned int selection = 0;
	Boolean offerMoreTests = TRUE;

	printf( "\n\r Select a test to perform: \n\r");
	printf("\t 0) Return to main menu \n\r");
	printf("\t 1) CMD Get extra data\n\r");
	printf("\t 2) payload operations\n\r");
	printf("\t 3) dump ram\n\r");



	unsigned int number_of_tests = 2;
	while(UTIL_DbguGetIntegerMinMax(&selection, 0, number_of_tests) == 0);

	switch(selection) {
	case 0:
		offerMoreTests = FALSE;
		break;
	case 1:
		offerMoreTests = CMD_GetExtraData_Demo();
		break;
	case 2:
		offerMoreTests =  CMD_Payload_Operations_Demo();
		break;
	case 3:
		offerMoreTests =  CMD_DumpRamTLM_Demo();
		break;
		

	default:
		break;
	}
	return offerMoreTests;
}

Boolean MainCMDTestBench()
{
	Boolean offerMoreTests = FALSE;

	while(1)
	{
		offerMoreTests = selectAndExecuteCMDDemoTest();

		if(offerMoreTests == FALSE)
		{
			return FALSE;
		}
	}
	return FALSE;
}
