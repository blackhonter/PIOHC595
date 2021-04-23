
/*
* Autor: Jason Piloti 
* Adaptado de: Elcids Chagas
*/

#include "PioHc595.h"

PioHc595::PioHc595(int pino_SCLK, int pino_SDOUT, int pino_OUTPUT_LD, int pino_OUTPUT_RESSET, int total_SAIDAS) 
{ 
    this->pino_SCLK = pino_SCLK;
    this->pino_SDOUT = pino_SDOUT;
    this->pino_OUTPUT_LD = pino_OUTPUT_LD;
    this->pino_OUTPUT_RESSET = pino_OUTPUT_RESSET;
    
    this->total_SAIDAS = total_SAIDAS;
    this->n_reg_SAIDAS = total_SAIDAS/8;

    vetor_SAIDAS = new char[this->n_reg_SAIDAS];

    digitalWrite ( pino_SDOUT, LOW ); // estado inicial do sinal "SDOUT".
    digitalWrite ( pino_SCLK, LOW );  // estado inicial do sinal "SCLK".
    digitalWrite ( pino_OUTPUT_LD, LOW ); // estado inicial do sinal "OUTPUT_LD".
    digitalWrite ( pino_OUTPUT_RESSET, LOW ); // estado inicial do sinal "OUT_RESSET".
  
    pinMode ( pino_SDOUT, OUTPUT );   // configura o pino do sinal "SDOUT".
    pinMode ( pino_SCLK, OUTPUT );    // configura o pino do sinal "SCLK".
    pinMode ( pino_OUTPUT_LD, OUTPUT ); // configura o pino do sinal "OUTPUT_LD".
    pinMode ( pino_OUTPUT_RESSET, OUTPUT ); // configura o pino do sinal "OUT_RESSET".
  
    desliga_SAIDAS ();
}

//**********************************************************************************************************************************************
//  Funcao para serializacao de um padrao binario especificado, para o Registro fisico das Saidas Digitais. A funcao serializa
//  8 bits, o que implica que um char e' necessario para representar o estado de 8 Saidas.
//**********************************************************************************************************************************************
void PioHc595::SAIDAS_SERIAL_OUT ( char DATA_8 )
{
  char bit_n; // bit atual sendo enviado ao ShiftRegister.
  char bit_MASK;  // mascara binaria para o bit atual.

  bit_MASK = BIT_7; // seleciona o bit inicial.

  for ( bit_n = 0; bit_n <= 7; bit_n++ ) // para cada bit do padrao binario:
  {
    if ( DATA_8 & bit_MASK )  // se o bit for "1":
    {
      digitalWrite ( pino_SDOUT, HIGH ); // seta a Saida em "1".
    }
    else  
    {
      digitalWrite ( pino_SDOUT, LOW ); // senao, seta a Saida em "0".
    }

    digitalWrite ( pino_SCLK, LOW ); // prepara para enviar o bit.
    delayMicroseconds(1); // garante o "setup time".

    digitalWrite ( pino_SCLK, HIGH ); // envia o bit atual.
    delayMicroseconds(1); // garante o "hold time".

    bit_MASK = ( bit_MASK >> 1 ); // seleciona proximo bit.
  }
}

//**********************************************************************************************************************************************
//  Atualiza o Registro fisico das Saidas Digitais com o conteudo do Registro "virtual", ou seja, com o conteudo da variavel de
//  Registro especificada. A sequencia de envio do conteudo da variavel de Registro, deve estar conforme as ligacoes fisicas no Hardware.
//**********************************************************************************************************************************************
void  PioHc595::atualizar_SAIDAS ( char *REG_SAIDAS )
{
  char n;

  digitalWrite ( pino_OUTPUT_LD, LOW ); // "pre-setting" do PCLK.

  for ( n = n_reg_SAIDAS; n>0; n-- )  // para cada Registro de Saida:
  {
    SAIDAS_SERIAL_OUT ( REG_SAIDAS [n-1] ); // envia o conteudo do Registro "n".
  }

  digitalWrite ( pino_OUTPUT_LD, HIGH );  // atualiza o Registro fisico das Saidas.
}


