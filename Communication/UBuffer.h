/*
 * UBuffer.h
 *
 *  Created on: 2018年4月15日
 *      Author: Romeli
 */

#ifndef COMMUNICATION_UBUFFER_H_
#define COMMUNICATION_UBUFFER_H_

#include <cmsis_device.h>
#include <Misc/UMisc.h>

class UBuffer {
public:
	UBuffer(uint16_t size);
	virtual ~UBuffer();

	uint8_t* Data;
	uint16_t Size;
	volatile uint16_t Start;
	volatile uint16_t End;
	volatile bool Busy;
};

#endif /* COMMUNICATION_UBUFFER_H_ */
