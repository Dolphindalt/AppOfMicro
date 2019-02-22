#ifndef LCD_H_
#define LCD_H_

void lcd_init();

void lcd_set_ddram(char address);

void lcd_write_characters(char *characters);

#endif
