/*
 * ULimit.h
 *
 *  Created on: 2018年1月29日
 *      Author: Romeli
 */

#ifndef TOOL_ULIMIT_H_
#define TOOL_ULIMIT_H_

#include <cmsis_device.h>
#include <Misc/UMisc.h>

class ULimit {
public:
	volatile TwoWordtoBit_Typedef Data;

	ULimit(BitAction bitAction);
	virtual ~ULimit();

	void Init();

	virtual void RefreshData() = 0;
	bool Check(uint8_t sensorNo, bool reFresh = false);
	bool WaittingFor(uint8_t sensorNo, uint64_t timeOut = 0);
	bool WaittingWhile(uint8_t sensorNo, uint64_t timeOut = 0);
	void SetActive(BitAction bitAction);
private:
	virtual void GPIOInit() = 0;
	BitAction _BitAction;
};

#endif /* TOOL_ULIMIT_H_ */
