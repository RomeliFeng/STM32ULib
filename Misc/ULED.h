/*
 * ULED.h
 *
 *  Created on: 2018年2月8日
 *      Author: Romeli
 */

#ifndef TOOL_ULED_H_
#define TOOL_ULED_H_

#include <cmsis_device.h>

class ULED {
public:
	enum Color {
		Color_None,
		Color_Red,
		Color_Green,
		Color_Blue,
		Color_Yellow,
		Color_Cyan,
		Color_Purple,
		Color_White
	};

	ULED();
	virtual ~ULED();

	void Init(Color color = Color_None);
	void Turn(Color color);
	void Set(Color color, bool state);
protected:
	virtual void GPIOInit() = 0;

	virtual void SetRedPin(bool state) = 0;
	virtual void SetGreenPin(bool state) = 0;
	virtual void SetBluePin(bool state) = 0;
};

#endif /* TOOL_ULED_H_ */
