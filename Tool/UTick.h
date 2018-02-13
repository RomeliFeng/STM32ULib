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

class UTick {
public:
	static void Init();
	static void uWait(uint64_t us);
	static void mWait(uint64_t ms);
	static inline void Wait(uint64_t s) {
		mWait(s * 1000);
	}
	static inline void Tick(uint16_t t) {
		while (t--)
			;
	}
	static uint64_t Millis();
	static uint64_t Micros();
	static void IRQ();
private:
	static bool _InitFlag;
	static volatile uint_fast64_t _Now;
	static uint_fast64_t _Last;
	static uint32_t _Interval;
};

#endif /* UTICK_H_ */
