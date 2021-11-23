//******************************************************************************/
//**************************Para Executar no hardware***************************/
//******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "altera_avalon_pio_regs.h"
#include <altera_avalon_lcd_16207.h>
#include <altera_avalon_lcd_16207_regs.h>
#include <altera_avalon_lcd_16207_fd.h>
#include "system.h"
#include <unistd.h> // usleep.h

void swap(char *x, char *y);
char* itoa(int value, char* buffer, int base);
char* reverse(char *buffer, int i, int j);
//lcd functions
void lcd_init();
void lcd_jump_line();
void  lcd_display(char *a);

int main() {
    /*Nios II acessa o controlador de LCD como um dispositivo STDOUT, e pode ser acessado com métodos de arquivo, such as printf().
    O controlador LCD pode ser facilmente integrado à arquitetura no QSYS. Se escolher no BSP Reduced Drives, não usa LCD*/

    typedef unsigned char alt_u8;
    alt_u8 palavra[8] = {0x07,0x08,0x21,0x12,0x41,0x0E,0x2F,0x2B}; // tAdSUFrn
    int sw1, swInputs, cont;
    char left_dir[] = "<<<<<";
    char right_dir[] = ">>>>>";
    int flag = 0;

    while(1) { // Este laço exibe permanentemente a frase TADS UFRN nos displays de 7 segmentos

    	swInputs = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE); //17 bits (1 é o sw17 usado para reset do cpu)
    	sw1 = (swInputs & 0x00001); // xxx 0 0000 0000 0000 0010
    	if (!flag) {
    	   cont = (sw1 == 1 ? 7 : 0);
    	   printf("cont: %d \n", cont);
    	   flag = 1;
    	}
    	sw1 == 0? lcd_display(left_dir) : lcd_display(right_dir);

    	IOWR_ALTERA_AVALON_PIO_DATA(HEX7_BASE, palavra[cont]); // t
        IOWR_ALTERA_AVALON_PIO_DATA(HEX6_BASE, palavra[cont+1]); // A
        IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, palavra[cont+2]); // d
        IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, palavra[cont+3]); // S
        IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, palavra[cont+4]); // U
        IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, palavra[cont+5]); // F
        IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, palavra[cont+6]); // r
        IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, palavra[cont+7]); // n

        if (flag) {
        	printf("entrei para alterar o cont: \n");
        	(sw1 == 0)? cont++ : cont--;
        }
        usleep(500000); //50000
        printf("cont fora: %d e sw fora: %d\n", cont,sw1);
        if((sw1 == 1 && cont < 0) || (sw1 == 0 && cont == 8)) {
            //cont = 7;
            //lcd_display(right_dir);
            flag = 0;
            printf("Entrei com cont: %d e sw1: %d\n", cont, sw1);
            //fprintf(lcd_d, ">>>>> \n");
        }

        //if(cont == 8 && sw1 == 0) {
        //    cont = 0;
        //    lcd_display(left_dir);
            //fprintf(lcd_d, "<<<<< \n");
        //}
    }
    return 0;
}

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
