/*
 * UValve.h
 *
 *  Created on: 2018年4月2日
 *      Author: Romeli
 */

#ifndef MISC_UVALVE_H_
#define MISC_UVALVE_H_

#include <Misc/UMisc.h>

class UValve {
public:
	volatile Byte_Typedef* Data;

	UValve(uint8_t valveNum, bool inverting = false);
	virtual ~UValve();

	virtual void Init() = 0;

	void Open();
	void Open(uint8_t valveNo);
	void Close();
	void Close(uint8_t valveNo);
	void Control(uint8_t* status);
	void Control(uint8_t* status, bool onOff);

	void SetInverting(bool inverting);
protected:
	uint8_t _valveNum;
	uint8_t _dataSize;
	bool _inverting;

	virtual void Apply() = 0;
};

#endif /* MISC_UVALVE_H_ */
