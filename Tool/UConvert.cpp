/*
 * UConvert.cpp
 *
 *  Created on: 20161006
 *      Author: Romeli
 */

#include <Tool/UConvert.h>

/*
 * author Romeli
 * explain 根据进制计算整形数长度
 * param1 num 整形数
 * param2 base 进制
 * return uint8_t
 */
uint8_t UConvert::GetLen(uint32_t num, uint8_t base) {
	uint8_t i;
	if (num == 0) {
		return 1;
	}
	for (i = 0; num != 0; ++i) {
		num /= base;
	}
	return i;
}

/*
 * author Romeli
 * explain 求字符串长度
 * param 字符串
 * return uint8_t
 */
uint8_t UConvert::GetLen(uint8_t* str) {
	uint8_t len = 0;
	while (*(str + len) != '\0') {
		++len;
	}
	return len;
}

/*
 * author Romeli
 * explain 求10的n次方
 * param power 次方数
 * return uint32_t
 */
uint32_t UConvert::Pow10(uint8_t power) {
	if (power == 0)
		return 1;
	else
		return 10 * Pow10(--power);
}

/*
 * author Romeli
 * explain 两个字符串拼接
 * param1 str_to 目标字符串
 * param2 str_to_len 目标字符串长度
 * param3 str_from 被拼接的字符串
 * param4 str_from_len 被拼接的字符串长度
 * return uint8_t
 */
uint8_t UConvert::StrCat(uint8_t* str_to, uint8_t str_to_len, uint8_t* str_from,
		uint8_t str_from_len) {
	uint8_t i;
	for (i = 0; i < str_from_len; ++i) { //搬移数据
		str_to[str_to_len + i] = str_from[i];
	}
	str_to[str_to_len + i] = '\0'; //在字符串末尾填'\0'
	return (uint8_t) (str_to_len + str_from_len); //返回字符串长度
}
