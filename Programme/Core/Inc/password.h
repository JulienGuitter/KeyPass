/*
 * password.h
 *
 *  Created on: Jun 15, 2022
 *      Author: julie
 */

#ifndef INC_PASSWORD_H_
#define INC_PASSWORD_H_

#include "main.h"
#include <stdbool.h>

void generatePassword(char *str, uint8_t length, bool hasLetter, bool hasNumber, bool hasSpecialChar, bool hasMajuscule);
uint8_t getRandomNumber(uint8_t max);

void resetAllPassword();

#endif /* INC_PASSWORD_H_ */
