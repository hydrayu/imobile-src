#ifndef _SERIAL_PORT_FOR_WIN_H_
#define _SERIAL_PORT_FOR_WIN_H_

#include <windows.h>
#include "Opt2DAPI.h"

/**
 * @addtogroup SerialPortForWindows
 * @{
 */
struct tagSerialPort
{
	int portNumber;
	OPT_HANDLE optHandle;
	HANDLE hSerialPort;
	OVERLAPPED writingOverLapped;
	OVERLAPPED readingOverLapped;
	OVERLAPPED waitingOverLapped;
	unsigned long oneSecondSize;
	unsigned long oneCharTimeout;
	DCB dcb;
};

/**
 * For performing a serial communication port is a serial data.
 */
typedef struct tagSerialPort SerialPort;

/**
 * By initializing the SerialPort structure to create a serial port data area.
 *Serial port data that has been formatted on SerialPort_Create(),
 * You must be destroyed by the SerialPort_Destroy().
 * @param serialPort Serial port data to be initialized
 * @return Initialization was successful or not
 */
OPT_BOOL SerialPort_Create(SerialPort *serialPort);

/**
 * will destroy the data of SerialPort structure.
 * @param serialPort Serial port data to be destroyed
 */
void SerialPort_Destroy(SerialPort *serialPort);

/**
 * will open the serial port.
 * @param serialPort Serial port data to be opened
 * @param portNumber Port number to open
 */
OPT_BOOL SerialPort_Open(SerialPort *serialPort, int portNumber);

/**
 * will close the serial port.
 * @param serialPort Serial port data to be closed
 */
void SerialPort_Close(SerialPort *serialPort);

/**
 * get the port number of the serial port.
 * will return 0 if the port is not open.
 * @param serialPort Serial port data to obtain the port number
 * @return port number（0 for not open）
 */
int SerialPort_GetPortNumber(SerialPort *serialPort);

/**
 * Data from the serial port, I get the serial port opt handle.
 * @param serialPort Serial port data to get a handle opt
 */
OPT_HANDLE SerialPort_GetOptHandle(SerialPort *serialPort);

/**
 * Set the serial port to default value:
 * Baudrate = 9600,
 * Parity Check = None
 * Stop Bits = 1
 * Data Flow = None
 * @param serialPort Serial port data to open a dialog
 */
void SerialPort_SetDefault(SerialPort *serialPort);

/**
 * For port serial port data is currently opened, I open the configuration dialog.
 * @param hParent Parent window of the dialog
 * @param serialPort Serial port data to open a dialog
 */
void SerialPort_OpenConfigDialog(HWND hParent, SerialPort *serialPort);

/**
 * check the serial port to see if it is open.
 * As long as it is open, the OPT_TRUE, if it is not, and returns the OPT_FALSE.
 * @param serialPort Serial port data to find out whether it is open
 */
OPT_BOOL OPT_CALL SerialPort_IsOpen(SerialPort *serialPort);

/**
 * will read the data from the serial port.
 * @param serialPort Serial port to read the data
 * @param buffer Read buffer
 * @param length Number of bytes of data to read
 * @return The number of bytes actually read
 */
OPT_DWORD OPT_CALL SerialPort_Read(SerialPort *serialPort, OPT_CHAR *buffer, OPT_DWORD length);

/**
 * will wait for data to be received by the serial port.
 * @param serialPort Serial port to listen for incoming
 * @param timeoutMilliSeconds The maximum time to wait (in milliseconds)
 * @return Number of bytes available reads at the stage of detecting the received data
 */
OPT_DWORD OPT_CALL SerialPort_WaitForInput(SerialPort *serialPort, OPT_DWORD timeoutMilliSeconds);

/**
 * will write the data to the serial port.
 * @param serialPort Serial port data to write the data
 * @param buffer Head pointer of data to be written
 * @param length Number of bytes of data to be written
 */
void OPT_CALL SerialPort_Write(SerialPort *serialPort, const OPT_CHAR *buffer, OPT_DWORD length);

/**
 * Will delay processing in sleep.
 * @param serialPort Do not use
 * (If the thread should be sleep is different from the serial port, 
 * you can then take advantage of this parameter, so that you can determine the thread should be sleep.)
 * @param milliSeconds Time to sleep (in milliseconds)
 */
void OPT_CALL SerialPort_Delay(SerialPort *serialPort, OPT_DWORD milliSeconds);

/**
 * If the serial communication, you are buffering your own, do the flash process.
 * The implementation of this serial communication, because it does not buffer alone, it does not do anything in this function.
 * @param serialPort Do not use (If your communication, to buffer their own, using this argument, do the flash process.)
 */
void OPT_CALL SerialPort_Flush(SerialPort *serialPort);

/**
 * will make the settings for the serial port of a captured image acquisition.
 * @param serialPort Serial port data to be set
 */
void OPT_CALL SerialPort_SetShotConfig(SerialPort *serialPort);
/**
 * will make the settings for the serial port at the time of download.
 * @param serialPort Serial port data to be set
 */
void OPT_CALL SerialPort_SetDownloadConfig(SerialPort *serialPort);

/**
 * will make the settings for the serial port at the normal time.
 * この関数は、撮影画像取得や、ダウンロード終了時に、
 * Shooting and image acquisition, to download at the end of this function
 *  is used when you want to undo the settings for the serial port.
 * @param serialPort Serial port data to be set
 */
void OPT_CALL SerialPort_SetUserConfig(SerialPort *serialPort);

/**
 * @}
 */

#endif//_SERIAL_PORT_FOR_WIN_H_
