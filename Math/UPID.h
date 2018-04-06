/*
 * UPID.h
 *
 *  Created on: Aug 10, 2016
 *      Author: Romeli
 */

#ifndef UPID_H_
#define UPID_H_

class UPID {
public:
	enum Mode_Typedef {
		Mode_Normal, //普通模式
		Mode_PONM //过冲抑制模式
	};

	enum Dir_Typedef {
		Dir_Postive, Dir_Negtive
	};

	UPID(float& input, float& setPoint, float& output, float kp, float ki,
			float kd, float outputMin, float outputMax, float interval,
			Mode_Typedef mode = Mode_Normal, Dir_Typedef dir = Dir_Postive);
	void SetTunings(float kp, float ki, float kd);
	void SetInterval(float interval);
	void SetLimits(float min, float max);
	void SetDir(Dir_Typedef dir);
	void Compute();
	void Clear();

private:
	float& _input;
	float& _setPoint;
	float& _output;
	Mode_Typedef _mode;
	Dir_Typedef _dir;
	float _kp;
	float _ki;
	float _kd;
	float _lastInput;
	float _pError;
	float _iTerm;
	float _dError;
	float _outputMin;
	float _outputMax;
	float _interval;

	float Constrain(float value);
};

#endif /* UPID_H_ */
