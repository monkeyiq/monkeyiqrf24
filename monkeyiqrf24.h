
#include <Streaming.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"



struct radiomsg;
class rf24
{
    RF24 m_radio;
    bool m_debug;
    uint64_t m_pipes[2];
    enum { m_bufsz = 100 };
    char m_buf[m_bufsz+1];
    
  public:
    rf24( bool client = true, bool debug = false )
        : m_radio( 9,10 )
        , m_debug( debug )
    {
       m_pipes[0] = 0xF0F0F0F0E1LL;
       m_pipes[1] = 0xF0F0F0F0D2LL;
       if( !client )
       {
          m_pipes[1] = 0xF0F0F0F0E1LL;
          m_pipes[0] = 0xF0F0F0F0D2LL;
       }

       radio.begin();
       radio.setRetries(15,15);
       
       // write mode.
       radio.openWritingPipe(pipes[0]);
       radio.openReadingPipe(1,pipes[1]);  
       
       radio.startListening();
       if( m_debug ) 
           radio.printDetails();
    }

    struct radiomsg* tryGetMessage()
    {
        if ( m_radio.available() ) {
            memset( m_buf, 0, m_bufsz );
            m_radio.read( m_buf, m_bufsz );
            struct radiomsg* msg = (struct radiomsg*)m_buf;
            return msg;
        }
        return 0;
    }

    
    /**
    * If there is an interrupt for the radio then we sleep waiting for that
    * otherwise we just wait a bit.
    */
    void maybeSleep()
    {
        wait(0.02);
    }    
};


