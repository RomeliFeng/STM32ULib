/*
 * UPID.h
 *
 *  Created on: Aug 10, 2016
 *      Author: Romeli
 */

#ifndef UPID_H_
#define UPID_H_

template<typename TInput, typename TOutput>
class UPID {
public:
	enum Mode_Typedef {
		Mode_Normal, //普通模式
		Mode_PONM //过冲抑制模式
	};

	enum Dir_Typedef {
		Dir_Postive, Dir_Negtive
	};

	UPID(TInput& input, TInput& setPoint, TOutput& output, TOutput outputMin,
			TOutput outputMax, float kp, float ki, float kd, float interval,
			Mode_Typedef mode = Mode_Normal, Dir_Typedef dir = Dir_Postive) :
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
	void Compute() {
		float input = _input;

		//计算误差和误差的倒数
		_pError = _setPoint - input;
		_dError = input - _lastInput;

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
		_lastInput = input;
	}

	void SetLimits(TOutput min, TOutput max) {
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

	void SetTunings(float kp, float ki, float kd) {
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

	void SetInterval(float interval) {
		if (interval < 0) {
			return;
		}
		float ratio = interval / _interval;
		_ki *= ratio;
		_kd /= ratio;
		_interval = interval;
	}

	void SetDir(Dir_Typedef dir) {
		if (_dir != dir) {
			_kp = 0 - _kp;
			_ki = 0 - _ki;
			_kd = 0 - _kd;
			_dir = dir;
		}
	}

	void Clear() {
		_iTerm = 0;
		_output = 0;
	}

private:
	TInput& _input;
	TInput& _setPoint;
	TOutput& _output;
	Mode_Typedef _mode;
	Dir_Typedef _dir;
	float _kp;
	float _ki;
	float _kd;
	TInput _lastInput;
	float _pError;
	float _iTerm;
	float _dError;
	TOutput _outputMin;
	TOutput _outputMax;
	float _interval;

	float Constrain(float value) {
		if (value > _outputMax) {
			return _outputMax;
		} else if (value < _outputMin) {
			return _outputMin;
		} else {
			return value;
		}
	}
};

#endif /* UPID_H_ */
