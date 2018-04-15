/*
 * UStream.cpp
 *
 *  Created on: 2017年9月29日
 *      Author: Romeli
 */

#include <Communication/UStream.h>
#include <cstring>

UStream::UStream(uint16_t rxBufSize) :
		_rxBuf(rxBufSize), _DMARxBuf(0), _txBuf(0), _txBuf2(0) {

	ReceivedEvent = nullptr;
	SendedEvent = nullptr;
	_receivedEventPool = nullptr;
	_sendedEventPool = nullptr;
}

UStream::UStream(uint16_t rxBufSize, uint16_t txBufSize, DMA_TypeDef* DMAx,
		DMA_Channel_TypeDef* DMAy_Channelx_Rx,
		DMA_Channel_TypeDef* DMAy_Channelx_Tx, UIT_Typedef& itDMARx,
		UIT_Typedef& itDMATx) :
		_rxBuf(rxBufSize), _DMARxBuf(rxBufSize), _txBuf(txBufSize), _txBuf2(
				txBufSize) {
}

UStream::~UStream() {
}

/*
 * author Romeli
 * explain 获取缓冲区内的缓冲字符数量
 * return uint16_t
 */
uint16_t UStream::Available() {
	return _rxBuf.Available();
}

/*
 * author Romeli
 * explain 通过流写单字节
 * param data 数据
 * param sync 是否是同步模式传输
 * return Status_Typedef
 */
Status_Typedef UStream::Write(uint8_t data, bool sync) {
	return Write(&data, 1);
}

/*
 * author Romeli
 * explain 通过流写字符串
 * param 字符串地址
 * return Status_Typedef
 */
Status_Typedef UStream::Print(uint8_t* str) {
	uint16_t len = GetLen((uint8_t *) str);
	if (len != 0) {
		return Write((uint8_t *) str, len);
	} else {
		UDebugOut("Error @Romeli 空数组");
		return Status_Error;
	}
}

/*
 * author Romeli
 * explain 通过流打印整形数
 * param1 num 整形数
 * param2 base 进制
 * return Status_Typedef
 */
Status_Typedef UStream::Print(int32_t num, uint8_t base) {
	uint8_t str[32];
	uint16_t len = ByNumber(num, base, str);
	return Write(str, len);
}

/*
 * author Romeli
 * explain 通过流打印浮点数
 * param1 flo 浮点数
 * param2 ndigit 小数位数
 * return Status_Typedef
 */
Status_Typedef UStream::Print(double flo, uint8_t ndigit) {
	uint8_t str[32];
	uint16_t len = ByNumber(flo, ndigit, str);
	return Write(str, len);
}

/*
 * author Romeli
 * explain 从流中读取指定数据长度
 * param data 读回数据存放数组
 * param sync 是否是同步读取
 * return Status_Typedef
 */
Status_Typedef UStream::Read(uint8_t* data, uint16_t len, bool sync) {
	//循环读取
	for (uint8_t i = 0; i < len; ++i) {
		Read(data + i);
	}
	return Status_Ok;
}

/*
 * author Romeli
 * explain 从流中读取一个字节
 * param data 读回数据存放位置
 * param sync 是否是同步读取
 * return Status_Typedef
 */
Status_Typedef UStream::Read(uint8_t* data, bool sync) {
	//读取一个数
	*data = _rxBuf.Data[_rxBuf.Start];
	_rxBuf.SpInc();
	return Status_Ok;
}

/*
 * author Romeli
 * explain 偷看一个字节（不移动缓冲指针）
 * param data 读回数据存放位置
 * return Status_Typedef
 */
Status_Typedef UStream::Peek(uint8_t* data) {
	//偷看一个数
	*data = _rxBuf.Data[_rxBuf.Start];
	return Status_Ok;
}

/*
 * author Romeli
 * explain 偷看下一个数字
 * param1 data 读取的数字存放位置
 * param2 ignore 忽略的字符
 * return Status_Typedef
 */
