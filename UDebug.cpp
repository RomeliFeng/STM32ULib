/*
 * UDebug.cpp
 *
 *  Created on: 20171108
 *      Author: Romeli
 */

#include <UDebug.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void UDebug::Print(uint8_t* file, uint32_t line, const char* message) {
#ifdef DEBUG
	trace_printf("U_assert() failed: file \"%s\", line %d, message \"%s\"",
			file, line, message);
	trace_printf("\n");
#endif
}

#pragma GCC diagnostic pop


