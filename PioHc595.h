
/*
* Autor: Jason Piloti 
* Código original: Elcids Chagas
*/

#ifndef PioHc595_h
#define PioHc595_h
 
#include <wiringPi.h>
#include <unistd.h>

class PioHc595 
{
  #define BIT_0 0x01  // padrao binario correspondente ao "bit 0".
  #define BIT_1 0x02  // padrao binario correspondente ao "bit 1".
  #define BIT_2 0x04  // padrao binario correspondente ao "bit 2".
  #define BIT_3 0x08  // padrao binario correspondente ao "bit 3".
  #define BIT_4 0x10  // padrao binario correspondente ao "bit 4".
  #define BIT_5 0x20  // padrao binario correspondente ao "bit 5".
  #define BIT_6 0x40  // padrao binario correspondente ao "bit 6".
  #define BIT_7 0x80  // padrao binario correspondente ao "bit 7".
  #define BIT_blank 0x00  // padrao para todos os bits em "0".

  private:
    const char TAB_bits [8] = { BIT_0, BIT_1, BIT_2, BIT_3, BIT_4, BIT_5, BIT_6, BIT_7 };
   
    int  total_SAIDAS;
    
    int pino_SCLK;
    int pino_SDOUT;
    int pino_SDIN;
    int pino_RESSET;

    int n_reg_SAIDAS;
    char * vetor_SAIDAS;

    void SAIDAS_SERIAL_OUT ( char DATA_8 );
    short  HEX16_to_BCD16 ( short HEX_16 );

  public:
    PioHc595(int pino_SCLK, int pino_SDOUT, int pino_SDIN, int pino_RESSET, int total_SAIDAS);
    
    void atualizar_SAIDAS ( char *REG_SAIDAS );
    void desligar_SAIDAS ();
    void resetar_SAIDA_n ( char n );
    void setar_SAIDA_n ( char n );
};

#endif
