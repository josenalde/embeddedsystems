#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "alt_types.h"
#include "altera_avalon_lcd_16207_regs.h"
#include <string.h> //strlen

//acende 8 leds iniciais - para fazer o padrão nos outros 10 vermelhos da DE2, ajustar
alt_u8 led[8] ={0x00001,0x00002,0x00004,0x00008,0x00010,0x00020,0x00040,0x00080} ; //typedef unsigned char byte
int i,j;
//auxiliary functions for itoa implementation: https://www.techiedelight.com/implement-itoa-function-in-c/
void swap(char *x, char *y);
char* itoa(int value, char* buffer, int base);
char* reverse(char *buffer, int i, int j);
//lcd functions
void lcd_init();
void lcd_jump_line();
void  lcd_display(char *a);

int main() //BEGIN
{
	int v1,p=0;

	for(i=0; i<2; i++) { //pisca 2 vezes
    	IOWR_ALTERA_AVALON_PIO_DATA(LEDS_R_BASE, 0x00000); // 0000 0000 0000 0000 0000
    	usleep(1000000); // 1segundo
    	IOWR_ALTERA_AVALON_PIO_DATA(LEDS_R_BASE, 0xFFFFF); // 1111 1111 1111 1111 1111
    	usleep(1000000);
    }

	char num_count[32]; //16x2 display
    while (1) {
    	v1 = IORD_ALTERA_AVALON_PIO_DATA(CTU_BASE);
    	p+=v1;
    	//char a[] = "string muito grande"; //only for tests
    	//lcd_display(a);
    	lcd_display(itoa(p,num_count,10));
    	usleep(500000); //to get count change (delay) 500000 (good for manual switching sw)
    }
} //END

//check https://titanwolf.org/Network/Articles/Article?AID=0c2fa9fb-6c49-4fbd-92bc-4f19af83aaf5 for coordinates
//https://github.com/supeingo/RoboticArmControl/blob/master/robotic_arm/LCD.c

void  lcd_display(char *a) {
  //exibe no console
  printf("Ciclo n.: %s\n", a);
  lcd_init(); //para ir limpando a cada iteração, senão sobrepõe
  int w;
  for (w = 0; w < strlen(a); w++) {
	  if (w==16) {
	  	lcd_jump_line();
	   	break;
	  }
	  IOWR_ALTERA_AVALON_LCD_16207_DATA(LCD_BASE, a[w]);
	  usleep(100);
  }
  if (strlen(a) > 16) {
	 for (w=16; w<strlen(a); w++) {
     	IOWR_ALTERA_AVALON_LCD_16207_DATA(LCD_BASE, a[w]);
      	usleep(100);
     }
  }
}

/*
void lcd_display() { // test for DE2
	FILE *lcd_d;
	lcd_d = fopen(LCD_BASE, "w");
	if (lcd_d == NULL) {
		fprintf(stderr, "Erro abertura\n");
		return 0;
	}
	fprintf(lcd_d, "TADS UFRN \n"); // se o tamanho da string exibida não ultrapassar 16, não gera scroll
	fclose(lcd_d);
}
*/

void lcd_init() {
    /*
     * --   Function Set
--      2-line mode, display on             Line 93    lcd_data <= "00111100"; 3C
--      1-line mode, display on             Line 94    lcd_data <= "00110100"; 34
--      1-line mode, display off            Line 95    lcd_data <= "00110000"; 30
--      2-line mode, display off            Line 96    lcd_data <= "00111000"; 38

--   Display ON/OFF
--      display on, cursor off, blink off   Line 104   lcd_data <= "00001100";
--      display on, cursor off, blink on    Line 105   lcd_data <= "00001101";
--      display on, cursor on, blink off    Line 106   lcd_data <= "00001110";
--      display on, cursor on, blink on     Line 107   lcd_data <= "00001111";
--      display off, cursor off, blink off  Line 108   lcd_data <= "00001000";
--      display off, cursor off, blink on   Line 109   lcd_data <= "00001001";
--      display off, cursor on, blink off   Line 110   lcd_data <= "00001010";
--      display off, cursor on, blink on    Line 111   lcd_data <= "00001011";

--   Entry Mode Set
--      increment mode, entire shift off    Line 127   lcd_data <= "00000110";
--      increment mode, entire shift on     Line 128   lcd_data <= "00000111";
--      decrement mode, entire shift off    Line 129   lcd_data <= "00000100";
--      decrement mode, entire shift on     Line 130   lcd_data <= "00000101";
--
     */
	//--wait 50 ms to ensure Vdd has risen and required LCD wait is met
	usleep(50000); //15000 (15ms)
	IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_BASE, 0x38); // 38 2-line display off
    usleep(10);
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_BASE, 0x0E); //06
    usleep(70);
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_BASE, 0x01); //0E
    usleep(130);
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_BASE, 0x06); //entry mode set
    usleep(2140);
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_BASE, 0x00); //0E
    usleep(2200);
}

void lcd_jump_line() {
    IOWR_ALTERA_AVALON_LCD_16207_COMMAND(LCD_BASE,0xC0);
    usleep(1000);
}

// Iterative function to implement `itoa()` function in C
char* itoa(int value, char* buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32) {
        return buffer;
    }

    // consider the absolute value of the number
    int n = abs(value);

    int i = 0;
    while (n)
    {
        int r = n % base;

        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }

        n = n / base;
    }

    // if the number is 0
    if (i == 0) {
        buffer[i++] = '0';
    }

    // If the base is 10 and the value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0'; // null terminate string

    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}

// Function to swap two numbers
void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}

// Function to reverse `buffer[i…j]`
char* reverse(char *buffer, int i, int j)
{
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }

    return buffer;
}
