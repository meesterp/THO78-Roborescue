#include "ATV.h"
#include <iostream>


ATV::ATV(MAVLinkExchanger & mavlinkCommunicator) :
mavlinkCommunicator(mavlinkCommunicator)
{
}


ATV::~ATV()
{
}


void ATV::moveForward(int value)
{
	auto newValue = ((RCTrimValues.CHANNEL_THREE_LOW +
		RCTrimValues.CHANNEL_THREE_HIGH) / 2) - (RCTrimValues.CHANNEL_THREE_HIGH -
		RCTrimValues.CHANNEL_THREE_LOW) * value /
		MAX_PERCENTAGE;
	//auto sendValue = neutralthrottleValue - value;
	mavlink_msg_rc_channels_override_pack(
		SYSTEMID,
		COMPONENTID,
		&message,
		TARGET_SYSTEMID,
		TARGET_COMPONENTID,
		UINT16_MAX,
		UINT16_MAX,
		newValue,
		UINT16_MAX,
		UINT16_MAX,
		UINT16_MAX,
		UINT16_MAX,
		UINT16_MAX);
	//mavlinkCommunicator.sendMessage(message);
}


void ATV::moveBackward(int value)
{
	auto newValue = (RCTrimValues.CHANNEL_THREE_HIGH -
		RCTrimValues.CHANNEL_THREE_LOW) * value /
		MAX_PERCENTAGE + ((RCTrimValues.CHANNEL_THREE_LOW +
		RCTrimValues.CHANNEL_THREE_HIGH) / 2);
	//auto sendValue = neutralthrottleValue + value;
	mavlink_msg_rc_channels_override_pack(
		SYSTEMID,
		COMPONENTID,
		&message,
		TARGET_SYSTEMID,
		TARGET_COMPONENTID,
		UINT16_MAX,
		UINT16_MAX,
		newValue,
		UINT16_MAX,
		UINT16_MAX,
		UINT16_MAX,
		UINT16_MAX,
		UINT16_MAX);
	//mavlinkCommunicator.sendMessage(message);
}



void ATV::steer(int value)
{
	auto newValue = (RCTrimValues.CHANNEL_ONE_HIGH -
		RCTrimValues.CHANNEL_ONE_LOW) * value /
		MAX_PERCENTAGE + ((RCTrimValues.CHANNEL_ONE_LOW +
		RCTrimValues.CHANNEL_ONE_HIGH) / 2);
	//auto sendValue = neutralSteeringValue + value;
	mavlink_msg_rc_channels_override_pack(
		SYSTEMID,
		COMPONENTID,
		&message,
		TARGET_SYSTEMID,
		TARGET_COMPONENTID,
		newValue,
		UINT16_MAX,
		UINT16_MAX,
		UINT16_MAX,
		UINT16_MAX,
		UINT16_MAX,
		UINT16_MAX,
		UINT16_MAX);
	//mavlinkCommunicator.sendMessage(message);
}

void ATV::shutdown()
{
	mavlink_msg_command_long_pack(
		SYSTEMID,
		0,
		&message,
		TARGET_SYSTEMID,
		TARGET_COMPONENTID,
		MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN,
		0,
		TARGET_SYSTEMID,
		TARGET_COMPONENTID,
		0,
		0,
		0,
		0,
		0);
	//mavlinkCommunicator.sendMessage(message);
}

void ATV::returnControlToRc()
{
	mavlink_msg_rc_channels_override_pack(
		SYSTEMID,
		COMPONENTID,
		&message,
		TARGET_SYSTEMID,
		TARGET_COMPONENTID,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0);
	//mavlinkCommunicator.sendMessage(message);
}

void ATV::loop()
{
	while (1)
	{
		/*if (mavlinkCommunicator.receiveQueueSize())
		{
			handleIncomingMessage(mavlinkCommunicator.receiveMessage());
		}
		calculateRCChannels();
		exchanger.enqueueMessage(RCOverrideMessage);*/
	}
}

void ATV::handleIncomingMessage(PrioritisedMAVLinkMessage incomingMessage)
{
	switch (incomingMessage.msgid)
	{
	case MAVLINK_MSG_ID_HEARTBEAT:
		//flightMode = static_cast<FlightMode>(
			//mavlink_msg_heartbeat_get_custom_mode(&incomingMessage));
		break;
	case MAVLINK_MSG_ID_VFR_HUD:
		heading = mavlink_msg_vfr_hud_get_heading(&incomingMessage);
		groundSpeed = mavlink_msg_vfr_hud_get_groundspeed(&incomingMessage);
		break;
	case MAVLINK_MSG_ID_SYS_STATUS:
		batteryRemaining = mavlink_msg_sys_status_get_battery_remaining(
			&incomingMessage);
		break;

	}
}

const float ATV::getHeading()
{
	return heading;
}

const int ATV::getBatteryRemaining()
{
	return batteryRemaining;
}

const float ATV::getGroundSpeed()
{
	return groundSpeed;
}

const int ATV::getSteeringDirection()
{
	return steeringDirection;
}
