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
	volatile BytetoBit_Typedef* Data;

	ULimit(uint8_t dataSize, BitAction bitAction);
	virtual ~ULimit();

	virtual void Init() = 0;

	virtual void RefreshData() = 0;
	bool Check(uint8_t sensorNo, bool reFresh = false);
	bool WaittingFor(uint8_t sensorNo, uint64_t timeOut = 0);
	bool WaittingWhile(uint8_t sensorNo, uint64_t timeOut = 0);
	void SetActive(BitAction bitAction);
protected:
	BitAction _BitAction;
	uint8_t _DataSize;
};

#endif /* TOOL_ULIMIT_H_ */
