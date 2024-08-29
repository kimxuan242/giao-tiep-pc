#define F_CPU 7372800UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <myLCD.h>
#include <stdio.h>
//truyen
void uart_char_tx(unsigned char chr) {
	while (bit_is_clear(UCSR0A, UDRE0));
	UDR0 = chr;
}

volatile unsigned char u_data;
char str[] = "-Vat Ly Dien Tu";

void hien_thi() {
	clr_LCD();
	//move_LCD(1, 1);
	printf_LCD("TRUYEN UART:");
	//char c[10] = {u_data, '\0'};
	//printf_LCD(c);
	putChar_LCD(u_data);
}

ISR(USART0_RX_vect) {
	u_data = UDR0;
	hien_thi();
	if (u_data == 'b') {
		//hien_thi();
		PORTE |= (1 << PE2);
		PORTE &=~(1<<PE3);
	}
	else if (u_data == 'a')
	{
		//hien_thi();
		PORTE |= (1 << PE3);
		PORTE &=~(1<<PE2);
	}
	else if(u_data == 'c')
	{
		//hien_thi();
		PORTE &= ~((1 << PE2)|(1 << PE3));
	}
	else if(u_data=='d')
	{
		//hien_thi();
		PORTE |=(1<<PE2)|(1<<PE3);
	}
	else{
		clr_LCD();
		hien_thi();
		move_LCD(2,1);
		printf_LCD("KO THUC HIEN");
		PORTE &= ~((1 << PE2)|(1 << PE3));
	}
}


int main(void) {
	init_LCD();
	clr_LCD();
	
	DDRD |= (1 << PD0) | (1 << PD1);
	DDRE |= (1 << PE3)|(1<<PE2);
	DDRB |= (1 << PB6);
	DDRB &= ~(1<<PB5);
	PORTB |=(1<<PB5);
	//115200
	UBRR0H = 0;
	UBRR0L = 3;

	UCSR0A = 0x00;
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
	// Ch?n d? li?u 8 bit
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

	sei();
	while (1) {
		//putChar_LCD('a');
		if(bit_is_clear(PINB,5))
		{
			//while(bit_is_clear(PINB,5));
			
			for (int i = 0; i < sizeof(str)-1; i++) {
				uart_char_tx(str[i]);
				_delay_ms(10);
			}
			clr_LCD();
			//printf_LCD("                                    ");
			//move_LCD(1,1);
			printf_LCD(str);
			_delay_ms(500);
		}
	}
}