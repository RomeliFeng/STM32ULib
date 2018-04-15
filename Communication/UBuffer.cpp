/*
 * UBuffer.cpp
 *
 *  Created on: 2018年4月15日
 *      Author: Romeli
 */

#include <Communication/UBuffer.h>

UBuffer::UBuffer(uint16_t size) {
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
	if (Data != nullptr) {
		delete[] Data;
	}
}

bool UBuffer::IsEmpty() {
	return (Size == 0) || (Start == End);
}

uint16_t UBuffer::Available() {
	return uint16_t(Start <= End ? End - Start : Size - Start + End);
}

/*
 * author Romeli
 * explain 清空缓冲内数据
 * return void
 */
void UBuffer::Discard(uint16_t num) {
	if ((num != 0) && (num < Available())) {
		//如果丢弃的字节小于缓冲剩余字节数，丢弃相应的字节并退出
		Start = (Start + num) % Size;
	} else {
		Start = End;
	}
}

/*
 * author Romeli
 * explain 向下移动流指针
 * return void
 */
void UBuffer::SpInc() {
	if (!IsEmpty()) {
		//缓冲区指针+1
		Start = uint16_t((Start + 1) % Size);
	}
}

/*
 * author Romeli
 * explain 向上移动流指针
 * return void
 */
void UBuffer::SpDec() {
	Start = uint16_t(Start == 0 ? Size : Start - 1);
}
