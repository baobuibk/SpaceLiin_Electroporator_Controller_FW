/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "command.h"
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static void fsp_print(uint8_t packet_length);
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
extern uart_stdio_typedef RS232_UART;
extern uart_stdio_typedef RF_UART;
extern uart_stdio_typedef GPP_UART;

tCmdLineEntry g_psCmdTable[] = {
		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Cap Control Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		{ "CAP_VOLT",			CMD_CAP_VOLT, 			" : Set cap voltage" },
		{ "CAP_CONTROL",		CMD_CAP_CONTROL, 		" : Control charger on/off" },
		{ "CAP_RELEASE",		CMD_CAP_RELEASE, 		" : Control releasing cap" },
		//{ "CAP_GET",			CMD_CAP_GET, 			" : Command to get all related cap controll data" },

		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Pulse Control Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		{ "PULSE_COUNT",		CMD_PULSE_COUNT, 		" : Set number of pulse" },
		{ "PULSE_DELAY",		CMD_PULSE_DELAY, 		" : Set delay between pulse hv and lv" },
		{ "PULSE_HV", 			CMD_PULSE_HV, 			" : Set hs pulse on time and off time" },
		{ "PULSE_LV", 			CMD_PULSE_LV, 			" : Set ls pulse on time and off time" },
		{ "PULSE_CONTROL", 		CMD_PULSE_CONTROL, 		" : Start pulsing" },
		//{ "PULSE_GET",			CMD_PULSE_GET, 			" : Command to get all related pulse controll data" },

		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Relay Control Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		{ "RELAY_SET", 			CMD_RELAY_SET, 			" : Set up cuvette" },
		{ "RELAY_CONTROL", 		CMD_RELAY_CONTROL, 		" : Stop cuvette" },
		//{ "RELAY_GET", 			CMD_RELAY_GET, 			" : Command to get all related relay controll data" },

		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ VOM Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		{ "MEASURE_VOLT", 		CMD_MEASURE_VOLT,		" : Measure cap voltage"},
		{ "MEASURE_CURRENT",	CMD_MEASURE_CURRENT,	" : Measure cuvette current"},
		{ "MEASURE_IMPEDANCE", 	CMD_MEASURE_IMPEDANCE,	" : Measure cuvette impedance"},

		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ultility Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		{ "HELP", 				CMD_HELP,				" : Display list of commands" },
		{ "CALIB_RUN", 			CMD_CALIB_RUN, 			" : Start cap calib process" },
		{ "CALIB_MEASURE",		CMD_CALIB_MEASURE, 		" : Command to input VOM value" },
		{ "CHANNEL_SET", 		CMD_CHANNEL_SET, 		" : Choose a cap channel" },
		{ "CHANNEL_CONTROL",	CMD_CHANNEL_CONTROL,	" : Control the setted channel" },
		{ "CALL_GPP", 			CMD_CALL_GPP,	    	" : Test communicate to GPP" },
		{ "GET_BMP390", 		CMD_GET_BMP390,	    	" : Get temperature and pressure" },
		{ 0, 0, 0 }
};
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: Cap Control Command :::::::: */
int CMD_CAP_VOLT(int argc, char *argv[])
{
	if (g_is_calib_running == true) {
		return CMDLINE_CALIB_IS_RUNNING;
	}

	if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	if ((receive_argm[0] > 300) || (receive_argm[0] < 0))
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[1] > 50) || (receive_argm[1] < 0))
		return CMDLINE_INVALID_ARG;

	Calib_Calculate(receive_argm[0], receive_argm[1]);

	return CMDLINE_OK;
}

int CMD_CAP_CONTROL(int argc, char *argv[])
{
	if (g_is_calib_running == true) {
		return CMDLINE_CALIB_IS_RUNNING;
	}

	if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	if ((receive_argm[0] > 1) || (receive_argm[0] < 0))
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[1] > 1) || (receive_argm[1] < 0))
		return CMDLINE_INVALID_ARG;

	if (receive_argm[0] == 1) {
		g_is_Discharge_300V_On = 0;
	}

	if (receive_argm[1] == 1) {
		g_is_Discharge_50V_On = 0;
	}

	PID_is_300V_on = receive_argm[0];
	PID_is_50V_on = receive_argm[1];

	return CMDLINE_OK;
}

