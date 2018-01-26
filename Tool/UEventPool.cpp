/*
 * UEventPool.cpp
 *
 *  Created on: 2018年1月24日
 *      Author: Romeli
 */

#include <Tool/UEventPool.h>

UEventPool::~UEventPool() {
	Clear();
}

/*
 * author Romeli
 * explain 当队列不为空时，依次执行事件
 * return void
 */
void UEventPool::TryDo() {
	while (!_queue.empty()) {
		_queue.front()();
		_queue.pop();
	}
}

/*
 * author Romeli
 * explain 往队列中插入一个事件
 * param 欲插入的事件
 * return void
 */
void UEventPool::Insert(voidFun event) {
	_queue.push(event);
}

/*
 * author Romeli
 * explain 清除队列中的所有事件
 * return void
 */
void UEventPool::Clear() {
	while (!_queue.empty()) {
		_queue.pop();
	}
}
