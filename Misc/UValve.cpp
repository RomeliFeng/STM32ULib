/*
 * UValve.cpp
 *
 *  Created on: 2018年4月2日
 *      Author: Romeli
 */

#include <Misc/UValve.h>

UValve::UValve(uint8_t valveNum, bool inverting) {
	_dataSize = valveNum / 8 + 1;
	_data = new BytetoBit_Typedef[_dataSize]();
	SetInverting(inverting);
}

UValve::~UValve() {
	delete[] _data;
}

void UValve::Open() {
	uint8_t status = _inverting ? 0x00 : 0xff;
	for (uint8_t i = 0; i < _dataSize; ++i) {
		_data[i].byte = status;
	}
	Apply();
}

void UValve::Open(uint8_t valveNo) {
	//求第几个字节
	uint8_t index = valveNo >> 3;
	//计算掩码
	uint8_t mask = uint8_t(1) << (valveNo % 8);
	if (_inverting) {
		_data[index].byte &= (~mask);
	} else {
		_data[index].byte |= mask;
	}
	Apply();
}

void UValve::Close() {
	uint8_t status = _inverting ? 0xff : 0x00;
	for (uint8_t i = 0; i < _dataSize; ++i) {
		_data[i].byte = status;
	}
	Apply();
}

void UValve::Close(uint8_t valveNo) {
	//求第几个字节
	uint8_t index = valveNo >> 3;
	//计算掩码
	uint8_t mask = uint8_t(1) << (valveNo % 8);
	if (_inverting) {
		_data[index].byte |= mask;
	} else {
		_data[index].byte &= (~mask);
	}
	Apply();
}

void UValve::Control(uint8_t* status) {
	for (uint8_t i = 0; i < _dataSize; ++i) {
		_data[i].byte = _inverting ? ~status[i] : status[i];
	}
	Apply();
}

void UValve::Control(uint8_t* status, bool onOff) {
	for (uint8_t i = 0; i < _dataSize; ++i) {
		if ((_inverting && onOff) || ((!_inverting) && (!onOff))) {
			//当反向并且打开  或者 未反向并且关闭
			_data[i].byte &= (~status[i]);
		} else {
			_data[i].byte |= status[i];
		}
	}
	Apply();
}

void UValve::SetInverting(bool inverting) {
	_inverting = inverting;
}
