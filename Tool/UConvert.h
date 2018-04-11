/*
 * UConvert.h
 *
 *  Created on: 2016��10��6��
 *      Author: Romeli
 */

#ifndef UCONVERT_H_
#define UCONVERT_H_

#include <cmsis_device.h>
#include <Misc/UDebug.h>

class UConvert {
public:
	//利用模板类型实现的多态接口
	template<typename T>
	static inline uint8_t ByNumber(T num, uint8_t base, uint8_t* str) {
		return byNumber(num, base, str);
	}
	//利用模板类型实现的多态接口
	template<typename T>
	static inline uint8_t ByFloat(T flo, uint8_t ndigit, uint8_t* str) {
		return byFloat(flo, ndigit, str);
	}

	static uint8_t GetLen(uint32_t num, uint8_t base);
	static uint8_t GetLen(uint8_t* str);
	static inline uint8_t getLen(const char* str) {
		return GetLen((uint8_t*) str);
	}

	static uint32_t Pow10(uint8_t power);
	static uint8_t StrCat(uint8_t* str_to, uint8_t str_to_len,
	        uint8_t* str_from, uint8_t str_from_len);
private:
	static uint8_t byNumber(int32_t num, uint8_t base, uint8_t* str);
	static uint8_t byFloat(double flo, uint8_t ndigit, uint8_t* str);
};

#endif /* UCONVERT_H_ */
