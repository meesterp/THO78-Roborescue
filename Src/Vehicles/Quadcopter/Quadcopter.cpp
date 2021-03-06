#include "Quadcopter.h"
#include "MAVLinkExchanger.h"
#include <algorithm>

Quadcopter::Quadcopter(MAVLinkExchanger& communicator) :
communicator(communicator),
flightMode{ FlightMode::UNKNOWN },
armed{ false },
failsafe{ false }
{
}

void Quadcopter::liftOff(int newAltitude)
{
	changeAltitude(newAltitude);
}

void Quadcopter::changeAltitude(float targetAltitude)
{
	int newChannelThree = UINT16_MAX;
	if (targetAltitude > altitude)
	{
		newChannelThree = (RCTrimValues.CHANNEL_THREE_HIGH - 
			RCTrimValues.CHANNEL_THREE_LOW) * ASCEND_PERCENTAGE / 
			100 + RCTrimValues.CHANNEL_THREE_LOW;
		ascending = true;
		descending = false;
		std::cout << "Ascending to " << targetAltitude 
			<< " From " << altitude << std::endl;
	}
	else if (targetAltitude < altitude)
	{
		newChannelThree = (RCTrimValues.CHANNEL_THREE_HIGH - 
			RCTrimValues.CHANNEL_THREE_LOW) * DESCEND_PERCENTAGE / 
			100 + RCTrimValues.CHANNEL_THREE_LOW;
		descending = true;
		ascending = false;
		std::cout << "Descending to " << targetAltitude  
			<< " From " << altitude << std::endl;
	}
	sendRCMessage(UINT16_MAX, UINT16_MAX, newChannelThree);
}

void Quadcopter::holdAltitude()
{
	auto newChannelThree = (RCTrimValues.CHANNEL_THREE_HIGH - 
		RCTrimValues.CHANNEL_THREE_LOW) * (HOLD_PERCENTAGE /
		100) + RCTrimValues.CHANNEL_THREE_LOW;
	sendRCMessage(UINT16_MAX, UINT16_MAX, newChannelThree);
}

void Quadcopter::arm()
{
	mavlink_msg_command_long_pack(
		SYSTEMID, 
		COMPONENTID, 
		&message, 
		TARGET_SYSTEMID, 
		TARGET_COMPONENTID, 
		MAV_CMD_COMPONENT_ARM_DISARM, 
		0,
		1, 
		0,
		0,
		0,
		0,
		0,
		0);
	communicator.enqueueMessage(message);
}

void Quadcopter::disarm()
{
	mavlink_msg_command_long_pack(
		SYSTEMID, 
		COMPONENTID, 
		&message, 
		TARGET_SYSTEMID, 
		TARGET_COMPONENTID, 
		MAV_CMD_COMPONENT_ARM_DISARM, 
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0);
	communicator.enqueueMessage(message);
}

void Quadcopter::moveLeft(signed int value)
{
	moveRight(-value);
}

void Quadcopter::moveRight(signed int value)
{
	sendRCMessage(MEANVALUELEFTRIGHT + value);
}

void Quadcopter::moveForward(signed int value)
{
	moveBackward(-value);
}

void Quadcopter::moveBackward(signed int value)
{
	auto newValue = (RCTrimValues.CHANNEL_TWO_HIGH -
		RCTrimValues.CHANNEL_TWO_LOW) * value /
		MAX_PERCENTAGE + ((RCTrimValues.CHANNEL_TWO_LOW + 
		RCTrimValues.CHANNEL_TWO_HIGH) / 2 );
	sendRCMessage(UINT16_MAX,
		newValue);
}

