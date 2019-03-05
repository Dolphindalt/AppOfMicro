#ifndef LCD_H_
#define LCD_H_

void lcd_init();

void lcd_display_on_off(char display_on, char cursor_on, char cursor_blink_on);

void lcd_set_ddram(char l);

void lcd_write_character(char c);

void lcd_delete_backward(char offset);

#endif
