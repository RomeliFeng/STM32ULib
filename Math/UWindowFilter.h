/*
 * UWindowFilter.h
 *
 *  Created on: 2018年3月31日
 *      Author: Romeli
 */

#ifndef MATH_UWINDOWFILTER_H_
#define MATH_UWINDOWFILTER_H_

#include <cmsis_device.h>

template<typename T>
class UWindowFilter {
public:
	UWindowFilter(uint8_t size) {
		_Size = size;
		_Sp = 0;
		Buffer = new T[size]();
	}
	~UWindowFilter() {
		delete[] Buffer;
	}

	T Get(T input) {
		//建立临时变量用于存储和
		T sum = 0;
		//将当前数据移入缓冲
		Buffer[_Sp++] = input;
		//移动缓冲指针
		_Sp %= _Size;
		//累计求和
		for (uint8_t i = 0; i < _Size; ++i) {
			sum += Buffer[i];
		}
		//返回平均数
		return sum / _Size;
	}
private:
	T *Buffer;
	uint8_t _Size;
	uint8_t _Sp;
};

#endif /* MATH_UWINDOWFILTER_H_ */