int CMD_CAP_RELEASE(int argc, char *argv[])
{
	if (g_is_calib_running == true) {
		return CMDLINE_CALIB_IS_RUNNING;
	}

	if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	if ((receive_argm[0] > 1) || (receive_argm[0] < 0))
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[1] > 1) || (receive_argm[1] < 0))
		return CMDLINE_INVALID_ARG;

	if (receive_argm[0] == 1) {
		PID_is_300V_on = 0;
	}

	if (receive_argm[1] == 1) {
		PID_is_50V_on = 0;
	}

	g_is_Discharge_300V_On = receive_argm[0];
	g_is_Discharge_50V_On = receive_argm[1];

	return CMDLINE_OK;
}

/* :::::::::: Pulse Control Command :::::::: */
int CMD_PULSE_COUNT(int argc, char *argv[])
{
	if (g_is_calib_running == true) {
		return CMDLINE_CALIB_IS_RUNNING;
	}

	if (argc < 5)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 5)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[4];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);
	receive_argm[2] = atoi(argv[3]);
	receive_argm[3] = atoi(argv[4]);

	if ((receive_argm[0] > 20) || (receive_argm[1] > 20) || (receive_argm[2] > 20) || (receive_argm[3] > 20))
		return CMDLINE_INVALID_ARG;

	pu_GPC_FSP_Payload->pulseCount.Cmd 			= FSP_CMD_PULSE_COUNT;
	pu_GPC_FSP_Payload->pulseCount.HV_pos_count = receive_argm[0];
	pu_GPC_FSP_Payload->pulseCount.HV_neg_count = receive_argm[1];

	pu_GPC_FSP_Payload->pulseCount.LV_pos_count = receive_argm[2];
	pu_GPC_FSP_Payload->pulseCount.LV_neg_count = receive_argm[3];

	fsp_print(5);
	return CMDLINE_OK;
}

int CMD_PULSE_DELAY(int argc, char *argv[])
{
	if (g_is_calib_running == true) {
		return CMDLINE_CALIB_IS_RUNNING;
	}

	if (argc < 4)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 4)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[3];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);
	receive_argm[2] = atoi(argv[3]);

	if ((receive_argm[0] > 100) || (receive_argm[0] < 0))
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[1] > 100) || (receive_argm[1] < 0))
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[2] > 1000) || (receive_argm[2] < 0))
		return CMDLINE_INVALID_ARG;

	pu_GPC_FSP_Payload->pulseDelay.Cmd 			= FSP_CMD_PULSE_DELAY;
	pu_GPC_FSP_Payload->pulseDelay.HV_delay		= receive_argm[0];
	pu_GPC_FSP_Payload->pulseDelay.LV_delay		= receive_argm[1];

	pu_GPC_FSP_Payload->pulseDelay.Delay_low 	= receive_argm[2];
	pu_GPC_FSP_Payload->pulseDelay.Delay_high 	= (receive_argm[2]  >> 8);

	fsp_print(5);
	return CMDLINE_OK;
}

int CMD_PULSE_HV(int argc, char *argv[])
{
	if (g_is_calib_running == true) {
		return CMDLINE_CALIB_IS_RUNNING;
	}

	if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	if ((receive_argm[0] > 20) || (receive_argm[0] < 1))
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[1] > 20) || (receive_argm[1] < 1))
		return CMDLINE_INVALID_ARG;

	pu_GPC_FSP_Payload->pulseHV.Cmd 	= FSP_CMD_PULSE_HV;
	pu_GPC_FSP_Payload->pulseHV.OnTime 	= receive_argm[0];
	pu_GPC_FSP_Payload->pulseHV.OffTime = receive_argm[1];

	fsp_print(3);
	return CMDLINE_OK;
}

int CMD_PULSE_LV(int argc, char *argv[])
{
	if (g_is_calib_running == true) {
		return CMDLINE_CALIB_IS_RUNNING;
	}

	if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	if ((receive_argm[0] > 1000) || (receive_argm[0] < 1))
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[1] > 1000) || (receive_argm[1] < 1))
		return CMDLINE_INVALID_ARG;

	pu_GPC_FSP_Payload->pulseLV.Cmd = FSP_CMD_PULSE_LV;
	pu_GPC_FSP_Payload->pulseLV.OnTime_low 		= receive_argm[0];
	pu_GPC_FSP_Payload->pulseLV.OnTime_high 	= (receive_argm[0] >> 8);
	pu_GPC_FSP_Payload->pulseLV.OffTime_low 	= receive_argm[1];
	pu_GPC_FSP_Payload->pulseLV.OffTime_high 	= (receive_argm[1] >> 8);

	fsp_print(5);
	return CMDLINE_OK;
}

