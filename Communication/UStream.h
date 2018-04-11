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

	/*
	 * author Romeli
	 * explain 从流中读取一个整形数
	 * param num 读取的整数存放位置
	 * param ignore 忽略的字符
	 * return Status_Typedef 是否成功
	 */
	template<typename T>
	Status_Typedef NextInt(T& num, uint8_t ignore = 0) {
		bool firstChar = true;
		bool isNeg = false;
		uint8_t c = 0;
		uint16_t sp = _rxBuf.start;
		int32_t n = 0;

		while (Available() > 0) {
			if (PeekNextDigital(&c, ignore, false) == Status_Ok) {
				//如果读到数字
				if (c == '-') {
					if (firstChar) {
						//检测到一个'-'
						isNeg = true;
						SpInc(_rxBuf);
						continue;
					} else {
						//'-'不是第一个数
						break;
					}
				} else if (c == '+') {
					if (firstChar) {
						//检测到一个'+'
						SpInc(_rxBuf);
						continue;
					} else {
						//'+'不是第一个数
						break;
					}
				} else if ((c == ignore) && (ignore != 0)) {
					SpInc(_rxBuf);
					continue;
				}
				n = n * 10 + c - '0';
				firstChar = false;
				SpInc(_rxBuf);
			} else {
				break;
			}
		}
		if ((sp != _rxBuf.start) && (c != '-') && (c != ignore)) {
			//有读取到数
			if (isNeg) {
				n = -n;
			}
			num = n;
			return Status_Ok;
		} else {
			//没有读取到数
			num = 0;
			return Status_Error;
		}
	}

	/*
	 * author Romeli
	 * explain 从流中读取一个浮点数
	 * param flo 读取的浮点数存放位置
	 * param ignore 忽略的字符
	 * return Status_Typedef 是否成功
	 */
	template<typename T>
	Status_Typedef NextDouble(T flo, uint8_t ignore = 0) {
		double f = 0;
		double frac = 1.0;
		bool isNeg = false;
		bool isFra = false;
		bool firstChar = true;
		uint16_t sp = _rxBuf.start;
		uint8_t c = 0;

		while (Available() > 0) {
			if (PeekNextDigital(&c, ignore, true) == Status_Ok) {
				if (c == '-') {
					if (firstChar) {
						//检测到一个'-'
						isNeg = true;
						SpInc(_rxBuf);
						continue;
					} else {
						//'-'不是第一个数
						break;
					}
				} else if (c == '+') {
					if (firstChar) {
						//检测到一个'+'
						SpInc(_rxBuf);
						continue;
					} else {
						//'-'不是第一个数
						break;
					}
				} else if ((c == ignore) && (ignore != 0)) {
					SpInc(_rxBuf);
					continue;
				} else if (c == '.') {
					if (isFra) { //不应出现两个'-'
						break;
					} else {
						if (!firstChar) {
							SpInc(_rxBuf);
							isFra = true;
							continue;
						} else {
							//第一个字符为点的时候
							break;
						}
					}
				}
				if (isFra) {
					frac *= 0.1;
				}
				f = f * 10 + c - '0';
				SpInc(_rxBuf);
				firstChar = false;
			} else {
				break;
			}
		}

		if ((sp != _rxBuf.start) && (c != '-') && (c != ignore)) {
			//有读取到数
			f = isNeg ? -f : f;
			f = isFra ? f * frac : f;
			flo = f;
			return Status_Ok;
		} else {
			//没有读取到数
			flo = 0;
			return Status_Error;
		}
	}

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

	DMA_TypeDef* _DMAx;
	DMA_Channel_TypeDef* _DMAy_Channelx_Rx;
	DMA_Channel_TypeDef* _DMAy_Channelx_Tx;
	uint32_t _DMAy_IT_TCx_Rx, _DMAy_IT_TCx_Tx;

	volatile bool _DMABusy = false;

	Status_Typedef SpInc(Buffer_Typedef &buffer);
	Status_Typedef SpDec(Buffer_Typedef &buffer);
	uint16_t GetLen(uint8_t *str);

	void DMASend(uint8_t *&data, uint16_t &len);
	void DMAReceive(uint8_t *&data, uint16_t &len);
private:
};

#endif /* USTEAM_H_ */
