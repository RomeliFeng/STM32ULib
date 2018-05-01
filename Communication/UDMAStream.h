/*
 * UDMAIO.h
 *
 *  Created on: 2018年4月14日
 *      Author: Romeli
 */

#ifndef COMMUNICATION_UDMASTREAMH_
#define COMMUNICATION_UDMASTREAMH_

#include <cmsis_device.h>
#include <Misc/UMisc.h>
#include <Communication/UStream.h>
#include <Communication/UBuffer.h>

class UDMAStream: public UStream {
public:
	UDMAStream(uint16_t rxBufSize);
	UDMAStream(uint16_t rxBufSize, uint16_t txBufSize, DMA_TypeDef* DMAx,
			DMA_Channel_TypeDef* DMAy_Channelx_Rx,
			DMA_Channel_TypeDef* DMAy_Channelx_Tx, UIT_Typedef& itDMARx,
			UIT_Typedef& itDMATx, uint32_t DMARxPeriphDR,
			uint32_t DMATxPeriphDR);
	virtual ~UDMAStream();

	virtual void IRQDMATx();
protected:
	UBuffer _DMARxBuf, _txBuf, _txBuf2;

	DMA_TypeDef* _DMAx;
	DMA_Channel_TypeDef* _DMAy_Channelx_Rx;
	DMA_Channel_TypeDef* _DMAy_Channelx_Tx;
	UIT_Typedef* _itDMARx, *_itDMATx;
	uint32_t _DMARxPeriphDR, _DMATxPeriphDR;

	uint32_t _DMAy_IT_TCx_Rx, _DMAy_IT_TCx_Tx;
	volatile bool _DMATxBusy;
	volatile bool _DMARxBusy;

	virtual void DMASend(uint8_t*& data, uint16_t& len);
	virtual void DMAReceive(uint8_t*& data, uint16_t& len);

	virtual void DMASend(UBuffer* buffer);

	void DMARCCInit(DMA_TypeDef* DMAx);
	uint32_t DMATCCalc(DMA_Channel_TypeDef* DMAy_Channelx);

	virtual void ITInit();
	virtual void DMAInit() = 0;
};

#endif /* COMMUNICATION_UDMASTREAMH_ */
