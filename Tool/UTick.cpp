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
uint16_t UTick::_Interval = 0;

/*
 * author Romeli
 * explain 初始化系统滴答计时
 * param us 最小时间刻度，单位微妙
 * return void
 */
void UTick::Init(uint16_t us) {
	_Interval = us;
	SysTick_Config((SystemCoreClock / (1000000 / _Interval)) - 5); //Set SysTick timer=us
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
		_Last = _Now;					//Record time_now
		while ((_Now - _Last) < us)
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
	return Micros() / 1000;
}

/*
 * author Romeli
 * explain 获取开机到现在的计时，单位毫秒
 * return uint64_t
 */
uint64_t UTick::Micros() {
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
 * explain 中断服务子程序，用于计时
 */
void UTick::IRQ() {
	_Now += _Interval;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-declarations"
extern "C" void SysTick_Handler() {
	UTick::IRQ();
}
#pragma GCC diagnostic pop