void Quadcopter::stop()
{
	//moet nog worden aangepast zodra er een struct is voor de mid waarde.
	changeMode(FlightMode::ALTHOLD);
	sendRCMessage(
		(RCTrimValues.CHANNEL_ONE_LOW + RCTrimValues.CHANNEL_ONE_HIGH) / 2,
		(RCTrimValues.CHANNEL_TWO_LOW + RCTrimValues.CHANNEL_TWO_HIGH) / 2,
		(RCTrimValues.CHANNEL_THREE_LOW + RCTrimValues.CHANNEL_THREE_HIGH) / 2,
		(RCTrimValues.CHANNEL_FOUR_LOW + RCTrimValues.CHANNEL_FOUR_HIGH) / 2,
		(RCTrimValues.CHANNEL_FIVE_LOW + RCTrimValues.CHANNEL_FIVE_HIGH) / 2,
		(RCTrimValues.CHANNEL_SIX_LOW + RCTrimValues.CHANNEL_SIX_HIGH) / 2,
		(RCTrimValues.CHANNEL_SEVEN_LOW + RCTrimValues.CHANNEL_SEVEN_HIGH) / 2,
		(RCTrimValues.CHANNEL_EIGHT_LOW + RCTrimValues.CHANNEL_EIGHT_HIGH) / 2
		);
}

void Quadcopter::land()
{
	changeMode(FlightMode::LAND);
}

void Quadcopter::changeFlightSpeed(int)
{
	std::cout << "Not yet implemented";
}

void Quadcopter::changeHeading(int value)
{
	sendRCMessage(
		UINT16_MAX, 
		UINT16_MAX, 
		UINT16_MAX, 
		MEANVALUELEFTRIGHT + value);
}

