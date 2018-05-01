/*
 * UBuffer.cpp
 *
 *  Created on: 2018年4月15日
 *      Author: Romeli
 */

#include <Communication/UBuffer.h>

UBuffer::UBuffer(uint16_t size) :
		Size(size) {
	Size = size;
	Start = 0;
	End = 0;
	if (Size != 0) {
		Data = new uint8_t[Size];
	} else {
		Data = nullptr;
	}
}

UBuffer::~UBuffer() {
	//unique_str 意义不大，反而对调试增加难度
	if (Data != nullptr) {
		delete[] Data;
	}
}
