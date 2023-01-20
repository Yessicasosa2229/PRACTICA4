#include "lib/include.h"

char data_str[32] = " ";
uint16_t adc_data[adc_canales] = {0};
uint8_t = 0;

int main(void)
{

    uint16_t Result[2];
    float valor;
    float valor1;
    Configurar_PLL();  //Confiuracion de velocidad de reloj
    Configura_Reg_ADC0();
    Configura_Reg_ADC1();
    Configurar_UART0();
    Configurar_ADCleercanal();
    //printString("3");
    while(1)
    {
        PWM0
        ADC0_InSeq2(Result); //llamada a la conversion por procesador
        valor=(float)(((Result[0]))*3.3)/4096;
        valor1=(float)(((Result[1]))*3.3)/4096;
        

    }

}
