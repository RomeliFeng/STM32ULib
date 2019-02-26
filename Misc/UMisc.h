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
#include <cstring>
#include <functional>

#define TIM_IT_CCx (TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4)

//#define voidFun
//typedef void (*voidFun)();
typedef std::function<void(void)> UEvent;

//these function are work for high speed setting
inline void TIM_Enable(TIM_TypeDef* TIMx) {
	TIMx->CR1 |= uint16_t(TIM_CR1_CEN);
}

inline void TIM_Disable(TIM_TypeDef* TIMx) {
	TIMx->CR1 &= uint16_t(~((uint16_t) TIM_CR1_CEN));
}

inline void TIM_Clear_Update_Flag(TIM_TypeDef* TIMx) {
	TIMx->SR = uint16_t(~TIM_IT_Update);
}

inline void TIM_Clear_CC1_Flag(TIM_TypeDef* TIMx) {
	TIMx->SR = uint16_t(~TIM_IT_CC1);
}

inline void TIM_Clear_CC2_Flag(TIM_TypeDef* TIMx) {
	TIMx->SR = uint16_t(~TIM_IT_CC2);
}

inline void TIM_Clear_CC3_Flag(TIM_TypeDef* TIMx) {
	TIMx->SR = uint16_t(~TIM_IT_CC3);
}

inline void TIM_Clear_CC4_Flag(TIM_TypeDef* TIMx) {
	TIMx->SR = uint16_t(~TIM_IT_CC4);
}

inline void TIM_Clear_CCx_Flag(TIM_TypeDef* TIMx) {
	TIMx->SR = uint16_t(~TIM_IT_CCx);
}

inline void TIM_PSC_Reload(TIM_TypeDef* TIMx) {
	TIMx->EGR = uint16_t(TIM_PSCReloadMode_Immediate);
}

inline void TIM_Enable_IT_Update(TIM_TypeDef* TIMx) {
	TIMx->DIER |= uint16_t(TIM_IT_Update);
}

inline void TIM_Enable_IT_CCx(TIM_TypeDef* TIMx) {
	TIMx->DIER |= uint16_t(TIM_IT_CCx);
}

inline void TIM_Disable_IT_Update(TIM_TypeDef* TIMx) {
	TIMx->DIER &= uint16_t(~TIM_IT_Update);
}

inline void TIM_Disable_IT_CCx(TIM_TypeDef* TIMx) {
	TIMx->DIER &= uint16_t(~TIM_IT_CCx);
}

inline bool TIM_Get_IT_Update(TIM_TypeDef* TIMx) {
	return ((TIMx->SR & TIM_IT_Update) != 0);
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

union Byte_Typedef {
	Byte_Typedef(uint8_t b) {
		byte = b;
	}
	Byte_Typedef() {
	}
	uint8_t byte;
	Bit8_Typedef bit;
};

union HalfWord_Typedef {
	HalfWord_Typedef(uint16_t hW) {
		halfWord = hW;
	}
	HalfWord_Typedef(uint8_t* pb) {
		memcpy(this->byte, pb, sizeof(this));
	}
	HalfWord_Typedef() {
	}
	uint8_t byte[2];
	uint16_t halfWord;
	Bit16_Typedef bit;
};

union HalfWordSigned_Typedef {
	HalfWordSigned_Typedef(int16_t hW) {
		halfWord = hW;
	}
	HalfWordSigned_Typedef(uint8_t* pb) {
		memcpy(this->byte, pb, sizeof(this));
	}
	HalfWordSigned_Typedef() {
	}
	uint8_t byte[2];
	int16_t halfWord;
	Bit16_Typedef bit;
};

union Word_Typedef {
	Word_Typedef(uint32_t w) {
		word = w;
	}
	Word_Typedef(uint8_t* pb) {
		memcpy(this->byte, pb, sizeof(this));
	}
	Word_Typedef() {
	}
	uint8_t byte[4];
	uint16_t halfWord[2];
	uint32_t word;
	Bit32_Typedef bit;
};

union WordSigned_Typedef {
	WordSigned_Typedef(int32_t w) {
		word = w;
	}
	WordSigned_Typedef(uint8_t* pb) {
		memcpy(this->byte, pb, sizeof(this));
	}
	WordSigned_Typedef() {
	}
	uint8_t byte[4];
	int16_t halfWord[2];
	int32_t word;
	Bit32_Typedef bit;
};

union Double_Typedef {
	Double_Typedef(double d) {
		this->d = d;
	}
	Double_Typedef(uint8_t* pb) {
		memcpy(this->byte, pb, sizeof(this));
	}
	Double_Typedef() {
	}
	double d;
	uint8_t byte[sizeof(double)];
};

union Float_Typedef {
	Float_Typedef(float f) {
		this->f = f;
	}
	Float_Typedef(uint8_t* pb) {
		memcpy(this->byte, pb, sizeof(this));
	}
	Float_Typedef() {
	}
	float f;
	uint8_t byte[sizeof(float)];
};

enum Status_Typedef {
	Status_Ok, Status_Error, Status_TimeOut
};

struct UIT_Typedef {
	uint8_t NVIC_IRQChannel;
	uint8_t PreemptionPriority;
	uint8_t SubPriority;
};

#ifdef USERLIB_F1
extern uint32_t CalcDMATC(DMA_Channel_TypeDef* DMAy_Channelx);
#endif

#endif /* UMISC_H_ */
