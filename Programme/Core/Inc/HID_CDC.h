/*
 * HID_CDC.h
 *
 *  Created on: Jun 12, 2022
 *      Author: julie
 */

#ifndef INC_HID_CDC_H_
#define INC_HID_CDC_H_

#include <stdbool.h>
#include "main.h"

#define CHAR_LEN    44
#define AZERTY      1
#define QWERTY      0

typedef struct{
    char lettre;
    int value;
}KeyValue;

void CDC_Send_Message(uint8_t *data);

#endif /* INC_HID_CDC_H_ */
