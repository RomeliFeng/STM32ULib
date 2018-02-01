/*
 * ULimit.cpp
 *
 *  Created on: 2018年1月29日
 *      Author: Romeli
 */

#include <Tool/ULimit.h>

ULimit::ULimit(BitAction bitAction) {
	_BitAction = bitAction;
}

ULimit::~ULimit() {
}

void ULimit::RefreshData() {
}

bool ULimit::Check(uint8_t sensorNo, bool reFresh) {
	if (reFresh) {
		RefreshData();
	}
//if(Data.bit.)
}

void ULimit::WaittingFor(uint8_t sensorNo) {
}

void ULimit::WaittingWhile(uint8_t sensorNo) {
}

void ULimit::SetActive(BitAction bitAction) {
}
