
#include <wiringPi.h>
#include <iostream>
#include <unistd.h>

#include "PioHc595.h"
#include "PioHc595.cpp"

using namespace std;

int main (void)
{
	//serial data
	char signal = 0;
	int serialData[4][4] = {
	    {1,0,0,1},
	    {0,1,1,0},
	    {0,1,1,0},
	    {1,0,0,1}
	};

	cout << "Start GPIO Multiplex \n";

	wiringPiSetup();

	PioHc595 m_gpio(0, 1, 2, 3, 16); //pino_SCLK, pino_SDOUT, pino_SDIN, pino_RESSET, total_SAIDAS
	
	/* ---------------------------------------------------------------------------------------------*/
	/*
	m_gpio.desligar_SAIDAS();

	for ( char N=1; N <= 16; N++ )
	{		
		m_gpio.setar_SAIDA_n ( N ); // liga a saida, e assim o LED conectado nela.
		delay(500); // tempo que fica ligado.
		m_gpio.resetar_SAIDA_n ( N ); // desliga a saida, e assim o LED conectado nela.
	}

	m_gpio.desligar_SAIDAS();
	*/

	m_gpio.desligar_SAIDAS();
	
	for (int l=0; l<4; l++)
  	{
    		for(int c=0; c<4; c++)
    		{
    			signal++;

    			if(serialData[l][c] == 1)
      			{ 
      				m_gpio.setar_SAIDA_n ( signal ); // liga a saida, e assim o LED conectado nela.
				delay(500); // tempo que fica ligado.
				m_gpio.resetar_SAIDA_n ( signal ); // desliga a saida, e assim o LED conectado nela.
      			}

      			cout << serialData[l][c] << " ";
    		}

    		cout << "\n";
    	}

    	m_gpio.desligar_SAIDAS();

	/* ---------------------------------------------------------------------------------------------*/

	cout << "End GPIO Multiplex \n";

	return 0;
}
