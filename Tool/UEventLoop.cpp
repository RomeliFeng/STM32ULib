/*
 * UEventLoop.cpp
 *
 *  Created on: 2018年1月24日
 *      Author: Romeli
 */

#include <Tool/UEventLoop.h>
#include <UDebug.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waggregate-return"

/*
 * author Romeli
 * explain 删除循环时释放资源
 */
UEventLoop::~UEventLoop() {
	Clear();
}

/*
 * author Romeli
 * explain 进行事件轮询检查
 * return void
 */
void UEventLoop::TryDo() {
	EventList::iterator iter = _List.begin();
	do {
		Unit_Typedef& unit = *iter;
		uint64_t now = UTick::Millis();
		if (now - unit.LastCall >= unit.Interval) {
			//如果事件距离上次被调用超过了间隔时间
			unit.LastCall = now;
			unit.Event();
		}
		if ((unit.OverTime != 0) && (now - unit.LastCall >= unit.OverTime)) {
			iter = _List.erase(iter);
		} else {
			++iter;
		}
	} while (iter != _List.end());
}

/*
 * author Romeli
 * explain 往事件循环中插入一个事件
 * param1 event 事件回调函数
 * param2 interval 调用时间间隔
 * return void
 */
void UEventLoop::Insert(voidFun event, uint32_t interval) {
	//尝试移除相同事件
	Remove(event);
	//插入新的事件
	_List.push_back(Unit_Typedef(event, interval));
}

/*
 * author Romeli
 * explain 从事件循环中移除指定事件
 * param event 待移除的事件
 * return void
 */
void UEventLoop::Remove(voidFun event) {
	EventList::iterator iter = _List.begin();
	do {
		if ((*iter).Event == event) {
			iter = _List.erase(iter);
		} else {
			++iter;
		}
	} while (iter != _List.end());
}

/*
 * author Romeli
 * explain 清空所有的事件
 * return void
 */
void UEventLoop::Clear() {
	_List.clear();
}

#pragma GCC diagnostic pop
