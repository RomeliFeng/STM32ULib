/*
 * UPID.cpp
 *
 *  Created on: Aug 10, 2016
 *      Author: Romeli
 */

#include <Math/UPID.h>

UPID::UPID(float& input, float& setPoint, float& output, float kp, float ki,
		float kd, float outputMin, float outputMax, float interval,
		Mode_Typedef mode, Dir_Typedef dir) :
		_input(input), _setPoint(setPoint), _output(output) {

	_mode = mode;
	_dir = dir;
	_interval = interval;

	_lastInput = _input;
	_pError = 0;
	_iTerm = 0;
	_dError = 0;
	_outputMin = outputMin;
	_outputMax = outputMax;

	SetTunings(kp, ki, kd);
}

void UPID::SetTunings(float kp, float ki, float kd) {
	if (_kp < 0 || _ki < 0 || _kd < 0) {
		return;
	}
	_kp = kp;
	_ki = ki * _interval;
	_kd = kd / _interval;

	if (_dir == Dir_Negtive) {
		_kp = (0 - _kp);
		_ki = (0 - _ki);
		_kd = (0 - _kd);
	}
}

void UPID::SetInterval(float interval) {
	if (interval < 0) {
		return;
	}
	float ratio = interval / _interval;
	_ki *= ratio;
	_kd /= ratio;
	_interval = interval;
}

void UPID::SetLimits(float min, float max) {
	if (max < min) {
		return;
	}
	_outputMin = min;
	_outputMax = max;
	if (_iTerm > _outputMax) {
		_iTerm = _outputMax;
	} else if (_iTerm < _outputMin) {
		_iTerm = _outputMin;
	}
	if (_output > _outputMax) {
		_output = _outputMax;
	} else if (_output < _outputMin) {
		_output = _outputMin;
	}
}

void UPID::SetDir(Dir_Typedef dir) {
	if (_dir != dir) {
		_kp = 0 - _kp;
		_ki = 0 - _ki;
		_kd = 0 - _kd;
		_dir = dir;
	}
}

void UPID::Compute() {
	//计算误差和误差的倒数
	_pError = _setPoint - _input;
	_dError = _input - _lastInput;

	if (_mode == Mode_PONM) {
		//当处于PONM模式时，使用比例量压制积分
		_iTerm -= _kp * _dError;
	}

	//累加积分量
	if (!(((_output >= _outputMax) && ((_pError >= 0) && (_iTerm >= 0)))
			|| ((_output < _outputMin) && ((_pError < 0) && (_iTerm < 0))))) {
		//如果输出没有饱和，累加积分量
		_iTerm += _pError * _ki;
	}

	//限幅积分量
	_iTerm = Constrain(_iTerm);

	//计算输出值 当处于PONM模式时没有比例量
	//传出最终值
	_output = Constrain(
			(_mode != Mode_PONM ? (_kp * _pError) : 0) + _iTerm
					- (_kd * _dError));
	//记录这次的输入值
	_lastInput = _input;
}

void UPID::Clear() {
	_iTerm = 0;
	_output = 0;
}

float UPID::Constrain(float value) {
	if (value > _outputMax) {
		return _outputMax;
	} else if (value < _outputMin) {
		return _outputMin;
	} else {
		return value;
	}
}
