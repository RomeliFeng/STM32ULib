/*
 * UStream.cpp
 *
 *  Created on: 2017年9月29日
 *      Author: Romeli
 */

#include <Communication/UStream.h>
#include <cstring>

UStream::UStream(uint16_t rxBufSize) :
		_rxBuf(rxBufSize) {
	_mode = Mode_Normal;

	ReceivedEvent = nullptr;
	SendedEvent = nullptr;
	_receivedEventPool = nullptr;
	_sendedEventPool = nullptr;
}

UStream::~UStream() {
}

/*
 * author Romeli
 * explain 获取缓冲区内的缓冲字符数量
 * return uint16_t
 */
uint16_t UStream::Available() {
	return uint16_t(
			_rxBuf.Start <= _rxBuf.End ?
					_rxBuf.End - _rxBuf.Start :
					_rxBuf.Size - _rxBuf.Start + _rxBuf.End);
}

/*
 * author Romeli
 * explain 通过流写单字节
 * param data 数据
 * param sync 是否是同步模式传输
 * return Status_Typedef
 */
Status_Typedef UStream::Write(uint8_t data, bool sync, UEvent callBackEvent) {
	return Write(&data, 1, sync, callBackEvent);
}

/*
 * author Romeli
 * explain 从流中读取一个字节
 * param data 读回数据存放位置
 * param sync 是否是同步读取
 * return Status_Typedef
 */
Status_Typedef UStream::Read(uint8_t* data, bool sync, UEvent callBackEvent) {
	//读取一个数
	return Read(data, 1, sync, callBackEvent);
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
	if ((num != 0) && (num < Available())) {
		//如果丢弃的字节小于缓冲剩余字节数，丢弃相应的字节并退出
		_rxBuf.Start = (_rxBuf.Start + num) % _rxBuf.Size;
	} else {
		_rxBuf.Start = _rxBuf.End;
	}
}

/*
 * author Romeli
 * explain 通过流写字符串
 * param 字符串地址
 * return Status_Typedef
 */
Status_Typedef UStream::Print(uint8_t* str, bool sync) {
	uint16_t len = GetLen((uint8_t *) str);
	if (len != 0) {
		return Write((uint8_t *) str, len, sync);
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
Status_Typedef UStream::Print(int32_t num, uint8_t base, bool sync) {
	uint8_t str[32];
	uint16_t len = ByNumber(num, base, str);
	return Write(str, len, sync);
}

/*
 * author Romeli
 * explain 通过流打印浮点数
 * param1 flo 浮点数
 * param2 ndigit 小数位数
 * return Status_Typedef
 */
Status_Typedef UStream::Print(double flo, uint8_t ndigit, bool sync) {
	uint8_t str[32];
	uint16_t len = ByNumber(flo, ndigit, str);
	return Write(str, len, sync);
}

/*
 * author Romeli
 * explain 从流中读取一个整形数
 * param num 读取的整数存放位置
 * param ignore 忽略的字符
 * return Status_Typedef 是否成功
 */
Status_Typedef UStream::nextInt(int64_t& num, uint8_t ignore) {
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
					SpInc();
					continue;
				} else {
					//'-'不是第一个数
					break;
				}
			} else if (c == '+') {
				if (firstChar) {
					//检测到一个'+'
					SpInc();
					continue;
				} else {
					//'+'不是第一个数
					break;
				}
			} else if ((c == ignore) && (ignore != 0)) {
				SpInc();
				continue;
			}
			n = n * 10 + c - '0';
			firstChar = false;
			SpInc();
		} else {
			break;
		}
	}
	if ((sp != _rxBuf.Start) && (c != '-') && (c != ignore)) {
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
Status_Typedef UStream::nextFloat(double& flo, uint8_t ignore) {
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
					SpInc();
					continue;
				} else {
					//'-'不是第一个数
					break;
				}
			} else if (c == '+') {
				if (firstChar) {
					//检测到一个'+'
					SpInc();
					continue;
				} else {
					//'-'不是第一个数
					break;
				}
			} else if ((c == ignore) && (ignore != 0)) {
				SpInc();
				continue;
			} else if (c == '.') {
				if (isFra) { //不应出现两个'-'
					break;
				} else {
					if (!firstChar) {
						SpInc();
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
			SpInc();
			firstChar = false;
		} else {
			break;
		}
	}

	if ((sp != _rxBuf.Start) && (c != '-') && (c != ignore)) {
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

/*
 * author Romeli
 * explain 向下移动流指针
 * return void
 */
void UStream::SpInc() {
	if ((_rxBuf.Size != 0) && (_rxBuf.Start != _rxBuf.End)) {
		//缓冲区指针+1
		_rxBuf.Start = uint16_t((_rxBuf.Start + 1) % _rxBuf.Size);
	}
}
