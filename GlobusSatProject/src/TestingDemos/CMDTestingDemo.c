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
	ActUponCommand(&cmd);

	return TRUE;
}

Boolean CMD_Payload_Operations_Demo()//check
{
	int path;
	sat_packet_t cmd;
	printf("turn the payload: 1-on 2-off 3-restart else return");
	scanf("%d", &path);
	cmd.ID = 2;
	cmd.cmd_type = eps_cmd_type;
	cmd.cmd_subtype = PAYLOAD_OPERATIONS;
	cmd.length = 00 01;//this prob doesnt work
	memcpy(&cmd -> data,&path,1);
	ActUponCommand(&cmd);

	return TRUE;
}






Boolean selectAndExecuteCMDDemoTest()
{
	unsigned int selection = 0;
	Boolean offerMoreTests = TRUE;

	printf( "\n\r Select a test to perform: \n\r");
	printf("\t 0) Return to main menu \n\r");
	printf("\t 1) CMD Get extra data\n\r");



	unsigned int number_of_tests = 2;
	while(UTIL_DbguGetIntegerMinMax(&selection, 0, number_of_tests) == 0);

	switch(selection) {
	case 0:
		offerMoreTests = FALSE;
		break;
	case 1:
		offerMoreTests = CMD_GetExtraData_Demo();
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
