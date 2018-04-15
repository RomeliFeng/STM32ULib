/*
 * UDMAIO.h
 *
 *  Created on: 2018年4月14日
 *      Author: Romeli
 */

#ifndef COMMUNICATION_UDMAIO_H_
#define COMMUNICATION_UDMAIO_H_

#include <cmsis_device.h>
#include <Misc/UMisc.h>

class UDMAIO {
public:
	UDMAIO();
	UDMAIO(uint16_t rxBufSize, uint16_t txBufSize, DMA_TypeDef* DMAx,
			DMA_Channel_TypeDef* DMAy_Channelx_Rx,
			DMA_Channel_TypeDef* DMAy_Channelx_Tx, UIT_Typedef& itDMARx,
			UIT_Typedef& itDMATx);
	virtual ~UDMAIO();





};

#endif /* COMMUNICATION_UDMAIO_H_ */
