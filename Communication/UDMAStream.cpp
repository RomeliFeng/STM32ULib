/*
 * UDMAStream.cpp
 *
 *  Created on: 2018年4月14日
 *      Author: Romeli
 */

#include <Communication/UDMAStream.h>

UDMAStream::UDMAStream(uint16_t rxBufSize) :
		UStream(rxBufSize), _DMARxBuf(0), _txBuf(0), _txBuf2(0) {
	_mode = Mode_Normal;
}

UDMAStream::UDMAStream(uint16_t rxBufSize, uint16_t txBufSize,
		DMA_TypeDef* DMAx, DMA_Channel_TypeDef* DMAy_Channelx_Rx,
		DMA_Channel_TypeDef* DMAy_Channelx_Tx, UIT_Typedef& itDMARx,
		UIT_Typedef& itDMATx, uint32_t DMARxPeriphDR, uint32_t DMATxPeriphDR) :
		UStream(rxBufSize), _DMARxBuf(rxBufSize), _txBuf(txBufSize), _txBuf2(
				txBufSize), _DMAx(DMAx), _DMAy_Channelx_Rx(DMAy_Channelx_Rx), _DMAy_Channelx_Tx(
				DMAy_Channelx_Tx), _itDMARx(&itDMARx), _itDMATx(&itDMATx), _DMARxPeriphDR(
				DMARxPeriphDR), _DMATxPeriphDR(DMATxPeriphDR) {
	_mode = Mode_DMA;

	_DMAy_IT_TCx_Rx = DMATCCalc(_DMAy_Channelx_Rx);
	_DMAy_IT_TCx_Tx = DMATCCalc(_DMAy_Channelx_Tx);

	_DMARxBusy = false;
	_DMATxBusy = false;
}

UDMAStream::~UDMAStream() {
}

void UDMAStream::IRQDMATx() {
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
	if (!_DMATxBusy) {
		if (WriteCallBackEvent != nullptr) {
			WriteCallBackEvent();
			WriteCallBackEvent = nullptr;
		}
	}
}

void UDMAStream::DMARCCInit(DMA_TypeDef* DMAx) {
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
uint32_t UDMAStream::DMATCCalc(DMA_Channel_TypeDef* DMAy_Channelx) {
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

/*
 * author Romeli
 * explain 使用DMA发送数据（数据长度为使用的缓冲区的剩余空间大小）
 * param data 指向数据的指针的引用 NOTE @Romeli 这里使用的指针的引用，用于发送数据后移动指针位置
 * param len 数据长度的引用
 * param txBuf 使用的缓冲区的引用
 * return void
 */
void UDMAStream::DMASend(uint8_t *&data, uint16_t &len) {
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
			//在发送缓冲中End指示数据剩余大小
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

void UDMAStream::DMAReceive(uint8_t*& data, uint16_t& len) {
}

/*
 * author Romeli
 * explain 设置DMA发送的源地址并开始发送
 * param buffer 要发送的缓冲
 * return void
 */
void UDMAStream::DMASend(UBuffer* buffer) {
	_DMAy_Channelx_Tx->CMAR = (uint32_t) buffer->Data;
	_DMAy_Channelx_Tx->CNDTR = buffer->End;
	_DMAy_Channelx_Tx->CCR |= DMA_CCR1_MINC;
	//使能DMA发送
	_DMAy_Channelx_Tx->CCR |= DMA_CCR1_EN;
}

void UDMAStream::ITInit() {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = _itDMARx->NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =
			_itDMARx->PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = _itDMARx->SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = _itDMATx->NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =
			_itDMATx->PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = _itDMATx->SubPriority;
	NVIC_Init(&NVIC_InitStructure);
}

void UDMAStream::DMAInit() {
	DMA_InitTypeDef DMA_InitStructure;

	//开启DMA时钟
	DMARCCInit(_DMAx);

	DMA_DeInit(_DMAy_Channelx_Rx);
	DMA_InitStructure.DMA_PeripheralBaseAddr = _DMARxPeriphDR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) _DMARxBuf.Data;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = _DMARxBuf.Size;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(_DMAy_Channelx_Rx, &DMA_InitStructure);

	DMA_DeInit(_DMAy_Channelx_Tx);
	DMA_InitStructure.DMA_PeripheralBaseAddr = _DMATxPeriphDR;
	DMA_InitStructure.DMA_MemoryBaseAddr = 0;				//临时设置，无效
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 0;				//临时设置，无效
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(_DMAy_Channelx_Tx, &DMA_InitStructure);
}
