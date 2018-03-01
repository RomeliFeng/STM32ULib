/*
 * ULED.cpp
 *
 *  Created on: 2018年2月8日
 *      Author: Romeli
 */

#include <Misc/ULED.h>

ULED::ULED() {
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
		SetRedPin(true);
		SetGreenPin(true);
		SetBluePin(true);
		break;
	case Color_Red:
		SetRedPin(false);
		SetGreenPin(true);
		SetBluePin(true);
		break;
	case Color_Green:
		SetRedPin(true);
		SetGreenPin(false);
		SetBluePin(true);
		break;
	case Color_Blue:
		SetRedPin(true);
		SetGreenPin(true);
		SetBluePin(false);
		break;
	case Color_Yellow:
		SetRedPin(false);
		SetGreenPin(false);
		SetBluePin(true);
		break;
	case Color_Cyan:
		SetRedPin(true);
		SetGreenPin(false);
		SetBluePin(false);
		break;
	case Color_Purple:
		SetRedPin(false);
		SetGreenPin(true);
		SetBluePin(false);
		break;
	case Color_White:
		SetRedPin(false);
		SetGreenPin(false);
		SetBluePin(false);
		break;
	default:
		break;
	}
}

void ULED::Set(Color color, bool state) {
	switch (color) {
	case Color_Red:
		SetRedPin(!state);
		break;
	case Color_Green:
		SetGreenPin(!state);
		break;
	case Color_Blue:
		SetBluePin(!state);
		break;
	case Color_Yellow:
		SetRedPin(!state);
		SetGreenPin(!state);
		break;
	case Color_Cyan:
		SetGreenPin(!state);
		SetBluePin(!state);
		break;
	case Color_Purple:
		SetRedPin(!state);
		SetBluePin(!state);
		break;
	case Color_White:
		SetRedPin(!state);
		SetGreenPin(!state);
		SetBluePin(!state);
		break;
	default:
		break;
	}
}