void Quadcopter::restart()
{
	mavlink_msg_command_long_pack(
		SYSTEMID, 
		COMPONENTID,
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
	communicator.enqueueMessage(message);
}

void Quadcopter::changeMode(FlightMode mode)
{
	mavlink_msg_set_mode_pack(
		SYSTEMID, 
		COMPONENTID, 
		&message,
		TARGET_SYSTEMID, 
		TARGET_COMPONENTID,
		static_cast<uint32_t>(mode));
	communicator.enqueueMessage(message);
}

void Quadcopter::loop()
{
	while (1)
	{
		if (ascending)
		{
			if (altitude >= targetAltitude)
			{
				holdAltitude();
				ascending = false;
				std::cout << "Finished ascending to: " 
					<< targetAltitude << std::endl;
			}
		}
		else if (descending)
		{
			if (altitude <= targetAltitude)
			{
				holdAltitude();
				descending = false;
				std::cout << "Finished descending to: " 
					<< targetAltitude << std::endl;
			}
		}
		
		if (communicator.receiveQueueSize())
		{
			handleIncomingMessage(communicator.dequeueMessage());
		}
		if (!failsafe)
		{
			if (std::chrono::system_clock::now() - lastRCSent >= 
				RCHeartbeatInterval)
			{
				sendRCMessage();
			}
			if (isArmed()){
				orient();
			}
		}
	}
}

void Quadcopter::setAbsoluteHeading(int targetHeading){
	this->targetHeading = (targetHeading+360)%360;
}

void Quadcopter::changeAbsoluteHeading(int headingDifference){
	targetHeading = (targetHeading+headingDifference+360)%360;
}

void Quadcopter::orient(){
	int diff = ((targetHeading - heading + 180 + 360) % 360) - 180;
	//TODO: magic numbers
	
	if (diff > 10 || diff < 10){
		if (!orienting){
			setHeadingStreamSpeed(10);
		}
		changeAbsoluteHeading(std::min(100,diff));
	}else{
		if (orienting){
			setHeadingStreamSpeed(1);
			orienting = false;
		}
		changeAbsoluteHeading(0);
	}
}


void Quadcopter::setHeadingStreamSpeed(int i){
	mavlink_msg_request_data_stream_pack(
		SYSTEMID,
		COMPONENTID,
		&message,
		TARGET_SYSTEMID,
		TARGET_COMPONENTID,
		MAV_DATA_STREAM_RAW_CONTROLLER,
		i,
		true);
}

void Quadcopter::handleIncomingMessage(
	mavlink_message_t incomingMessage)
{
	receivedMessageMap[incomingMessage.msgid]++;

	switch (incomingMessage.msgid)
	{
		case MAVLINK_MSG_ID_HEARTBEAT:
		{
			flightMode = static_cast<FlightMode>
				(mavlink_msg_heartbeat_get_custom_mode(
					&incomingMessage));
			armed = mavlink_msg_heartbeat_get_base_mode(
				&incomingMessage) & (1 << 7);
			break;
		}
		case MAVLINK_MSG_ID_VFR_HUD:
		{
			altitude = mavlink_msg_vfr_hud_get_alt(&incomingMessage);
			heading = mavlink_msg_vfr_hud_get_heading(&incomingMessage);
			break;
		}
		case MAVLINK_MSG_ID_ATTITUDE:
		{
			roll = mavlink_msg_attitude_get_roll(&incomingMessage);
			pitch = mavlink_msg_attitude_get_pitch(&incomingMessage);
			yaw = mavlink_msg_attitude_get_yaw(&incomingMessage);
			break;
		}
		case MAVLINK_MSG_ID_STATUSTEXT:
		{
			char text[50];
			auto rtn = mavlink_msg_statustext_get_text(
				&incomingMessage, text);
			auto severity = mavlink_msg_statustext_get_severity(
				&incomingMessage);

			text[rtn - 1] = '\0';
			if (statusTextMap.count(text) > 0)
			{
				notifyListeners(statusTextMap.at(text));
			}
			else
			{
				notifyListeners(StatusText::UNKNOWN);
			}
			break;
		}
	}
	notifyListeners(StatusText::NONE);
}

std::ostream& operator<<(std::ostream& stream,
	const Quadcopter::FlightMode& mode)
{
	switch (mode)
	{
	case Quadcopter::FlightMode::STABILIZE:
		stream << "Stabilize";
		break;
	case Quadcopter::FlightMode::ACRO:
		stream << "Acro";
		break;
	case Quadcopter::FlightMode::ALTHOLD:
		stream << "AltHold";
		break;
	case Quadcopter::FlightMode::AUTO:
		stream << "Auto";
		break;
	case Quadcopter::FlightMode::GUIDED:
		stream << "Guided";
		break;
	case Quadcopter::FlightMode::LOITER:
		stream << "Loiter";
		break;
	case Quadcopter::FlightMode::RTL:
		stream << "RTL";
		break;
	case Quadcopter::FlightMode::CIRCLE:
		stream << "Circle";
		break;
	case Quadcopter::FlightMode::LAND:
		stream << "Land";
		break;
	case Quadcopter::FlightMode::DRIFT:
		stream << "Drift";
		break;
	case Quadcopter::FlightMode::SPORT:
		stream << "Sport";
		break;
	default:
		stream << "UNKNOWN";
		break;
	}
	return stream;
}

float Quadcopter::getYaw() const
{
	return yaw;
}

float Quadcopter::getRoll() const
{
	return roll;
}

float Quadcopter::getPitch() const
{
	return pitch;
}

float Quadcopter::getAltitude() const
{
	return altitude;
}

int Quadcopter::getHeading() const
{
	return heading;
}

bool Quadcopter::isArmed() const
{
	return armed;
}

Quadcopter::FlightMode Quadcopter::getMode() const
{
	return flightMode;
}

void Quadcopter::sendRCMessage(
	unsigned channelOne,
	unsigned channelTwo,
	unsigned channelThree,
	unsigned channelFour,
	unsigned channelFive,
	unsigned channelSix,
	unsigned channelSeven,
	unsigned channelEight)
{
	if (!failsafe)
	{
	mavlink_msg_rc_channels_override_pack(
		SYSTEMID,
		COMPONENTID,
		&message,
		TARGET_SYSTEMID,
		TARGET_COMPONENTID,
		channelOne,
		channelTwo,
		channelThree,
		channelFour,
		channelFive,
		channelSix,
		channelSeven,
		channelEight);
	communicator.enqueueMessage(message);
	lastRCSent = std::chrono::system_clock::now();
}
}

void Quadcopter::statusTextTest(int s)
{
	notifyListeners(static_cast<StatusText>(s));
}

void Quadcopter::saveQuadcopter()
{
	sendRCMessage(0, 0, 0, 0, 0, 0, 0, 0);
	failsafe = true;
}
