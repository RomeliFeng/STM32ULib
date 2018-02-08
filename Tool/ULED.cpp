/*
 * ULED.cpp
 *
 *  Created on: 2018年2月8日
 *      Author: Romeli
 */

#include <Tool/ULED.h>

ULED::ULED() {
	// TODO Auto-generated constructor stub

}


ULED::~ULED() {

}

void ULED::Init(Color color) {
	GPIOInit();
	Turn(color);
}

void ULED::Turn(Color color) {
	switch (color) {
	case Color_None:
		SetRedPin(false);
		SetGreenPin(false);
		SetBluePin(false);
		break;
	case Color_Red:
		SetRedPin(true);
		SetGreenPin(false);
		SetBluePin(false);
		break;
	case Color_Green:
		SetRedPin(false);
		SetGreenPin(true);
		SetBluePin(false);
		break;
	case Color_Blue:
		SetRedPin(false);
		SetGreenPin(false);
		SetBluePin(true);
		break;
	case Color_Yellow:
		SetRedPin(true);
		SetGreenPin(true);
		SetBluePin(false);
		break;
	case Color_Cyan:
		SetRedPin(false);
		SetGreenPin(true);
		SetBluePin(true);
		break;
	case Color_Purple:
		SetRedPin(true);
		SetGreenPin(false);
		SetBluePin(true);
		break;
	case Color_White:
		SetRedPin(true);
		SetGreenPin(true);
		SetBluePin(true);
		break;
	default:
		break;
	}
}
