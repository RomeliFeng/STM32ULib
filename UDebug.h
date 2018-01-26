/*
 * UDebug.h
 *
 *  Created on: 20171108
 *      Author: Romeli
 */

#ifndef UDEBUG_H_
#define UDEBUG_H_

#include <cmsis_device.h>
#include <diag/Trace.h>

#define UDebugOut(message) (UDebug::Print((uint8_t*) (__FILE__), __LINE__, message))

class UDebug {
public:
	static void Print(uint8_t* file, uint32_t line, const char* message);
};


#endif /* UDEBUG_H_ */
