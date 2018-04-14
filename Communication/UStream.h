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
#include <Event/UEventPool.h>

class UStream: public UConvert {
public:
	enum Mode_Typedef {
		Mode_Normal, Mode_DMA
	};

	enum Periph_Typedef {
		Periph_USART, Periph_SPI, Periph_I2C
	};

	struct Buffer_Typedef {
		Buffer_Typedef(uint16_t size) {
			this->size = size;
			if (this->size != 0) {
				this->data = new uint8_t[this->size];
			} else {
				this->data = nullptr;
			}
		}
		uint8_t* data;
		uint16_t size;
		volatile uint16_t start;
		volatile uint16_t end;
		volatile bool busy;
	};

	UEvent SendedEvent, ReceivedEvent;

	UStream(uint16_t rxBufSize, uint16_t txBufSize, uint16_t DMARxBufSize,
	        uint16_t txBuf2Size);
	virtual ~UStream();

	virtual Status_Typedef Write(uint8_t* data, uint16_t len,
	        bool sync = false) = 0;
	virtual Status_Typedef Write(uint8_t data, bool sync = false);

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

	virtual Status_Typedef Read(uint8_t* data, uint16_t len, bool sync = false);
	virtual Status_Typedef Read(uint8_t* data, bool sync = false);

	Status_Typedef Peek(uint8_t *data);
	Status_Typedef PeekNextDigital(uint8_t* data, uint8_t ignore,
	        bool detectDecimal = false);

	//使用模板实现多态接口
	template<typename T>
	inline Status_Typedef NextInt(T& num, uint8_t ignore = 0) {
		int64_t n;
		Status_Typedef status = nextInt(&n, ignore);
		num = n;
		return status;
	}
	template<typename T>
	inline Status_Typedef NextFloat(T& flo, uint8_t ignore = 0) {
		double f;
		Status_Typedef status = nextFloat(&f, ignore);
		flo = f;
		return status;
	}

	virtual uint16_t Available();
	virtual bool IsEmpty(Buffer_Typedef &buffer);
	virtual bool IsBusy();

	virtual void SetReceivedEventPool(UEvent receivedEvent, UEventPool &pool);
	virtual void SetSendedEventPool(UEvent sendEvent, UEventPool &pool);

	void Discard(uint16_t num = 0);

	virtual void IRQDMATx();
protected:
	Buffer_Typedef _rxBuf, _txBuf, _DMARxBuf, _txBuf2;
	Periph_Typedef _periph;
	Mode_Typedef _mode;
	UEventPool* _receivedEventPool;
	UEventPool* _sendedEventPool;

	DMA_TypeDef* _DMAx;
	DMA_Channel_TypeDef* _DMAy_Channelx_Rx;
	DMA_Channel_TypeDef* _DMAy_Channelx_Tx;
	uint32_t _DMAy_IT_TCx_Rx, _DMAy_IT_TCx_Tx;

	volatile bool _DMATxBusy = false;
	volatile bool _DMARxBusy = false;

	Status_Typedef SpInc(Buffer_Typedef &buffer);
	Status_Typedef SpDec(Buffer_Typedef &buffer);
	uint16_t GetLen(uint8_t* str);

	virtual void DMASend(uint8_t*& data, uint16_t& len);
	virtual void DMAReceive(uint8_t*& data, uint16_t& len);

	virtual void DMASend(Buffer_Typedef* buffer);
private:
	Status_Typedef nextInt(int64_t* num, uint8_t ignore = 0);
	Status_Typedef nextFloat(double* flo, uint8_t ignore = 0);
};

#endif /* USTEAM_H_ */
