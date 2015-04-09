#include "Quadcopter.h"
#include "../Dependencies/Serial/SerialPort.h"
#include <string>
#include "MAVLinkExchanger.h"
#include <iostream>
#include <thread>

SerialPort serialPort("COM7");
MAVLinkExchanger communicator{ serialPort };
Quadcopter quadcopter(communicator);

int main() 
{
	std::thread quadcopterLoopThread { &Quadcopter::loop, &quadcopter};
	std::thread communicatorLoopThread{ &MAVLinkExchanger::loop, &communicator };
	quadcopterLoopThread.detach();
	communicatorLoopThread.detach();
	while(1) 
	{
	}
}