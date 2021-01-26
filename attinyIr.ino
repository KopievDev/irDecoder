#define F_CPU 9600000UL
#define LED PB0       
#define IR PB4
#include <avr/io.h>  
#include <avr/interrupt.h>

unsigned char n_bit = 0;
unsigned char is_started = 0;
unsigned char n_zeros = 0;
unsigned char n_ones = 0;
uint32_t data;

void init(){
  DDRB |= (1<<LED); // выход 
  DDRB &= ~(1<<IR); // вход 0b000001
  PORTB = 0;
  PORTB |= (1<<IR); //0b010000
  //TCCR0B |=(1<<CS02) | (1<<CS00); //  CLK/1024
  TCCR0B=0x5;
  OCR0A=0x2; // частота 3125 Гц
  //TIMSK0 |=(1<<OCIE0A);//Разрешаем выполнение прерываний по совпадению в OCR0A

  TIMSK0=0x04;//Разрешаем выполнение прерываний по совпадению в OCR0A
  sei();
}

ISR (TIMER1_COMPA_vect)
{
char b = (PINB >> 4) & 1;
  if (b) {
    if (n_zeros == 12) {
      is_started = !is_started;
      if (is_started)
        data = 0;
      else
        if (data == 0xc9b60383)//0xc9b60383 3384148867
        {
            PORTB |= (1<<LED); //
        }
        if (data == 4119462787) //0xf58a0383 4119462787
        {
            PORTB &= (0<<LED); //
        }
    }
    n_ones++;
    n_zeros = 0;
  } else {
    if (is_started) {
      if (n_ones == 4) {
        data = (data >> 1);
        data |= 0x80000000;
      }
      else if (n_ones) {
        data = (data >> 1);
        data &= 0x7fffffff;
      }  
    }
    n_zeros++;
    n_ones = 0;
  }
  if (n_zeros > 12)
    n_zeros = 12;
  if (n_ones > 4)
    n_ones = 4;
}

int main(void)
{
  init();
  while(1)
  {
    
  }
}
