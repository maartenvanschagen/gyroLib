#if defined(__AVR__) && !defined(ARDUINO)
  #include "wrapper.h"
  #include <stdint.h>
  #include <util/delay.h>
  #include <avr/io.h>
  #include <avr/interrupt.h>

  namespace wrapper{
    uint64_t overflowTick = 0;

    void delayMillis(const uint64_t milliseconds){
      _delay_ms(milliseconds); 
    }

    void init(){ // TODO: add support for use of timer 1 instead for less interrupts
      TCNT0 = 0x00; // set timer to 0
      #if defined (TCCR0)
        TCCR0B |= (0<<CS02) | (1<<CS01) | (1<<CS00); //set tick speed (4 microseconds on 16 MHz to be arduino compatible)
      #endif
      #if defined (TCCR0A) && defined (TCCR0B)
        TCCR0A |= 0x00;  //this register is useless for us
        TCCR0B |= (0<<CS02) | (1<<CS01) | (1<<CS00);  //set tick speed (4 microseconds on 16 MHz to be arduino compatible)
      #endif

      TIMSK0 |= 1 << TOIE0; //enable timer overflow interrupt

      sei();
    }

    uint64_t getMicros(){
      return (TCNT0 + 255 * overflowTick) /* current ticks + times overflowed * max value */ / (F_CPU / 64.0 / 1000.0 / 1000.0);
    }
  }

  extern "C" void __cxa_pure_virtual()
  {
    //TODO: handle undefined pure function here
  }

  //interrupt for millis and timing
  ISR(TIMER0_OVF_vect)  //timer overflow interrupt
  {
      wrapper::overflowTick++;
  }

#endif