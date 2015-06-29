﻿/**
*               __
*    _________ / /_  ____  ________  ____________  _____
*   /___/ __ \/ __ \/ __ \/ ___/ _ \/ ___/ ___/ / / / _ \
*  / / / /_/ / /_/ / /_/ / /  /  __(__  ) /__/ /_/ /  __/
* /_/  \____/_.___/\____/_/   \___/____/\___/\__,_/\___/
*
*
* @file MapLogicVSLAM.h
* @date Created: 6/21/2015
* @version 1.0
*
* @author Coen Andriessen
*
* @section LICENSE
* License: newBSD
*
* Copyright � 2015, HU University of Applied Sciences Utrecht.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
* - Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* - Neither the name of the HU University of Applied Sciences Utrecht nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE HU UNIVERSITY OF APPLIED SCIENCES UTRECHT
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#ifndef MAPLOGICVSLAM_H
#define MAPLOGICVSLAM_H

#include "Map.hpp"
#include "VirtualRosbee.h"
#include "VirtualLidar.h"

class MapLogicVSLAM
{
public:
	/**
	* @fn	MapLogicVSLAM(Map * Map, virtualrosbee * virtualrosbee, virtuallidar * virtuallidar);
	*
	* @brief	Constructor of the class MapLogicVSLAM.
	*/
	MapLogicVSLAM(Map * map, virtualRosbee * virtualrosbee, virtualLidar * virtuallidar);
	/**
	* @fn	~MapLogicVSLAM();
	*
	* @brief	Destructor of the class MapLogicVSLAM.
	*/
	~MapLogicVSLAM();
	/**
	* @fn	void setTilesInRangevirtuallidar();
	*
	* @brief	Function to set the tiles in range of the virtuallidar.
	*/
	void setTilesInRangevirtuallidar();
	/**
	* @fn	int * getUnscannedTile();
	*
	* @brief	Function that returns the x and y of a unscanned tile in the Map.
	*
	* @return	int * with the x and y of the unscanned tile in the Map.
	*			x = tileLocation[0] and y = tileLocation[1].
	*/
	int * getUnscannedTile();
private:
	/**
	* @brief map object.
	*/
	Map * map;
	/**
	* @brief virtualrosbee object.
	*/
	virtualRosbee * virtualrosbee;
	/**
	* @brief virtuallidar object.
	*/
	virtualLidar * virtuallidar;
};

#endif // MAPLOGICVSLAM_H
