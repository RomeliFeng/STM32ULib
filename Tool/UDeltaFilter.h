/*
 * UDeltaFilter.h
 *
 *  Created on: 2017年10月18日
 *      Author: Romeli
 */

#ifndef DELTAFILTER_H_
#define DELTAFILTER_H_

#include "cmsis_device.h"

template<typename T>
class UDeltaFilter {
public:
	UDeltaFilter(T *input, T deltaErrorLimit, uint8_t escapeTimes) {
		_Input = input;
		_DeltaErrorLimit = deltaErrorLimit;
		_EscapeTimes = escapeTimes;
		//外循环
		_Last = 0;
		_LastDelta = 0;
		//内循环
		_Last2 = 0;
		_LastDelta2 = 0;
		_MaxDeltaError = 0;
		_EscapeCount = 0;
	}

	void Set(T *input, T deltaErrorLimit, uint8_t escapeTimes);
	T Get();
private:
	T *_Input;
	T _DeltaErrorLimit;
	T _MaxDeltaError;
	T _Last, _Last2;
	T _LastDelta, _LastDelta2;
	uint8_t _EscapeTimes;
	uint8_t _EscapeCount;
};

#endif /* DELTAFILTER_H_ */
