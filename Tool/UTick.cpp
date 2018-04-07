/*
 * UTick.cpp
 *
 *  Created on: 20171213
 *      Author: Romeli
 */

#include <Tool/UTick.h>

bool UTick::_InitFlag = false;
volatile uint64_t UTick::_Now = 0;
uint64_t UTick::_Last = 0;
uint32_t UTick::_Interval = 0;

/*
 * author Romeli
 * explain 初始化系统滴答计时
 * param us 最小时间刻度，单位微妙
 * return void
 */
void UTick::Init() {
	_Interval = SystemCoreClock / 1000000;
	SysTick_Config(SystemCoreClock / 1000); //Set SysTick timer=us
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
		uint32_t tick, tick2;
		uint64_t ms, ms2;
		bool flag = false; //flase 第一组数据；true 第二组数据

		//中断插入的时间点
		//1：中断优先级是最高的，所有数据安全 ms=ms2
		tick = SysTick->VAL;
		//2：比实际值大1ms，ms=ms2
		ms = _Now;
		//3：所有数据安全 ms<ms2
		tick2 = SysTick->VAL;
		//4：比实际值大1ms，ms<ms2
		ms2 = _Now;
		//5：所有数据安全 ms=ms2

		//分析下来，中断插入点只会的影响会导致计算值偏大，所以取小的那个
		if (ms == ms2) {
			//中断插入点只会出现在 2号点，此时tick2若大于tick(向下计数)则第一组数据是错误的
			if (tick2 > tick) {
				//第一组数据是错误的，返回第二组
				flag = true;
			}
		} else if (ms < ms2) {
			//中断插入点只会出现在 4号点，第一组数据是安全的
//			flag = false; 默认值
		} else {
			//Error @Romeli 不可能出现的情况
			UDebugOut("System tick unknown error");
		}
		if (flag) {
			//第二组数据
			return (ms2 * 1000) + (1000 - (tick2 / _Interval));
		} else {
			//第一组数据
			return (ms * 1000) + (1000 - (tick / _Interval));
		}
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
