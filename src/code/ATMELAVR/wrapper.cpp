#ifdef __AVR__
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

    void init(){
      TCNT0 = 0x00;
      #if defined (TCCR0)
        TCCR0B |= (0<<CS02) | (1<<CS01) | (1<<CS00);
      #endif
      #if defined (TCCR0A) && defined (TCCR0B)
        TCCR0A |= 0x00;
        TCCR0B |= (0<<CS02) | (1<<CS01) | (1<<CS00);  //4 microseconds tick on a 16 MHz oscillator
      #endif

      TIMSK0 |= 1 << TOIE0;

      sei();
    }

    uint64_t getMicros(){
      return (TCNT0 + 255 * overflowTick) / (F_CPU / 64.0 / 1000.0 / 1000.0);
    }
  }

  extern "C" void __cxa_pure_virtual()
  {
    //TODO: handle undefined pure function here
  }

  //interrupt for millis and timing
  ISR(TIMER0_OVF_vect)
  {
      wrapper::overflowTick++;
  }

#endif