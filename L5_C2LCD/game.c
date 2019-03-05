#include "game.h"
#include "lcd.h"
#include "spi.h"
#include "keypad.h"

#define WALL 0x28
#define PLAYER 0x87

unsigned int points = 0;
char dead = 0;
char victory = 0;
char player_position = 0;
char obstacles[2][16];
char freq1 = 0;
char freq2 = 10;

int primitive_to_buffer(char *buffer, unsigned int buffer_size, int primitive);

static void move_character_up()
{
    if(player_position ^ 0)
    {
        player_position = 0;
        lcd_set_ddram(0x00);
        lcd_write_character(0x00);
        lcd_set_ddram(0x40);
        lcd_write_character(0x00);
    }
}

static void move_character_down()
{
    if(player_position ^ 1)
    {
        player_position = 1;
        lcd_set_ddram(0x40);
        lcd_write_character(0x00);
        lcd_set_ddram(0x00);
        lcd_write_character(0x00);
    }
}

static void chance_obstacles()
{
    freq1++;
    freq2++;
    if(freq1 == 12)
    {
        obstacles[0][15] = WALL;
        freq1 = 0;
    }
    else if(freq2 > 40)
    {
        obstacles[1][15] = WALL;
        freq2 = 1;
    }
}

static void try_kill_player()
{
    if((player_position == 0 && obstacles[0][0] == WALL) ||
            (player_position == 1 && obstacles[1][0] == WALL))
    {
        dead = 1;
    }
}

static void shift_obstacles()
{
    char i;
    for(i = 0; i < 15; i++)
    {
        if(obstacles[0][i+1] == WALL)
        {
            lcd_set_ddram(i+1);
            lcd_write_character(0x00);
            lcd_set_ddram(i);
            lcd_write_character(WALL);
            obstacles[0][i] = obstacles[0][i+1];
            obstacles[0][i+1] = 0;
        }
        else if(obstacles[1][i+1] == WALL)
        {
            lcd_set_ddram(0x40+i+1);
            lcd_write_character(0x00);
            lcd_set_ddram(0x40+i);
            lcd_write_character(WALL);
            obstacles[1][i] = obstacles[1][i+1];
            obstacles[1][i+1] = 0;
        }
    }
    try_kill_player();
    if(obstacles[0][0] == WALL)
    {
        lcd_set_ddram(0x00);
        lcd_write_character(0x00);
        obstacles[0][0] = 0;
        points++;
    }
    if(obstacles[1][0] == WALL)
    {
        lcd_set_ddram(0x40);
        lcd_write_character(0x00);
        obstacles[1][0] = 0;
        points++;
    }
}

static void input()
{
    char c;
    keypad_poll();
    if(keypad_read_char(&c))
    {
        switch(c)
        {
        case '5':
            move_character_up();
            break;
        case '8':
            move_character_down();
            break;
        default:
            break;
        }
    }
}

int primitive_to_buffer(char *buffer, unsigned int buffer_size, int primitive)
{
    if(buffer_size < 2) return 0;
    if(primitive == 0)                          // test special case
    {
            buffer[0] = '0';
            buffer[1] = '\0';
            return 1;
    }
    unsigned int buffer_ptr = 0, i;
    int negative = primitive < 0;
    unsigned int n = negative ? -primitive : primitive;
    while(n)                                    // calculate each digit
    {
        if(buffer_ptr >= buffer_size) return 0;
        buffer[buffer_ptr++] = n % 10 + '0';
        n /= 10;
    }
    if(negative)                                // include the minus if negative
    {
        if(buffer_ptr >= buffer_size) return 0;
        buffer[buffer_ptr++] = '-';
    }
    if(buffer_ptr >= buffer_size) return 0;
    buffer[buffer_ptr] = '\0';
    for(i = 0; i < buffer_ptr/2; i++)           // swap the order of the string
    {
        buffer[i] ^= buffer[buffer_ptr-i-1];
        buffer[buffer_ptr-i-1] ^= buffer[i];
        buffer[i] ^= buffer[buffer_ptr-i-1];
    }
    return 1;
}

static void draw_score()
{
    char buf[3];
    primitive_to_buffer(buf, 3, points);
    lcd_set_ddram(0x0D);
    lcd_write_character(buf[0]);
    lcd_write_character(buf[1]);
    lcd_write_character(buf[2]);
}

static void victory_condition()
{
    if(points >= 100)
    {
        victory = 1;
        dead = 1;
    }
}

static void draw()
{
    char i;
    if(player_position)
    {
        lcd_set_ddram(0x40);
        lcd_write_character(PLAYER);
        lcd_set_ddram(0x00);
        lcd_write_character(0x00);
    }
    else
    {
        lcd_set_ddram(0x00);
        lcd_write_character(PLAYER);
        lcd_set_ddram(0x40);
        lcd_write_character(0x00);
    }
    for(i = 0; i < 16; i++)
    {
        lcd_set_ddram(0x00+i);
        if(obstacles[0][i])
            lcd_write_character(WALL);
        lcd_set_ddram(0x40+i);
        if(obstacles[1][i])
            lcd_write_character(WALL);
    }
    draw_score();
}

static void reset()
{
    char i;
    for(i = 0; i < 16; i++)
    {
        lcd_set_ddram(i);
        lcd_write_character(0x00);
        lcd_set_ddram(0x40+i);
        lcd_write_character(0x00);
        obstacles[0][i] = 0;
        obstacles[1][i] = 0;
        dead = 0;
        victory = 0;
        points = 0;
        player_position = 0;
    }
}

void game_loop()
{
    keypad_init();
    spi_init();
    lcd_init();

    while(!dead)
    {
        input();
        shift_obstacles();
        draw();
        chance_obstacles();
        victory_condition();
    }
    lcd_set_ddram(0x00);
    if(victory)
    {
        lcd_write_character(0x57);
    }
    else
    {
        lcd_write_character(0x44);
        lcd_write_character(0x45);
        lcd_write_character(0x41);
        lcd_write_character(0x44);
    }

    char y;
    for(y = 0; y < 10; y++)
        __delay_cycles(50000);

    reset();
}
