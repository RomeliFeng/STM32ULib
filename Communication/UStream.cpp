/*
 * UStream.cpp
 *
 *  Created on: 2017年9月29日
 *      Author: Romeli
 */

#include <Communication/UStream.h>
#include <cstring>

UStream::UStream(uint16_t rxBufSize, uint16_t txBufSize, uint16_t dmaRxBufSize,
        uint16_t txBuf2Size) :
		_rxBuf(rxBufSize), _txBuf(txBuf2Size), _dmaRxBuf(dmaRxBufSize), _txBuf2(
		        txBuf2Size) {

	ReceivedEvent = nullptr;
	SendedEvent = nullptr;
	_receivedEventPool = nullptr;
	_sendedEventPool = nullptr;
}

UStream::~UStream() {
	delete[] _rxBuf.data;
	delete[] _txBuf.data;
	delete[] _dmaRxBuf.data;
	delete[] _txBuf2.data;
}

/*
 * author Romeli
 * explain 获取缓冲区内的缓冲字符数量
 * return uint16_t
 */
uint16_t UStream::Available() {
	return uint16_t(
	        _rxBuf.start <= _rxBuf.end ?
	                _rxBuf.end - _rxBuf.start :
	                _rxBuf.size - _rxBuf.start + _rxBuf.end);
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
	uint16_t len = byNumber(num, base, str);
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
	uint16_t len = byFloat(flo, ndigit, str);
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
	*data = _rxBuf.data[_rxBuf.start];
	return SpInc(_rxBuf);
}

/*
 * author Romeli
 * explain 偷看一个字节（不移动缓冲指针）
 * param data 读回数据存放位置
 * return Status_Typedef
 */
Status_Typedef UStream::Peek(uint8_t* data) {
	//偷看一个数
	*data = _rxBuf.data[_rxBuf.start];
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
 * explain 检查站内是否有数据
 * param buffer 栈地址
 * return bool
 */
inline bool UStream::IsEmpty(Buffer_Typedef &buffer) {
//判断缓冲区是否为空
	return buffer.start == buffer.end;
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

/*
 * author Romeli
 * explain 清空读取流内数据
 * return void
 */
void UStream::Discard(uint16_t num) {
	if (num != 0) {
		if (num < Available()) {
			//如果丢弃的字节小于缓冲剩余字节数，丢弃相应的字节并退出
			_rxBuf.start = (_rxBuf.start + num) % _rxBuf.size;
			return;
		}
	}
	_rxBuf.start = _rxBuf.end;
}

/*
 * author Romeli
 * explain 向下移动流指针
 * param1 buffer 流指针
 * return Status_Typedef
 */
inline Status_Typedef UStream::SpInc(Buffer_Typedef &buffer) {
	if (IsEmpty(buffer)) {
//缓冲区为空
		return Status_Error;
	} else {
//缓冲区指针+1
		buffer.start = uint16_t((buffer.start + 1) % buffer.size);
		return Status_Ok;
	}
}

/*
 * author Romeli
 * explain 向上移动流指针
 * param1 buffer 流指针
 * return Status_Typedef
 */
inline Status_Typedef UStream::SpDec(Buffer_Typedef &buffer) {
	buffer.start = uint16_t(buffer.start == 0 ? buffer.size : buffer.start - 1);
	return Status_Ok;
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
 * explain 使用DMA发送数据（数据长度为使用的缓冲区的剩余空间大小）
 * param data 指向数据的指针的引用 NOTE @Romeli 这里使用的指针的引用，用于发送数据后移动指针位置
 * param len 数据长度的引用
 * param txBuf 使用的缓冲区的引用
 * return void
 */
void UStream::DMASend(uint8_t *&data, uint16_t &len) {
	Buffer_Typedef* txBuf;

	while (len != 0) {
		if ((_DMAy_Channelx_Tx->CMAR != (uint32_t) _txBuf.data)
		        && (_txBuf.size - _txBuf.end != 0)) {
			//若缓冲区1空闲，并且有空闲空间
			txBuf = &_txBuf;
		} else if ((_DMAy_Channelx_Tx->CMAR != (uint32_t) _txBuf2.data)
		        && (_txBuf2.size - _txBuf2.end != 0)) {
			//若缓冲区2空闲，并且有空闲空间
			txBuf = &_txBuf2;
		} else {
			//发送繁忙，两个缓冲区均在使用或已满
			//FIXME@romeli 需要添加超时返回代码
			txBuf = 0;
		}

		if (txBuf != 0) {
			uint16_t avaSize, copySize;

			//置位忙标志，防止计算中DMA自动加载发送缓冲
			txBuf->busy = true;
			//计算缓冲区空闲空间大小
			avaSize = uint16_t(txBuf->size - txBuf->end);
			//计算可以发送的字节大小
			copySize = avaSize < len ? avaSize : len;
			//拷贝字节到缓冲区
			memcpy(txBuf->data + txBuf->end, data, copySize);
			//偏移发送缓冲区的末尾
			txBuf->end = uint16_t(txBuf->end + copySize);
			//偏移掉已发送字节
			data += copySize;
			//长度减去已发送长度
			len = uint16_t(len - copySize);

			if (!_DMABusy) {
				//DMA发送空闲，发送新的缓冲
				_DMABusy = true;

				//设置DMA地址
				_DMAy_Channelx_Tx->CMAR = (uint32_t) txBuf->data;
				_DMAy_Channelx_Tx->CNDTR = txBuf->end;

				//使能DMA开始发送
				_DMAy_Channelx_Tx->CCR |= DMA_CCR1_EN;
			}
			//解除忙标志
			txBuf->busy = false;
		}
	}
}

void UStream::DMAReceive(uint8_t*& data, uint16_t& len) {
}
