#ifndef _SERIALPORTXP_H
#define _SERIALPORTXP_H
#include "serial.h"
#include <windows.h>

class SerialPortWindows : public Serial
{
public:
	explicit SerialPortWindows(const char* portName);
	~SerialPortWindows();
	int readData(char* buffer, int nbChar) override;
	bool writeData(char *buffer, int nbChar) override;
private:
	HANDLE serialHandle;
	bool connected;
	COMSTAT status;
	DWORD errors;
};
#endif