/*
 * UDebug.cpp
 *
 *  Created on: 20171108
 *      Author: Romeli
 */

#include <Misc/UDebug.h>
#include <stdio.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void UDebug::Print(uint8_t* file, int line, const char* message) {
//#ifdef DEBUG
	printf("UDebug failed: file \"%s\", line %d, message \"%s\"\n",file, line, message);
	while (true)
		;
//#endif
}

#pragma GCC diagnostic pop