//**********************************************************************************************************************************************
//  Reseta todos as Saidas Digitais fisicas. Isto e' feito, setando em "0" todos os bits correspondentes as Saidas no Registro "virtual" (variavel de Registro).
//  Apos a alteracao do Registro virtual, o Registro fisico e' atualizado.
//**********************************************************************************************************************************************

void PioHc595::desligar_SAIDAS ()
{
  char n;
  
  for ( n=0; n < n_reg_SAIDAS; n++ ) // para cada Registro de Saida:
  {
    vetor_SAIDAS [n] = 0; // reseta o Registro "n".
  }

  atualizar_SAIDAS ( vetor_SAIDAS ); // atualiza o estado fisico das Saidas.

  //informa resset
  digitalWrite ( pino_OUTPUT_RESSET, HIGH );
  delayMicroseconds(1);
  
  digitalWrite ( pino_OUTPUT_RESSET, LOW );
  delayMicroseconds(1);
}

//**********************************************************************************************************************************************
//  Reseta apenas a Saida Digital fisica "n" especificada.
//  Simplesmente o bit que corresponde a Saida especificada "n", e' colocado em "0" no Registro "virtual" (variavel de Registro).
//  Apos a alteracao do Registro virtual, o Registro fisico e' atualizado.
//**********************************************************************************************************************************************

void PioHc595::resetar_SAIDA_n ( char n )
{
    char n_REG, n_bit;

  if ( ( n >= 1 ) && ( n <= total_SAIDAS ) )  // se "n" e' valido:
  {
    n_REG = (n-1) / 8;  // obtem o numero do Registro da Saida.
    n_bit = (n-1) & 0x07; // obtem o numero do bit no Registro da Saida.

    vetor_SAIDAS [n_REG] &= ~TAB_bits [n_bit];  // reseta o bit no "Registro Virtual".
    
    atualizar_SAIDAS ( vetor_SAIDAS ); // atualiza o estado fisico das Saidas.
  }
}

//**********************************************************************************************************************************************
//  Seta apenas a Saida Digital fisica "n" especificada.
//  Simplesmente o bit que corresponde a Saida especificada "n", e' colocado em "1" no Registro "virtual" (variavel de Registro).
//  Apos a alteracao do Registro virtual, o Registro fisico e' atualizado.
//**********************************************************************************************************************************************

void PioHc595::setar_SAIDA_n ( char n )
{
    char n_REG, n_bit;

  if ( ( n >= 1 ) && ( n <= total_SAIDAS ) )  // se "n" e' valido:
  {
    n_REG = (n-1) / 8;  // obtem o numero do Registro da Saida.
    n_bit = (n-1) & 0x07; // obtem o numero do bit no Registro da Saida.

    vetor_SAIDAS [n_REG] |= TAB_bits [n_bit]; // seta o bit no "Registro Virtual".

    atualizar_SAIDAS ( vetor_SAIDAS ); // atualiza o estado fisico das Saidas.
  }
}

//***********************************************************************
//  Funcao para converter um valor binario de 16 bits, para um valor "decimal codificado em binario" (BCD). Como cada digito "BCD" ocupa 4 bits, entao o maximo valor resultante de uma
//  conversao sera' 9999, que e' portanto o limite numerico para uma conversao correta. Valores acima deste limite, terao o zero como resultado. A tecnica classica de conversao e' usada,
//  ou seja, divisoes sucessivas por 10, para se obter cada um dos digitos BCD.
//=======================================================================

short  PioHc595::HEX16_to_BCD16 ( short HEX_16 )
{
  short  BCD_result;
  char  i;

  BCD_result = 0; // valor inicial da conversao (somatorio).

  if ( HEX_16 < 10000 ) // se esta' dentro do limite conversivel:
  {
    i = 0;  // contador de deslocamentos, para "pesagem" dos digitos.

    while ( HEX_16 >= 10 )  // enquanto for possivel dividir por 10:
    {
      BCD_result += ( ( HEX_16 % 10 ) << i ); // calcula e acrescenta um digito BCD.
      HEX_16 /= 10; // valor inteiro para obter proximo digito BCD.

      i += 4; // obtem o "peso" para o proximo digito BCD.
    }

    BCD_result += ( HEX_16 << i );  // acrescenta o digito BCD menos significativo.
  }

  return ( BCD_result );  // retorna o resultado convertido para BCD.
}