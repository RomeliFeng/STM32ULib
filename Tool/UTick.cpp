/*
 * UTick.cpp
 *
 *  Created on: 20171213
 *      Author: Romeli
 */

#include <Tool/UTick.h>

bool UTick::_InitFlag = false;
volatile uint_fast64_t UTick::_Now = 0;
uint_fast64_t UTick::_Last = 0;
uint32_t UTick::_Interval = 0;

/*
 * author Romeli
 * explain 初始化系统滴答计时
 * param us 最小时间刻度，单位微妙
 * return void
 */
void UTick::Init() {
	_Interval = SystemCoreClock / 1000000;
	SysTick_Config(SystemCoreClock / 1000 - 1); //Set SysTick timer=us
	NVIC_SetPriority(SysTick_IRQn, 0);					//Set SysTick interrupt
	_InitFlag = true;
}

/*
 * author Romeli
 * explain 等待一段时间
 * param us 等待的时间，微妙
 * return void
 */
void UTick::uWait(uint64_t us) {
	if (_InitFlag) {
		_Last = Micros();					//Record time_now
		while ((Micros() - _Last) < us)
			;
	} else {
		//Error @Romeli 系统滴答没有初始化
		UDebugOut("System tick has no be inited");
	}
}

/*
 * author Romeli
 * explain 等待一段时间
 * param ms 等待的时间，毫秒
 * return void
 */
void UTick::mWait(uint64_t ms) {
	if (_InitFlag) {
		_Last = Millis();					//Record time_now
		while ((Millis() - _Last) < ms)
			;
	} else {
		//Error @Romeli 系统滴答没有初始化
		UDebugOut("System tick has no be inited");
	}
}

/*
 * author Romeli
 * explain 获取开机到现在的计时，单位微妙
 * return uint64_t
 */
uint64_t UTick::Millis() {
	if (_InitFlag) {
		return _Now;
	} else {
		//Error @Romeli 系统滴答没有初始化
		UDebugOut("System tick has no be inited");
	}
	return 0;
}

/*
 * author Romeli
 * explain 获取开机到现在的计时，单位毫秒
 * return uint64_t
 */
uint64_t UTick::Micros() {
	if (_InitFlag) {
		return (_Now * 1000) + ((SysTick->LOAD - SysTick->VAL + 1) / _Interval);
	} else {
		//Error @Romeli 系统滴答没有初始化
		UDebugOut("System tick has no be inited");
	}
	return 0;
}

/*
 * author Romeli
 * explain 中断服务子程序，用于计时
 */
inline void UTick::IRQ() {
	++_Now;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-declarations"
extern "C" void SysTick_Handler() {
	UTick::IRQ();
}
#pragma GCC diagnostic pop
