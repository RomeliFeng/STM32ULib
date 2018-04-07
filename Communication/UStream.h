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
		uint8_t* data;
		uint16_t size;
		volatile uint16_t start;
		volatile uint16_t end;
		volatile bool busy;
	};

	UEvent SendedEvent, ReceivedEvent;

	UStream(uint16_t rxBufSize, uint16_t txBufSize, uint16_t dmaRxBufSize,
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

	virtual Status_Typedef NextInt(int32_t* num, uint8_t ignore = 0);
	virtual Status_Typedef NextDouble(double* flo, uint8_t ignore = 0);

	virtual uint16_t Available();
	virtual bool IsEmpty(Buffer_Typedef &buffer);
	virtual bool IsBusy();

	virtual void SetReceivedEventPool(UEvent receivedEvent, UEventPool &pool);
	virtual void SetSendedEventPool(UEvent sendEvent, UEventPool &pool);

	void Discard(uint16_t num = 0);
protected:
	Buffer_Typedef _rxBuf, _txBuf, _dmaRxBuf, _txBuf2;
	Periph_Typedef _periph;
	Mode_Typedef _mode;
	UEventPool* _receivedEventPool;
	UEventPool* _sendedEventPool;

	Status_Typedef SpInc(Buffer_Typedef &buffer);
	Status_Typedef SpDec(Buffer_Typedef &buffer);
private:
	uint16_t getLen(uint8_t *str);
};

#endif /* USTEAM_H_ */
