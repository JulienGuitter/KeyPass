/*
 * lcd_i2c.h
 *
 *  Created on: Jun 14, 2022
 *      Author: julie
 */

#ifndef INC_LCD_I2C_H_
#define INC_LCD_I2C_H_

void lcd_send_cmd (char cmd);

void lcd_send_data (char data);

void lcd_clear (void);

void lcd_put_cur(int row, int col);

void lcd_init (void);

void lcd_send_string (char *str);

void writeOnLCD(char *str1, char *str2);

#endif /* INC_LCD_I2C_H_ */

