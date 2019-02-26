/*
 * UTick.h
 *
 *  Created on: 2017年12月30日
 *      Author: Romeli
 */

#ifndef UTICK_H_
#define UTICK_H_

#include <cmsis_device.h>
#include <Misc/UDebug.h>
#include <Misc/UMisc.h>

class UTick {
public:
	RCC_ClocksTypeDef Clock;

	void Init();
	void uWait(uint64_t us);
	void mWait(uint64_t ms);
	inline void Wait(uint64_t s) {
		mWait(s * 1000);
	}
	inline void Tick(uint16_t t) {
		while (t--)
			;
	}
	bool WaitOne(UJudgFun fun, uint32_t uTimeout, uint32_t interval = 0);
	bool WaitMore(UJudgFun fun, UJudgFun fun2, uint16_t times,
			uint32_t uTimeout, uint32_t interval = 0);
	uint64_t Millis();
	uint64_t Micros();
	void IRQ();
private:
	bool _InitFlag;
	volatile uint64_t _Now;
	volatile uint64_t _Last;
	uint32_t _IntervalMs;
	uint32_t _IntervalUs;
	uint32_t _ClockInterval;
	uint32_t _ClockMs;
	uint32_t _ClockUs;
};

extern UTick uTick;
#endif /* UTICK_H_ */