Status_Typedef UStream::PeekNextDigital(uint8_t *data, uint8_t ignore,
		bool detectDecimal) {
	//偷看一个数
	Peek(data);
	//当读到的字符为 '-','+','0'-'9','.'（detectDecimal为true）时返回
	if ((*data == '-') || (*data == '+') || ((*data >= '0') && (*data <= '9'))
			|| ((*data == '.') && detectDecimal) || (*data == ignore)) {
	} else {
		return Status_Error;
	}
	return Status_Ok;
}

/*
 * author Romeli
 * explain 查询当前发送流是否繁忙（应在派生类中重写）
 * return bool
 */
bool UStream::IsBusy() {
	return false;
}

/*
 * author Romeli
 * explain 设置事件触发时自动加入事件池
 * param1 receivedEvent ReceiveEvent的回调函数
 * param2 pool 触发时会加入的的事件池
 * return void
 */
void UStream::SetReceivedEventPool(UEvent receivedEvent, UEventPool& pool) {
	ReceivedEvent = receivedEvent;
	_receivedEventPool = &pool;
}

/*
 * author Romeli
 * explain 设置事件触发时自动加入事件池
 * param1 receivedEvent ReceiveEvent的回调函数
 * param2 pool 触发时会加入的的事件池
 * return void
 */
void UStream::SetSendedEventPool(UEvent sendEvent, UEventPool& pool) {
	SendedEvent = sendEvent;
	_sendedEventPool = &pool;
}

void UStream::Discard(uint16_t num) {
	_rxBuf.Discard(num);
}

/*
 * author Romeli
 * explain 计算字符串长度
 * param str 字符串地址
 * return uint16_t
 */
uint16_t UStream::GetLen(uint8_t * str) {
	uint16_t len = 0;
	for (len = 0; *(str + len) != '\0'; ++len)
		;
	return len;
}

/*
 * author Romeli
 * explain 从流中读取一个整形数
 * param num 读取的整数存放位置
 * param ignore 忽略的字符
 * return Status_Typedef 是否成功
 */
