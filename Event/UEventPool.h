/*
 * UEventPool.h
 *
 *  Created on: 2018年1月24日
 *      Author: Romeli
 */

#ifndef UEVENTPOOL_H_
#define UEVENTPOOL_H_

#include <cmsis_device.h>
#include <Misc/UMisc.h>
#include <queue>

class UEventPool {
public:
	~UEventPool();
	void TryDo();
	void Insert(UEvent event);
	void Clear();
private:
	std::queue<UEvent> _queue;
};

#endif /* UEVENTPOOL_H_ */
