/*
 * password.c
 *
 *  Created on: Jun 15, 2022
 *      Author: julie
 */
#include "password.h"
#include "w25qxx.h"

#define LENGTHALPHA			26
#define LENGTHNUMB			10
#define LENGTHSPECIALCHAR	7

extern ADC_HandleTypeDef hadc1;

void generatePassword(char *str, uint8_t length, bool hasLetter, bool hasNumber, bool hasSpecialChar, bool hasMajuscule){
	static listAlpha[LENGTHALPHA] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
	static listNumb[LENGTHNUMB] = {'0','1','2','3','4','5','6','7','8','9'};
	static listSpecialChar[LENGTHSPECIALCHAR] = {'-','_','&','$','!','ç','*'};
	if(length >= 100){
		length = 100;
	}
	uint8_t i = 0;
	while(i<length){
		switch(getRandomNumber(3)){
			case 0:{
				if(hasLetter){
					str[i] = (hasMajuscule && getRandomNumber(100) > 50) ? toupper(listAlpha[getRandomNumber(LENGTHALPHA)]) : listAlpha[getRandomNumber(LENGTHALPHA)];
					i++;
				}
				break;
			}
			case 1:{
				if(hasNumber){
					str[i] = listNumb[getRandomNumber(LENGTHNUMB)];
					i++;
				}
				break;
			}
			case 2:{
				if(hasSpecialChar){
					str[i] = listSpecialChar[getRandomNumber(LENGTHSPECIALCHAR)];
					i++;
				}
				break;
			}
		}
	}
}

uint8_t getRandomNumber(uint8_t max){
	uint8_t random = 0;
	 // Start ADC Conversion
	HAL_ADC_Start(&hadc1);
	// Poll ADC1 Perihperal & TimeOut = 1mSec
	HAL_ADC_PollForConversion(&hadc1, 1);
	// Read The ADC Conversion Result & Map It To PWM DutyCycle
	random = HAL_ADC_GetValue(&hadc1)%max;
	return random;
}

void resetAllPassword(){
	uint8_t tempPassBuff[1] = {0};
	W25qxx_EraseBlock(1);
	W25qxx_WriteBlock(tempPassBuff, 1, 0, 1);
}

