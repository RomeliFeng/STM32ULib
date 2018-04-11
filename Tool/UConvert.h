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
	/*
	 * author Romeli
	 * explain 将整形数转换为字符串
	 * param num 整形数
	 * param base 进制
	 * param str 目标字符串地址
	 * return uint8_t 字符串长度
	 */
	template<typename T>
	static uint8_t ByNumber(T num, uint8_t base, uint8_t* str) {
		int32_t n = num;
		uint8_t len = 0;
		//小于零取反加负号
		if (n < 0) {
			n = -n;
			*str = '-';
			++len;
		}

		//按照进制取得长度
		len = (uint8_t) (len + GetLen((uint32_t) n, base));
		str += len;
		*str = '\0';
		do {
			uint8_t c = (uint8_t) (n % base);
			n /= base;
			*--str = c < 10 ? (uint8_t) (c + '0') : (uint8_t) (c + 'A' - 10);
		} while (n);
		str[len] = '\0';
		return len;
	}

	/*
	 * author Romeli
	 * explain 将浮点数转换为字符串
	 * param flo 浮点数
	 * param ndigit 小数位数
	 * param str 目标字符串地址
	 * return uint8_t
	 */
	template<typename T>
	static uint8_t ByFloat(T flo, uint8_t ndigit, uint8_t* str) {
		double f = flo;
		uint8_t len = 0, len2 = 0;
		uint32_t int_part = 0;
		double rem_part = 0;

		uint8_t dot = '.';

		uint8_t tmp[32];
		//小于零取反加负号
		if (f < 0.0) {
			str[len] = '-';
			f = -f;
			++len; //字符串长度+1 ‘-’号长度
		}

		// Round correctly so that print(1.999, 2) prints as "2.00"
		float rounding = 0.5f;
		for (uint8_t i = 0; i < ndigit; ++i)
			rounding /= 10.0f;

		f += rounding;

		// Extract the integer part of the number and print it
		int_part = (uint32_t) f;
		rem_part = (f - (double) int_part);

		len2 = ByNumber(int_part, 10, tmp); //转换整数部分
		len = StrCat(str, len, tmp, len2); //拼接整数部分到字符串
		if (ndigit > 0) { //如果有小数部分
			len = StrCat(str, len, &dot, 1);
			while (ndigit--) {
				rem_part *= 10;
				int_part = (int16_t) rem_part;	//每次转换一位小数
				rem_part -= int_part;
				str[len++] = (uint8_t) (int_part + 0x30);
			}
		}
		str[len] = '\0';
		return len;
	}

	static uint8_t GetLen(uint32_t num, uint8_t base);
	static uint8_t GetLen(uint8_t* str);
	static inline uint8_t getLen(const char* str) {
		return GetLen((uint8_t*) str);
	}

	static uint32_t Pow10(uint8_t power);
	static uint8_t StrCat(uint8_t* str_to, uint8_t str_to_len,
	        uint8_t* str_from, uint8_t str_from_len);
};

#endif /* UCONVERT_H_ */
