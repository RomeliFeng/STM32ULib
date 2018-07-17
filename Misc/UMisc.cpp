/*
 * Umisc.cpp
 *
 *  Created on: Apr 7, 2018
 *      Author: Romeli
 */

#include <Misc/UMisc.h>

#ifdef USERLIB_F1
/*
 * author Romeli
 * explain 根据DMA通道计算TC位
 * return uint32_t DMAy_IT_TCx
 */
uint32_t CalcDMATC(DMA_Channel_TypeDef* DMAy_Channelx) {
	if (DMAy_Channelx == DMA1_Channel1) {
		return (uint32_t) DMA1_IT_TC1;
	} else if (DMAy_Channelx == DMA1_Channel2) {
		return (uint32_t) DMA1_IT_TC2;
	} else if (DMAy_Channelx == DMA1_Channel3) {
		return (uint32_t) DMA1_IT_TC3;
	} else if (DMAy_Channelx == DMA1_Channel4) {
		return (uint32_t) DMA1_IT_TC4;
	} else if (DMAy_Channelx == DMA1_Channel5) {
		return (uint32_t) DMA1_IT_TC5;
	} else if (DMAy_Channelx == DMA1_Channel6) {
		return (uint32_t) DMA1_IT_TC6;
	} else if (DMAy_Channelx == DMA1_Channel7) {
		return (uint32_t) DMA1_IT_TC7;
	} else if (DMAy_Channelx == DMA2_Channel1) {
		return (uint32_t) DMA2_IT_TC1;
	} else if (DMAy_Channelx == DMA2_Channel2) {
		return (uint32_t) DMA2_IT_TC2;
	} else if (DMAy_Channelx == DMA2_Channel3) {
		return (uint32_t) DMA2_IT_TC3;
	} else if (DMAy_Channelx == DMA2_Channel4) {
		return (uint32_t) DMA2_IT_TC4;
	} else if (DMAy_Channelx == DMA2_Channel5) {
		return (uint32_t) DMA2_IT_TC5;
	} else {
		return 0;
	}
}
#endif
