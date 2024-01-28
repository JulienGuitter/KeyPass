/*
 * HID_CDC.c
 *
 *  Created on: Jun 12, 2022
 *      Author: julie
 */

#include "HID_CDC.h"
#include "usbd_def.h"
#include "usb_device.h"

extern USBD_HandleTypeDef hUsbDevice;

typedef struct
{
	uint8_t MODIFIER;
	uint8_t RESERVED;
	uint8_t KEYCODE1;
	uint8_t KEYCODE2;
	uint8_t KEYCODE3;
	uint8_t KEYCODE4;
	uint8_t KEYCODE5;
	uint8_t KEYCODE6;
} keyboardHID;

keyboardHID keyboardhid = {0,0,0,0,0,0,0,0};

KeyValue listCharactere[CHAR_LEN] = {{'a', 0x14},
                                     {'b', 0x05},
                                     {'c', 0x06},
                                     {'d', 0x07},
                                     {'e', 0x08},
                                     {'f', 0x09},
                                     {'g', 0x0A},
                                     {'h', 0x0B},
                                     {'i', 0x0C},
                                     {'j', 0x0D},
                                     {'k', 0x0E},
                                     {'l', 0x0F},
                                     {'m', 0x33},
                                     {'n', 0x11},
                                     {'o', 0x12},
                                     {'p', 0x13},
                                     {'q', 0x04},
                                     {'r', 0x15},
                                     {'s', 0x16},
                                     {'t', 0x17},
                                     {'u', 0x18},
                                     {'v', 0x19},
                                     {'w', 0x1D},
                                     {'x', 0x1B},
                                     {'y', 0x1C},
                                     {'z', 0x1A},
                                     {'0', 0x27},
                                     {'1', 0x1E},
                                     {'2', 0x1F},
                                     {'3', 0x20},
                                     {'4', 0x21},
                                     {'5', 0x22},
                                     {'6', 0x23},
                                     {'7', 0x24},
                                     {'8', 0x25},
                                     {'9', 0x26},
                                     {'-', 0x23},
                                     {'_', 0x25},
                                     {'&', 0x1E},
                                     {'$', 0x30},
                                     {'!', 0x38},
                                     {'ç', 0x26},
                                     {'*', 0x31},
                                     {' ', 0x2C}};

void CDC_Send_Message(uint8_t *data){
	CDC_Transmit(0, data, strlen(data));
}

void writePasswordHID(char *str){
	for(int i=0; str[i] ; i++){
		for(int j=0; j<CHAR_LEN; j++){
			if(str[i] == listCharactere[j].lettre || (65 <= str[i] && str[i] <= 90 && str[i]+32 == listCharactere[j].lettre)){
				if((65 <= str[i] && str[i] <= 90) || (48 <= listCharactere[j].lettre && listCharactere[j].lettre <= 57)){
					keyboardhid.MODIFIER = 0x02;  // left Shift
				}else{
					keyboardhid.MODIFIER = 0x00;
				}
				keyboardhid.KEYCODE1 = listCharactere[j].value;  // press 'a'
				USBD_HID_Keybaord_SendReport(&hUsbDevice, &keyboardhid, sizeof (keyboardhid));
				HAL_Delay (50);
			}
		}
	}
	keyboardhid.MODIFIER = 0x00;  // left Shift
	keyboardhid.KEYCODE1 = 0x00;  // press 'a'
	USBD_HID_Keybaord_SendReport(&hUsbDevice, &keyboardhid, sizeof (keyboardhid));
	HAL_Delay (50);
}
