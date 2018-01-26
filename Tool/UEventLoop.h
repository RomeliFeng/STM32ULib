/*
 * UEventLoop.h
 *
 *  Created on: 2018年1月24日
 *      Author: Romeli
 */

#ifndef UEVENTLOOP_H_
#define UEVENTLOOP_H_

#include <cmsis_device.h>
#include <Tool/UTick.h>
#include <UMisc.h>
#include <list>

class UEventLoop {
public:
	struct Unit_Typedef {
		Unit_Typedef(voidFun event, uint64_t interval, uint64_t overTime = 0) {
			//事件回调函数
			Event = event;
			//事件回调时间间隔
			Interval = interval;
			//最长超时时间
			OverTime = overTime;
			//事件上次回调时间
			LastCall = 0;
		}
		voidFun Event;
		uint64_t Interval;
		uint64_t LastCall;
		uint64_t OverTime;
	};

	typedef std::list<Unit_Typedef> EventList;

	virtual ~UEventLoop();

	void TryDo();
	void Insert(voidFun event, uint32_t interval);
	void Remove(voidFun event);
	void Clear();
private:
	EventList _List;
};

#endif /* UEVENTLOOP_H_ */