int CMD_PULSE_CONTROL(int argc, char *argv[])
{
	if (g_is_calib_running == true) {
		return CMDLINE_CALIB_IS_RUNNING;
	}

	if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int8_t receive_argm = atoi(argv[1]);

	if ((receive_argm > 1) || (receive_argm < 0))
		return CMDLINE_INVALID_ARG;

	pu_GPC_FSP_Payload->pulseControl.Cmd = FSP_CMD_PULSE_CONTROL;
	pu_GPC_FSP_Payload->pulseControl.State = receive_argm;

	fsp_print(2);
	return CMDLINE_OK;
}

/* :::::::::: Relay Control Command :::::::: */
int CMD_RELAY_SET(int argc, char *argv[])
{
	if (g_is_calib_running == true) {
		return CMDLINE_CALIB_IS_RUNNING;
	}

	if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	if (receive_argm[0] == receive_argm[1])
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[0] > 9) || (receive_argm[0] < 1) || (receive_argm[0] == 5))
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[1] > 9) || (receive_argm[1] < 1) || (receive_argm[1] == 5))
		return CMDLINE_INVALID_ARG;

	pu_GPC_FSP_Payload->relaySet.Cmd = FSP_CMD_RELAY_SET;
	pu_GPC_FSP_Payload->relaySet.HvRelay = receive_argm[0];
	pu_GPC_FSP_Payload->relaySet.LvRelay = receive_argm[1];

	fsp_print(3);
	return CMDLINE_OK;
}

int CMD_RELAY_CONTROL(int argc, char *argv[])
{
	if (g_is_calib_running == true) {
		return CMDLINE_CALIB_IS_RUNNING;
	}

	if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm;

	receive_argm = atoi(argv[1]);

	if ((receive_argm > 1) || (receive_argm < 0))
		return CMDLINE_INVALID_ARG;

	pu_GPC_FSP_Payload->relayControl.Cmd = FSP_CMD_RELAY_CONTROL;
	pu_GPC_FSP_Payload->relayControl.State = receive_argm;

	fsp_print(2);
	return CMDLINE_OK;
}

/* :::::::::: VOM Command :::::::: */
int CMD_MEASURE_VOLT(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	uint32_t data_tmp[2];
	uint8_t data1_tmp[2];
	data_tmp[0] = g_Feedback_Voltage[0]* 10 / hv_calib_coefficient.average_value;
	data_tmp[1] = g_Feedback_Voltage[1]* 10 / lv_calib_coefficient.average_value;

	data1_tmp[0] = data_tmp[0] % 10;
	data1_tmp[1] = data_tmp[1] % 10;

	data_tmp[0] /= 10;
	data_tmp[1] /= 10;
	UART_Printf(&RS232_UART, "> HV cap: %d.%dV, LV cap: %d.%dV\r\n", data_tmp[0], data1_tmp[0], data_tmp[1], data1_tmp[1]);
	UART_Printf(&RF_UART, "> HV cap: %d.%dV, LV cap: %d.%dV\r\n", data_tmp[0], data1_tmp[0], data_tmp[1], data1_tmp[1]);

	return CMDLINE_OK;
}

int CMD_MEASURE_CURRENT(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	return CMDLINE_OK;
}

uint8_t Impedance_Period = 0;
int CMD_MEASURE_IMPEDANCE(int argc, char *argv[])
{
	if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	Impedance_Period = atoi(argv[2]);

	g_is_Discharge_300V_On = 0;
	g_is_Discharge_50V_On = 0;
	PID_is_300V_on = 0;
	PID_is_50V_on = 0;
	Calib_Calculate(receive_argm[0], 0);
	UART_Printf(&RF_UART, "> CHARGING HV TO %dV\r\n", receive_argm[0]);
	PID_is_300V_on = 1;

	SchedulerTaskEnable(7, 1);

	return CMDLINE_OK;
}

