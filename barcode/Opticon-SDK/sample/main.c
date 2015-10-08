#include "Opt2DAPI.h"
#include "SerialPortWin.h"
#include <stdio.h>

void ok_or_exit(char *msg)
{
	if (GetLastError() != ERROR_SUCCESS)
	{
		printf ("Error %d for %s\n", GetLastError(), msg);
		exit(GetLastError());
	}
}

void Opt2D_ok_or_exit(char *msg)
{
	if (Opt2D_GetLastState() != OPT_STATE_SUCCESS)
	{
		printf ("Opt2D Error %d for %s\n", Opt2D_GetLastState(), msg);
		exit (Opt2D_GetLastState());
	}
}


/**
 * Use the port number that you specify on the command-line arguments,
 * It is a program designed to read some code in a two-dimensional module.
 * @param argc The number of command line arguments
 * @param argv Command line argument data
 * @return WindowsAPI Error Code or will return the error code OPT_STATE (return value of Opt2D_GetLastState())
 */
int main(int argc, char **argv)
{
	char *initializeCommand = "[BAPMZ$6PSMGPR";
	char *triggerCommand = "Z";
	char *triggerCancelCommand = "Y";
	SerialPort serialPort;
	OPT_HANDLE twoDimensionModule;
	char readLengthString[8];
	OPT_DWORD readLength;
	int portNumber;
	char readResult[8192];
	char cmd[1024];

	memset(cmd, 0, 1024);
	if (argc > 2) {
		int i;
		for (i=2; i<argc; ++i)
		{
			int argvLen = strlen(argv[i]);
			if (argvLen <= 2) { 
				strcat_s(cmd, 1024, argv[i]);
			}
			else if (argvLen == 3) {
				cmd[strlen(cmd)] = '[';
				strcat_s(cmd, 1024, argv[i]);
			}
			else if (argvLen == 4) {
				cmd[strlen(cmd)] = ']';
				strcat_s(cmd, 1024, argv[i]);
			}
			else {
				printf ("Error COMMAND on argc %d with '%s'....Skip it.\n", argc, argv[i]);
			}
		}
	}

	//Get port number from the command line argument, default is COM1
	if (argc < 2)
	{
		portNumber = 7;
	}
	else
	{
		if (sscanf(argv[1], "%d", &portNumber) != 1)
		{
			portNumber = 1;
		}
	}

	//Prepare a serial port using the WindowsAPI
	SerialPort_Create(&serialPort);
	ok_or_exit("SerialPort_Create()");

	//Open the serial port using the WindowsAPI
	SerialPort_Open(&serialPort, portNumber);
	ok_or_exit("SerialPort_Open()");

	//Use the WindowsAPI, to open the serial port settings dialog
	// baudrate: 9600, 8N1, Off
	// SerialPort_OpenConfigDialog(HWND_DESKTOP, &serialPort);
	SerialPort_SetDefault(&serialPort);
	ok_or_exit("SerialPort_SetDefault()");

	//To do the following: If the connection is successful
	if (SerialPort_IsOpen(&serialPort))
	{
		//To create a two-dimensional module handle using Opt2DAPI
		twoDimensionModule = Opt2D_CreateTwoDimensionModule(SerialPort_GetOptHandle(&serialPort));
		ok_or_exit("Opt2D_CreateTwoDimensionModule()");
		Opt2D_ok_or_exit("Opt2D_CreateTwoDimensionModule()");

		//Will send the default command for initialize
		Opt2D_SendCommand(twoDimensionModule, initializeCommand, (OPT_DWORD) strlen(initializeCommand));
		ok_or_exit("Opt2D_SendCommand()");
		Opt2D_ok_or_exit("Opt2D_SendCommand()");

		if (strlen(cmd) > 0) {
			//will send commands from command line arguments
			printf ("Send command '%s'\n", cmd);
			Opt2D_SendCommand(twoDimensionModule, cmd, (OPT_DWORD) strlen(cmd));
		}
		else {
			//will send a trigger start command
			printf ("Send trigger read command '%s'\n", triggerCommand);
			Opt2D_SendCommand(twoDimensionModule, triggerCommand, (OPT_DWORD) strlen(triggerCommand));
		}
		ok_or_exit("Opt2D_SendCommand()");
		Opt2D_ok_or_exit("Opt2D_SendCommand()");
		//will wait for the results read 20 seconds
		SerialPort_WaitForInput(&serialPort, 20000);
		ok_or_exit("SerialPort_WaitForInput()");

		if (Opt2D_GetLastState() == OPT_STATE_SUCCESS)
		{
			//Reading results when comes back
			//Reads six characters, get the number of characters to be read
			SerialPort_Read(&serialPort, readLengthString, 6);
			ok_or_exit("SerialPort_Read()");
			Opt2D_ok_or_exit("SerialPort_Read()");
			sscanf(readLengthString, "%d", &readLength);
			//Read and display character number of characters that is obtained
			SerialPort_Read(&serialPort, readResult, readLength);
			ok_or_exit("SerialPort_Read()");
			Opt2D_ok_or_exit("SerialPort_Read()");
			readResult[readLength] = '\0';
			printf("%s\n", readResult);
		}
		else if (Opt2D_GetLastState() == OPT_STATE_COMMUNICATION_TIMEOUT)
		{
			ok_or_exit("SerialPort_WaitForInput(1)");

			//Reading results when not returned,
			//will send a trigger end command
			Opt2D_SendCommand(twoDimensionModule, triggerCancelCommand, (OPT_DWORD) strlen(triggerCancelCommand));
			ok_or_exit("Opt2D_SendCommand()");
			Opt2D_ok_or_exit("Opt2D_SendCommand()");
			//Show that the result was a time-out
			printf("Timeout!!\n");
		}
		else
		{
			ok_or_exit("SerialPort_WaitForInput(2)");
			return Opt2D_GetLastState();
		}

		//To destroy the two-dimensional module handle using Opt2DAPI
		Opt2D_DestroyHandle(twoDimensionModule);
		Opt2D_ok_or_exit("Opt2D_DestroyHandle()");

		//Close the serial port using the WindowsAPI
		SerialPort_Close(&serialPort);
		ok_or_exit("SerialPort_Close()");
		Opt2D_ok_or_exit("SerialPort_Close()");
	}

	//Discard the serial port using the WindowsAPI
	SerialPort_Destroy(&serialPort);
	ok_or_exit("SerialPort_Destroy()");
	Opt2D_ok_or_exit("SerialPort_Destroy()");

	//Enter to end
	printf("Push return.");
	getchar();

	//Successful completion
	return 0;
}
