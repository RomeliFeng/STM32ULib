/*
 * UDeltaFilter.cpp
 *
 *  Created on: 2017年10月18日
 *      Author: Romeli
 */

#include <Math/UDeltaFilter.h>

/*
 * author Romeli
 * explain 设置滤波器参数
 * param1 *input 输入值的指针
 * param2 deltaErrorLimit 最大误差限制
 * param3 进入内循环后逃脱所需次数
 * return void
 */
template<typename T>
void UDeltaFilter<T>::Set(T *input, T deltaErrorLimit, uint8_t escapeTimes) {
	_Input = input;
	_DeltaErrorLimit = deltaErrorLimit;
	_EscapeTimes = escapeTimes;
}

/*
 * author Romeli
 * explain 获取滤波后的值
 * return T
 */
template<typename T>
T UDeltaFilter<T>::Get() {
	T delta = _Input - _Last; //计算一阶导数
	T deltaError = delta - _LastDelta; //计算二阶导数

	deltaError = deltaError < 0 ? -deltaError : deltaError; //正值化

	if (deltaError > _MaxDeltaError) {
		//保持历史最大值
		_MaxDeltaError = deltaError;
	}
	if (deltaError < _DeltaErrorLimit) {
		//变化趋势小于限定值,直接返回记录当前趋势（二阶导数）
		_Last = _Input;
		_LastDelta = delta;
	} else {
		//变化趋势超过限定值，当前数值可能无效，进入内循环判定
		//如果当前数值无效，下一数值应脱离内循环
		delta = _Input - _Last2; //计算内循环一阶导数
		deltaError = delta - _LastDelta2; //计算内循环二阶导数

		_Last2 = _Input; //保留上次的值
		_LastDelta2 = delta; //保留上次的一阶导数
		if (deltaError < _DeltaErrorLimit) {
			//如果内循环变化率小于限定值，累计次数进行逃脱
			++_EscapeCount;
			if (_EscapeCount >= _EscapeTimes) {
				//到达指定逃脱次数，逃脱内循环
				_Last = _Input;
				_LastDelta = delta;
			}
		} else {
			//变化率依旧大于限定值，清空累计
			_EscapeCount = 0;
		}
	}
	return _Last;
}