Status_Typedef UStream::nextInt(int64_t* num, uint8_t ignore) {
	bool firstChar = true;
	bool isNeg = false;
	uint8_t c = 0;
	uint16_t sp = _rxBuf.Start;
	int32_t n = 0;

	while (Available() > 0) {
		if (PeekNextDigital(&c, ignore, false) == Status_Ok) {
			//如果读到数字
			if (c == '-') {
				if (firstChar) {
					//检测到一个'-'
					isNeg = true;
					_rxBuf.SpInc();
					continue;
				} else {
					//'-'不是第一个数
					break;
				}
			} else if (c == '+') {
				if (firstChar) {
					//检测到一个'+'
					_rxBuf.SpInc();
					continue;
				} else {
					//'+'不是第一个数
					break;
				}
			} else if ((c == ignore) && (ignore != 0)) {
				_rxBuf.SpInc();
				continue;
			}
			n = n * 10 + c - '0';
			firstChar = false;
			_rxBuf.SpInc();
		} else {
			break;
		}
	}
	if ((sp != _rxBuf.Start) && (c != '-') && (c != ignore)) {
		//有读取到数
		if (isNeg) {
			n = -n;
		}
		*num = n;
		return Status_Ok;
	} else {
		//没有读取到数
		*num = 0;
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
Status_Typedef UStream::nextFloat(double* flo, uint8_t ignore) {
	double f = 0;
	double frac = 1.0;
	bool isNeg = false;
	bool isFra = false;
	bool firstChar = true;
	uint16_t sp = _rxBuf.Start;
	uint8_t c = 0;

	while (Available() > 0) {
		if (PeekNextDigital(&c, ignore, true) == Status_Ok) {
			if (c == '-') {
				if (firstChar) {
					//检测到一个'-'
					isNeg = true;
					_rxBuf.SpInc();
					continue;
				} else {
					//'-'不是第一个数
					break;
				}
			} else if (c == '+') {
				if (firstChar) {
					//检测到一个'+'
					_rxBuf.SpInc();
					continue;
				} else {
					//'-'不是第一个数
					break;
				}
			} else if ((c == ignore) && (ignore != 0)) {
				_rxBuf.SpInc();
				continue;
			} else if (c == '.') {
				if (isFra) { //不应出现两个'-'
					break;
				} else {
					if (!firstChar) {
						_rxBuf.SpInc();
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
			_rxBuf.SpInc();
			firstChar = false;
		} else {
			break;
		}
	}

	if ((sp != _rxBuf.Start) && (c != '-') && (c != ignore)) {
		//有读取到数
		f = isNeg ? -f : f;
		f = isFra ? f * frac : f;
		*flo = f;
		return Status_Ok;
	} else {
		//没有读取到数
		*flo = 0;
		return Status_Error;
	}
}

void UStream::RCCInit(DMA_TypeDef* DMAx) {
	uint32_t rcc = 0;
	if (DMAx == DMA1) {
		rcc = RCC_AHBPeriph_DMA1;
	} else if (DMAx == DMA2) {
		rcc = RCC_AHBPeriph_DMA1;
	} else {
		//不存在的DMA
		return;
	}
	RCC_AHBPeriphClockCmd(rcc, ENABLE);
}

/*
 * author Romeli
 * explain 根据DMA通道计算TC位
 * return uint32_t DMAy_IT_TCx
 */
uint32_t UStream::CalcTC(DMA_Channel_TypeDef* DMAy_Channelx) {
	uint32_t DMAy_IT_TCx = 0;
	if (DMAy_Channelx == DMA1_Channel1) {
		DMAy_IT_TCx = DMA1_IT_TC1;
	} else if (DMAy_Channelx == DMA1_Channel2) {
		DMAy_IT_TCx = DMA1_IT_TC2;
	} else if (DMAy_Channelx == DMA1_Channel3) {
		DMAy_IT_TCx = DMA1_IT_TC3;
	} else if (DMAy_Channelx == DMA1_Channel4) {
		DMAy_IT_TCx = DMA1_IT_TC4;
	} else if (DMAy_Channelx == DMA1_Channel5) {
		DMAy_IT_TCx = DMA1_IT_TC5;
	} else if (DMAy_Channelx == DMA1_Channel6) {
		DMAy_IT_TCx = DMA1_IT_TC6;
	} else if (DMAy_Channelx == DMA1_Channel7) {
		DMAy_IT_TCx = DMA1_IT_TC7;
	} else if (DMAy_Channelx == DMA2_Channel1) {
		DMAy_IT_TCx = DMA2_IT_TC1;
	} else if (DMAy_Channelx == DMA2_Channel2) {
		DMAy_IT_TCx = DMA2_IT_TC2;
	} else if (DMAy_Channelx == DMA2_Channel3) {
		DMAy_IT_TCx = DMA2_IT_TC3;
	} else if (DMAy_Channelx == DMA2_Channel4) {
		DMAy_IT_TCx = DMA2_IT_TC4;
	} else if (DMAy_Channelx == DMA2_Channel5) {
		DMAy_IT_TCx = DMA2_IT_TC5;
	}
	return DMAy_IT_TCx;
}

void UStream::IRQDMATx() {
	//关闭DMA发送
	_DMAy_Channelx_Tx->CCR &= (uint16_t) (~DMA_CCR1_EN);

	_DMAx->IFCR = _DMAy_IT_TCx_Tx;

	//判断当前使用的缓冲通道
	if (_DMAy_Channelx_Tx->CMAR == (uint32_t) _txBuf.Data) {
		//缓冲区1发送完成，置位指针
		_txBuf.End = 0;
		//判断缓冲区2是否有数据，并且忙标志未置位（防止填充到一半发送出去）
		if (_txBuf2.End != 0 && _txBuf2.Busy == false) {
			//当前使用缓冲区切换为缓冲区2，并加载DMA发送
			DMASend(&_txBuf2);
		} else {
			_DMAy_Channelx_Tx->CMAR = 0;
			//无数据需要发送，清除发送队列忙标志
			_DMATxBusy = false;
		}
	} else if (_DMAy_Channelx_Tx->CMAR == (uint32_t) _txBuf2.Data) {
		//缓冲区2发送完成，置位指针
		_txBuf2.End = 0;
		//判断缓冲区1是否有数据，并且忙标志未置位（防止填充到一半发送出去）
		if (_txBuf.End != 0 && _txBuf.Busy == false) {
			//当前使用缓冲区切换为缓冲区1，并加载DMA发送
			DMASend(&_txBuf);
		} else {
			_DMAy_Channelx_Tx->CMAR = 0;
			//无数据需要发送，清除发送队列忙标志
			_DMATxBusy = false;
		}
	} else {
		//可能是别的发送
		if (_txBuf2.End != 0 && _txBuf2.Busy == false) {
			//当前使用缓冲区切换为缓冲区2，并加载DMA发送
			DMASend(&_txBuf2);
		} else if (_txBuf.End != 0 && _txBuf.Busy == false) {
			//当前使用缓冲区切换为缓冲区1，并加载DMA发送
			DMASend(&_txBuf);
		} else {
			_DMAy_Channelx_Tx->CMAR = 0;
			//无数据需要发送，清除发送队列忙标志
			_DMATxBusy = false;
		}
	}
}

/*
 * author Romeli
 * explain 使用DMA发送数据（数据长度为使用的缓冲区的剩余空间大小）
 * param data 指向数据的指针的引用 NOTE @Romeli 这里使用的指针的引用，用于发送数据后移动指针位置
 * param len 数据长度的引用
 * param txBuf 使用的缓冲区的引用
 * return void
 */
void UStream::DMASend(uint8_t *&data, uint16_t &len) {
	UBuffer* txBuf;

	while (len != 0) {
		if ((_DMAy_Channelx_Tx->CMAR != (uint32_t) _txBuf.Data)
				&& (_txBuf.Size - _txBuf.End != 0)) {
			//若缓冲区1空闲，并且有空闲空间
			txBuf = &_txBuf;
		} else if ((_DMAy_Channelx_Tx->CMAR != (uint32_t) _txBuf2.Data)
				&& (_txBuf2.Size - _txBuf2.End != 0)) {
			//若缓冲区2空闲，并且有空闲空间
			txBuf = &_txBuf2;
		} else {
			//发送繁忙，两个缓冲区均在使用或已满
			//FIXME@romeli 需要添加超时返回代码
			txBuf = 0;
		}

		if (txBuf != 0) {
			//置位忙标志，防止计算中DMA自动加载发送缓冲
			txBuf->Busy = true;
			//计算缓冲区空闲空间大小
			uint16_t avaSize = uint16_t(txBuf->Size - txBuf->End);
			//计算可以发送的字节大小
			uint16_t copySize = avaSize < len ? avaSize : len;
			//拷贝字节到缓冲区
			memcpy(txBuf->Data + txBuf->End, data, copySize);
			//偏移发送缓冲区的末尾
			txBuf->End = uint16_t(txBuf->End + copySize);
			//偏移掉已发送字节
			data += copySize;
			//长度减去已发送长度
			len = uint16_t(len - copySize);

			if (!_DMATxBusy) {
				//DMA发送空闲，发送新的缓冲
				_DMATxBusy = true;
				DMASend(txBuf);
			}
			//解除忙标志
			txBuf->Busy = false;
		}
	}
}

void UStream::DMAReceive(uint8_t*& data, uint16_t& len) {
}

/*
 * author Romeli
 * explain 设置DMA发送的源地址并开始发送
 * param buffer 要发送的缓冲
 * return void
 */
void UStream::DMASend(UBuffer* buffer) {
	_DMAy_Channelx_Tx->CMAR = (uint32_t) buffer->Data;
	_DMAy_Channelx_Tx->CNDTR = buffer->End;
	_DMAy_Channelx_Tx->CCR |= DMA_CCR1_MINC;
	//使能DMA发送
	_DMAy_Channelx_Tx->CCR |= DMA_CCR1_EN;
}
