/*
 * UMisc.h
 *
 *  Created on: 27170108
 *      Author: Romeli
 */

#ifndef UMISC_H_
#define UMISC_H_

#include <cmsis_device.h>

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

struct Bit_Typedef {
	uint8_t bit0 :1;
	uint8_t bit1 :1;
	uint8_t bit2 :1;
	uint8_t bit3 :1;
	uint8_t bit4 :1;
	uint8_t bit5 :1;
	uint8_t bit6 :1;
	uint8_t bit7 :1;
};

union BytetoBit_Typedef {
	uint8_t byte;
	Bit_Typedef bit;
};

union WordtoByte_Typedef {
	uint8_t byte[2];
	uint16_t word;
};

union WordtoByteSigned_Typedef {
	uint8_t byte[2];
	int16_t word;
};

union TwoWordtoByte_Typedef {
	uint8_t byte[4];
	uint32_t twoword;
};

union TwoWordtoByteSigned_Typedef {
	uint8_t byte[4];
	int32_t twoword;
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

typedef void (*voidFun)(void);

#endif /* UMISC_H_ */
