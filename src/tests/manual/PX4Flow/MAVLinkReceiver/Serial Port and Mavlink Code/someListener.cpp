#include "someListener.h"
#include <iostream>
#include "CImg/CImg.h"
int pixel = 0;
cimg_library::CImg<unsigned char> image;
std::string imgS = "";
int imageWidth, imageHeight, imageSize;

SomeListener::SomeListener(){
	
}

SomeListener::SomeListener(MavlinkSubject * m){
	m->addListener(this);
	this->m = m;
	//m->addListener(this);
	//m->addListener(this);
}
void SomeListener::update(mavlink_message_t* msg){
	
	switch (msg->msgid){
		case MAVLINK_MSG_ID_HEARTBEAT:
			std::cout << "Heartbeat\n";
		break;
		
		case MAVLINK_MSG_ID_DATA_TRANSMISSION_HANDSHAKE:
			{
				std::cout << "Data transmission handshake -- \n	quality: [" << ((int)mavlink_msg_data_transmission_handshake_get_jpg_quality(msg)) << "]\n	seq: [" << ((int)msg->seq) << "]\n	type: [" << ((int)mavlink_msg_data_transmission_handshake_get_type(msg)) << "]\n	size: [" << ((int)mavlink_msg_data_transmission_handshake_get_size(msg)) << "]\n	packets: [" << ((int)mavlink_msg_data_transmission_handshake_get_packets(msg)) << "]\n	width: [" << ((int)mavlink_msg_data_transmission_handshake_get_width(msg)) << "]\n	height: [" << ((int)mavlink_msg_data_transmission_handshake_get_height(msg)) << "]\n";
				if (imgS!=""){
					image.save(imgS.c_str());
					std::cout << "Saved "<<imgS<<".\n";
				}
				pixel = 0;
				imgS = "img/image";
				imgS += std::to_string(img);
				imgS +=".BMP";
				std::cout << "Creating "<<imgS<<".\n";
				img ++;
				size = mavlink_msg_data_transmission_handshake_get_size(msg);
				imageWidth = mavlink_msg_data_transmission_handshake_get_width(msg);
				imageHeight = mavlink_msg_data_transmission_handshake_get_height(msg);
				imageSize = imageWidth*imageHeight;
				image.assign(imageWidth,imageHeight,1,1);
				std::cout << "Created "<<imgS<<".\n";
			}
		break;
		
		case MAVLINK_MSG_ID_ENCAPSULATED_DATA:
		{
			uint8_t buffer[253];
			mavlink_encapsulated_data_t* data = new mavlink_encapsulated_data_t();
			mavlink_msg_encapsulated_data_decode(msg,data);
			int size = mavlink_msg_encapsulated_data_get_data(msg,buffer);
			std::cout << "frame ["<<size<<"]\n";
			for (int i = 0; i < size; i ++){
				if (pixel < imageSize){
					image(pixel % imageWidth,pixel / imageWidth,0,0) = buffer[i]; //R
				}
				pixel++;
			}
		}
		break;
		
		case MAVLINK_MSG_ID_DEBUG_VECT:
			std::cout << "Debug vect -- time_usec:[" << mavlink_msg_debug_vect_get_time_usec(msg)<<"]\n";
		break;
		
		case MAVLINK_MSG_ID_OPTICAL_FLOW:
			std::cout << "Optical flow -- ground distance:["<< mavlink_msg_optical_flow_get_ground_distance(msg) << "]\n";
		break;
		
		case MAVLINK_MSG_ID_OPTICAL_FLOW_RAD:
			std::cout << "Optical flow rad -- ground distance:["<< mavlink_msg_optical_flow_rad_get_distance(msg) << "]\n";
		break;
		
		default:
			std::cout << "Unknown msgID [" << ((int)msg->msgid) << "]\n";
		break;
	}
	/*mavlink_array_test_0_t* testArray = new mavlink_array_test_0_t();
	mavlink_msg_array_test_0_decode(msg,testArray);
	for (int i = 0; i < 32; i++) {
		std::cout << testArray->ar_c[i];
	}
	std::cout << "]\n";
	delete testArray;*/
}