/* :::::::::: Ultility Command :::::::: */
int CMD_HELP(int argc, char *argv[])
{
	tCmdLineEntry *pEntry;

	UART_Send_String(&RS232_UART, "\nAvailable commands\r\n");
	UART_Send_String(&RS232_UART, "------------------\r\n");

	UART_Send_String(&RF_UART, "\nAvailable commands\r\n");
	UART_Send_String(&RF_UART, "------------------\r\n");

	// Point at the beginning of the command table.
	pEntry = &g_psCmdTable[0];

	// Enter a loop to read each entry from the command table.  The
	// end of the table has been reached when the command name is NULL.
	while (pEntry->pcCmd) {
		// Print the command name and the brief description.
		UART_Send_String(&RS232_UART, pEntry->pcCmd);
		UART_Send_String(&RS232_UART, pEntry->pcHelp);
		UART_Send_String(&RS232_UART, "\r\n");

		UART_Send_String(&RF_UART, pEntry->pcCmd);
		UART_Send_String(&RF_UART, pEntry->pcHelp);
		UART_Send_String(&RF_UART, "\r\n");

		// Advance to the next entry in the table.
		pEntry++;

	}
	// Return success.
	return (CMDLINE_OK);
}

int CMD_CALIB_RUN(int argc, char *argv[])
{
	if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm = atoi(argv[1]);

	if ((receive_argm > 1) || (receive_argm < 0))
		return CMDLINE_INVALID_ARG;

	g_is_calib_running = receive_argm;

	if (receive_argm == 1) {
		SchedulerTaskEnable(6, 1);
	}

	return CMDLINE_OK;
}

int CMD_CALIB_MEASURE(int argc, char *argv[])
{
	if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	g_HV_Measure_mv = receive_argm[0];
	g_LV_Measure_mv = receive_argm[1];

	g_is_measure_available = true;

	return CMDLINE_OK;
}

int CMD_CHANNEL_SET(int argc, char *argv[])
{
	if (g_is_calib_running == true) {
		return CMDLINE_CALIB_IS_RUNNING;
	}

	if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm;

	receive_argm = atoi(argv[1]);

	if ((receive_argm > 2) || (receive_argm < 1))
		return CMDLINE_INVALID_ARG;

	pu_GPC_FSP_Payload->channelSet.Cmd = FSP_CMD_CHANNEL_SET;
	pu_GPC_FSP_Payload->channelSet.Channel = receive_argm;

	fsp_print(2);
	return CMDLINE_OK;
}

int CMD_CHANNEL_CONTROL(int argc, char *argv[])
{
	if (g_is_calib_running == true) {
		return CMDLINE_CALIB_IS_RUNNING;
	}

	if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm;

	receive_argm = atoi(argv[1]);

	if ((receive_argm > 1) || (receive_argm < 0))
		return CMDLINE_INVALID_ARG;

	pu_GPC_FSP_Payload->channelControl.Cmd = FSP_CMD_CHANNEL_CONTROL;
	pu_GPC_FSP_Payload->channelControl.State = receive_argm;

	fsp_print(2);
	return CMDLINE_OK;
}

int CMD_CALL_GPP(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	pu_GPC_FSP_Payload->handshake.Cmd 	= FSP_CMD_HANDSHAKE;
	pu_GPC_FSP_Payload->handshake.Check = 0xAB;

	fsp_print(2);
	return CMDLINE_OK;
}
int CMD_GET_BMP390(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	pu_GPC_FSP_Payload->commonFrame.Cmd = FSP_CMD_GET_BMP390;

	fsp_print(1);
	return CMDLINE_OK;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static void fsp_print(uint8_t packet_length)
{
	s_GPC_FSP_Packet.sod 		= FSP_PKT_SOD;
	s_GPC_FSP_Packet.src_adr 	= fsp_my_adr;
	s_GPC_FSP_Packet.dst_adr 	= FSP_ADR_GPP;
	s_GPC_FSP_Packet.length 	= packet_length;
	s_GPC_FSP_Packet.type 		= FSP_PKT_TYPE_CMD_W_DATA;
	s_GPC_FSP_Packet.eof 		= FSP_PKT_EOF;
	s_GPC_FSP_Packet.crc16 		= crc16_CCITT(FSP_CRC16_INITIAL_VALUE, &s_GPC_FSP_Packet.src_adr, s_GPC_FSP_Packet.length + 4);

	uint8_t encoded_frame[25] = { 0 };
	uint8_t frame_len;
	fsp_encode(&s_GPC_FSP_Packet, encoded_frame, &frame_len);

	UART_FSP(&GPP_UART, encoded_frame, frame_len);
}
