/*
 * UStream.h
 *
 *  Created on: 2017年9月29日
 *      Author: Romeli
 */

#ifndef USTEAM_H_
#define USTEAM_H_

#include <cmsis_device.h>
#include <Misc/UMisc.h>
#include <Tool/UConvert.h>
#include <Communication/UBuffer.h>
#include <Misc/UDebug.h>
#include <Event/UEventPool.h>

class UStream: public UConvert {
public:
	enum Mode_Typedef {
		Mode_Normal, Mode_DMA
	};

	enum Periph_Typedef {
		Periph_USART, Periph_SPI, Periph_I2C
	};

	UEvent ReceivedEvent = nullptr, SendedEvent = nullptr;
	//CallBack函数由于不是操作系统实现，将在中断中被调用，严禁执行耗时操作！！！
	UEvent WriteCallBackEvent = nullptr, ReadCallBackEvent = nullptr;

	UStream(uint16_t rxBufSize);
	virtual ~UStream();

	virtual Status_Typedef Write(uint8_t* data, uint16_t len, bool sync = false,
			UEvent callBackEvent = nullptr) = 0;
	virtual Status_Typedef Read(uint8_t* data, uint16_t len, bool sync = false,
			UEvent callBackEvent = nullptr) = 0;
	virtual Status_Typedef Write(uint8_t data, bool sync = false,
			UEvent callBackEvent = nullptr);
	virtual Status_Typedef Read(uint8_t* data, bool sync = false,
			UEvent callBackEvent = nullptr);

	Status_Typedef Peek(uint8_t *data);
	Status_Typedef PeekNextDigital(uint8_t* data, uint8_t ignore,
			bool detectDecimal = false);

	//使用模板实现多态接口
	template<typename T>
	inline Status_Typedef NextInt(T& num, uint8_t ignore = 0) {
		int64_t n;
		Status_Typedef status = nextInt(n, ignore);
		num = n;
		return status;
	}
	template<typename T>
	inline Status_Typedef NextFloat(T& flo, uint8_t ignore = 0) {
		double f;
		Status_Typedef status = nextFloat(f, ignore);
		flo = f;
		return status;
	}

	virtual uint16_t Available();
	virtual bool IsBusy();

	virtual void SetReceivedEventPool(UEvent receivedEvent, UEventPool &pool);
	virtual void SetSendedEventPool(UEvent sendEvent, UEventPool &pool);

	void Discard(uint16_t num = 0);

	Status_Typedef Print(uint8_t* str, bool sync = false);
	inline Status_Typedef Print(const char* str, bool sync = false) {
		return Print((uint8_t*) str, sync);
	}
	Status_Typedef Print(int32_t num, uint8_t base = 10, bool sync = false);
	inline Status_Typedef Print(int16_t num, uint8_t base = 10, bool sync =
			false) {
		return Print((int32_t) num, base, sync);
	}
	inline Status_Typedef Print(int8_t num, uint8_t base = 10,
			bool sync = false) {
		return Print((int32_t) num, base, sync);
	}
	inline Status_Typedef Print(uint32_t num, uint8_t base = 10, bool sync =
			false) {
		return Print((int32_t) num, base, sync);
	}
	inline Status_Typedef Print(uint16_t num, uint8_t base = 10, bool sync =
			false) {
		return Print((int32_t) num, base, sync);
	}
	inline Status_Typedef Print(uint8_t num, uint8_t base = 10, bool sync =
			false) {
		return Print((int32_t) num, base, sync);
	}
	inline Status_Typedef Print(int num, uint8_t base = 10, bool sync = false) {
		return Print((int32_t) num, base, sync);
	}
	inline Status_Typedef Print(unsigned int num, uint8_t base = 10, bool sync =
			false) {
		return Print((int32_t) num, base, sync);
	}

	Status_Typedef Print(double flo, uint8_t ndigit = 2, bool sync = false);
	inline Status_Typedef Print(float flo, uint8_t ndigit = 2,
			bool sync = false) {
		return Print((double) flo, ndigit, sync);
	}

protected:
	UBuffer _rxBuf;

	Periph_Typedef _periph;
	Mode_Typedef _mode;
	UEventPool* _receivedEventPool;
	UEventPool* _sendedEventPool;
private:
	Status_Typedef nextInt(int64_t& num, uint8_t ignore = 0);
	Status_Typedef nextFloat(double& flo, uint8_t ignore = 0);
	void SpInc();
};

#endif /* USTEAM_H_ */
