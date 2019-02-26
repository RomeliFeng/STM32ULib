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
		_size = size;
		_sp = 0;
		_buffer = new T[size]();
	}
	~UWindowFilter() {
		delete[] _buffer;
	}

	T Get(T input) {
		//建立临时变量用于存储和
		T sum = 0;
		//将当前数据移入缓冲
		_buffer[_sp++] = input;
		//移动缓冲指针
		_sp %= _size;
		//累计求和
		for (uint8_t i = 0; i < _size; ++i) {
			sum += _buffer[i];
		}
		//返回平均数
		return sum / _size;
	}
private:
	T *_buffer;
	uint8_t _size;
	uint8_t _sp;
};

#endif /* MATH_UWINDOWFILTER_H_ */
