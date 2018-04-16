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
	volatile Byte_Typedef* Data;

	ULimit(uint8_t limitNum, bool inverting = false);
	virtual ~ULimit();

	virtual void Init() = 0;

	virtual void RefreshData() = 0;
	bool Check(uint8_t limitNo, bool reFresh = false);
	bool WaittingFor(uint8_t limitNo, uint64_t timeOut = 0);
	bool WaittingWhile(uint8_t limitNo, uint64_t timeOut = 0);

	void SetInverting(bool inverting);
protected:
	uint8_t _limitNum;
	uint8_t _dataSize;
	bool _inverting;
};

#endif /* TOOL_ULIMIT_H_ */
