#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"

int main(void) {



  usartInit();

  while (1)
  {
    usartSendString("AT\n");
    _delay_ms(1000);
  }
}
