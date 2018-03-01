/*
 * UStream.h
 *
 *  Created on: 2017年9月29日
 *      Author: Romeli
 */

#ifndef USTEAM_H_
#define USTEAM_H_

#include <cmsis_device.h>
#include <Tool/UConvert.h>
#include <Misc/UDebug.h>
#include <Misc/UMisc.h>

class UStream: public UConvert {
public:
	struct Buffer_Typedef {
		uint8_t* data;
		uint16_t size;
		volatile uint16_t start;
		volatile uint16_t end;
		volatile bool busy;
	};

	UStream(uint16_t rxBufSize, uint16_t txBufSize) {
		_RxBuf.size = rxBufSize;
		_RxBuf.data = new uint8_t[_RxBuf.size];
		_RxBuf.start = 0;
		_RxBuf.end = 0;
		_RxBuf.busy = false;

		_TxBuf.size = txBufSize;
		_TxBuf.data = new uint8_t[_TxBuf.size];
		_TxBuf.start = 0;
		_TxBuf.end = 0;
		_TxBuf.busy = false;
	}

	virtual ~UStream() {
		delete[] _RxBuf.data;
		delete[]  _TxBuf.data;
	}

	//接口
	/*
	 * author Romeli
	 * explain 通过流写数组
	 * param1 data 数组的首地址
	 * param2 len 数组的长度
	 * return Status_Typedef
	 */
	virtual Status_Typedef Write(uint8_t* data, uint16_t len) = 0;
	//虚函数
	virtual Status_Typedef Write(uint8_t data);

	Status_Typedef Print(uint8_t* str);
	inline Status_Typedef Print(const char* str) {
		return Print((uint8_t*) str);
	}
	Status_Typedef Print(int32_t num, uint8_t base = 10);
	inline Status_Typedef Print(int16_t num, uint8_t base = 10) {
		return Print((int32_t) num, base);
	}
	inline Status_Typedef Print(int8_t num, uint8_t base = 10) {
		return Print((int32_t) num, base);
	}
	inline Status_Typedef Print(uint32_t num, uint8_t base = 10) {
		return Print((int32_t) num, base);
	}
	inline Status_Typedef Print(uint16_t num, uint8_t base = 10) {
		return Print((int32_t) num, base);
	}
	inline Status_Typedef Print(uint8_t num, uint8_t base = 10) {
		return Print((int32_t) num, base);
	}
	inline Status_Typedef Print(int num, uint8_t base = 10) {
		return Print((int32_t) num, base);
	}
	inline Status_Typedef Print(unsigned int num, uint8_t base = 10) {
		return Print((int32_t) num, base);
	}

	Status_Typedef Print(double flo, uint8_t ndigit = 2);
	inline Status_Typedef Print(float flo, uint8_t ndigit = 2) {
		return Print((double) flo, ndigit);
	}

	virtual Status_Typedef Read(uint8_t* data, uint16_t len);
	virtual Status_Typedef Read(uint8_t* data);

	Status_Typedef Peek(uint8_t *data);
	Status_Typedef PeekNextDigital(uint8_t* data, uint8_t ignore,
			bool detectDecimal = false);

	virtual Status_Typedef NextInt(void* num, uint8_t ignore = 0);
	virtual Status_Typedef NextFloat(void* flo, uint8_t ignore = 0);

	virtual uint16_t Available();
	virtual bool IsEmpty(Buffer_Typedef &buffer);
	virtual bool IsBusy();
	void Clear();
protected:
	Buffer_Typedef _RxBuf, _TxBuf;
	Status_Typedef SpInc(Buffer_Typedef &buffer);
	Status_Typedef SpDec(Buffer_Typedef &buffer);
private:
	uint16_t getLen(uint8_t *str);
};

#endif /* USTEAM_H_ */
