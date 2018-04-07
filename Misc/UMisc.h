/*
 * UMisc.h
 *
 *  Created on: 27170108
 *      Author: Romeli
 */

#ifndef UMISC_H_
#define UMISC_H_

#include <cmsis_device.h>
#include <Misc/UMisc.h>
#include <functional>

//#define voidFun
//typedef void (*voidFun)();
typedef std::function<void(void)> UEvent;

//these function are work for high speed setting
inline void TIM_Enable(TIM_TypeDef* TIMx) {
	TIMx->CR1 |= TIM_CR1_CEN;
}

inline void TIM_Disable(TIM_TypeDef* TIMx) {
	TIMx->CR1 &= (uint16_t) (~((uint16_t) TIM_CR1_CEN));
}

inline void TIM_Clear_Update_Flag(TIM_TypeDef* TIMx) {
	TIMx->SR = (uint16_t) ~TIM_IT_Update;
}

inline void TIM_PSC_Reload(TIM_TypeDef* TIMx) {
	TIMx->EGR = TIM_PSCReloadMode_Immediate;
}

inline void TIM_Enable_IT_Update(TIM_TypeDef* TIMx) {
	TIMx->DIER |= TIM_IT_Update;
}

inline void TIM_Disable_IT_Update(TIM_TypeDef* TIMx) {
	TIMx->DIER &= (uint16_t) ~TIM_IT_Update;
}

struct Bit8_Typedef {
	uint8_t bit0 :1;
	uint8_t bit1 :1;
	uint8_t bit2 :1;
	uint8_t bit3 :1;
	uint8_t bit4 :1;
	uint8_t bit5 :1;
	uint8_t bit6 :1;
	uint8_t bit7 :1;
};

struct Bit16_Typedef {
	uint8_t bit0 :1;
	uint8_t bit1 :1;
	uint8_t bit2 :1;
	uint8_t bit3 :1;
	uint8_t bit4 :1;
	uint8_t bit5 :1;
	uint8_t bit6 :1;
	uint8_t bit7 :1;
	uint8_t bit8 :1;
	uint8_t bit9 :1;
	uint8_t bit10 :1;
	uint8_t bit11 :1;
	uint8_t bit12 :1;
	uint8_t bit13 :1;
	uint8_t bit14 :1;
	uint8_t bit15 :1;
};

struct Bit32_Typedef {
	uint8_t bit0 :1;
	uint8_t bit1 :1;
	uint8_t bit2 :1;
	uint8_t bit3 :1;
	uint8_t bit4 :1;
	uint8_t bit5 :1;
	uint8_t bit6 :1;
	uint8_t bit7 :1;
	uint8_t bit8 :1;
	uint8_t bit9 :1;
	uint8_t bit10 :1;
	uint8_t bit11 :1;
	uint8_t bit12 :1;
	uint8_t bit13 :1;
	uint8_t bit14 :1;
	uint8_t bit15 :1;
	uint8_t bit16 :1;
	uint8_t bit17 :1;
	uint8_t bit18 :1;
	uint8_t bit19 :1;
	uint8_t bit20 :1;
	uint8_t bit21 :1;
	uint8_t bit22 :1;
	uint8_t bit23 :1;
	uint8_t bit24 :1;
	uint8_t bit25 :1;
	uint8_t bit26 :1;
	uint8_t bit27 :1;
	uint8_t bit28 :1;
	uint8_t bit29 :1;
	uint8_t bit30 :1;
	uint8_t bit31 :1;
};

union BytetoBit_Typedef {
	uint8_t byte;
	Bit8_Typedef bit;
};

union WordtoBit_Typedef {
	uint16_t word;
	uint8_t byte[2];
	Bit16_Typedef bit;
};

union WordtoByte_Typedef {
	WordtoByte_Typedef(uint16_t& w) {
		word = w;
	}
	WordtoByte_Typedef() {

	}
	uint8_t byte[2];
	uint16_t word;
};

union WordtoByteSigned_Typedef {
	uint8_t byte[2];
	int16_t word;
};

union TwoWordtoByte_Typedef {
	uint8_t byte[4];
	uint16_t word[2];
	uint32_t twoWord;
};

union TwoWordtoByteSigned_Typedef {
	uint8_t byte[4];
	int16_t word[2];
	int32_t twoWord;
};

union TwoWordtoBit_Typedef {
	uint32_t twoWord;
	uint16_t word[2];
	uint8_t byte[4];
	Bit32_Typedef bit;
};

union DoubletoByte_Typedef {
	double d;
	uint8_t byte[8];
};

enum Status_Typedef {
	Status_Ok, Status_Error, Status_TimeOut
};

struct UIT_Typedef {
	uint8_t NVIC_IRQChannel;
	uint8_t PreemptionPriority;
	uint8_t SubPriority;
};

extern uint32_t CalcDMATC(DMA_Channel_TypeDef* DMAy_Channelx);


#endif /* UMISC_H_ */
