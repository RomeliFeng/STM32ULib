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
		Mode_Diff, Mode_Post  //Post λ����
	};

	struct Param_Typedef {
		float Input;
		float Output;
		float SetPoint;
	};

	enum Dir_Typedef {
		Dir_Postive, Dir_Negtive
	};

	UPID(float kp, float ki, float kd, float interval, Dir_Typedef dir,
			Param_Typedef &UPIDParam, Mode_Typedef mode);
	void SetTunings(float kp, float ki, float kd);
	void SetInterval(float interval);
	void SetLimits(float min, float max);
	void SetDir(Dir_Typedef dir);
	void Compute();
	void Clear();

private:
	Param_Typedef &PIDParam;
	Mode_Typedef Mode;
	Dir_Typedef Dir;
	float Kp;
	float Ki;
	float Kd;
	float Last;
	float pError;
	float iTerm;
	float dTerm;
	float Min;
	float Max;
	float Interval;
};


#endif /* UPID_H_ */
