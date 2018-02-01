/*
 * ULimit.h
 *
 *  Created on: 2018年1月29日
 *      Author: Romeli
 */

#ifndef TOOL_ULIMIT_H_
#define TOOL_ULIMIT_H_

#include <cmsis_device.h>
#include <UMisc.h>

class ULimit {
public:
	TwoWordtoBit_Typedef Data;

	ULimit(BitAction bitAction);
	virtual ~ULimit();

	void RefreshData();
	bool Check(uint8_t sensorNo, bool reFresh = false);
	void WaittingFor(uint8_t sensorNo);
	void WaittingWhile(uint8_t sensorNo);
	void SetActive(BitAction bitAction);
private:
	virtual void GPIOInit() = 0;
	BitAction _BitAction;
};

#endif /* TOOL_ULIMIT_H_ */
