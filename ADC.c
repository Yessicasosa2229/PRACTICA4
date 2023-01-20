
#include "lib/include.h"

extern void Configura_Reg_ADC0(void)
{
    /*
Usando el modulo 0 y 1 , configurar la tarjeta a la frecuencia asignada,
para adquirir 6 señales analógicas a una velocidad de 1 millón de muestras
por segundo, por los canales asignados  y guardar los valores en un arreglo 
para ser enviados con un botones  externos asociado al gpio D a través del 
protocolo de comunicación asíncrona a una velocidad de 115200 todo esto usando 
interrupciones.

 
    */
    //pag352 Habilitar y proporcionar un reloj al Modulo ADC0 Y ADC1
    SYSCTL->RCGCADC |= (1<<0) | (1<<1) ; //Modulo 0 y 1

     //Pag 406 (RGCGPIO) Habilitar Puertos a utilizar
    //                     F     E      D       C      B     A
    SYSCTL->RCGCGPIO |= (0<<5)|(1<<4)|(1<<3)|(0<<2)|(1<<1)|(0<<0);
 
    //Pag 663 (GPIODIR) Habilta los pines como I/O un cero para entrada y un uno para salida
    //PARA ADC TIENEN QUE SER ENTRADAS
    //EL GPIO -DIR se hace para cada puerto 
    GPIOB_AHB->DIR = (0<<5) ; //PB5
    GPIOD_AHB->DIR = (0<<0) | (0<<1) | (0<<3); //PD0 PD1 PD3
    GPIOE_AHB->DIR = (0<<1) | (0<<4); //PE1 PE4

    //(GPIOAFSEL) pag.672 Enable alternate función para que el modulo analógico tenga control de esos pines, 
    //EL GPIO -AFSEL se hace para cada puerto
    GPIOB_AHB->AFSEL =  (1<<5); //PB5
    GPIOD_AHB->AFSEL =  (1<<0) | (1<<1 ) | (1<<3 );//PD0 PD1 PD3
    GPIOE_AHB->AFSEL =  (1<<1) | (1<<4 ); //PE1 PE4
    
    //(GPIODEN) pag.682 desabilita el modo digital
    GPIOB_AHB->DEN = (0<<5) ; //PB5
    GPIOD_AHB->DEN = (0<<0) | (0<<1) | (0<<3); //PD0 PD1 PD3
    GPIOE_AHB->DEN = (0<<1) | (0<<4); //PE1 PE4

    //*************Pag 787 GPIOPCTL registro combinado con el GPIOAFSEL y la tabla pag 1808
    GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0xFF00FFFF);
    
    //(GPIOAMSEL) pag.786 Con el 1 habilitamos la función analogica
    GPIOB_AHB->AMSEL =  (1<<5); //PB5
    GPIOD_AHB->AMSEL =  (1<<0) | (1<<1 ) | (1<<3 );//PD0 PD1 PD3
    GPIOE_AHB->AMSEL =  (1<<1) | (1<<4 ); //PE1 PE4

//----------------------------------------------------------------------------------------------------
                //Configuración MODULO 0, Utilizaré el sec0 y el sec2

  //Pag 891 El registro (ADCPC) establece la velocidad de conversión por segundo
    //Voy a utilizar ADC0, Y ADC1, será a 1 millon de muestras por segundo = 0x7 = 0111bin= (1<<2)|(1<<1)|(1<<0)
    ADC0->PC = (1<<2)|(1<<1)|(1<<0);// 1msps
  
  //Pag 841 Este registro (ADCSSPRI) configura la prioridad de los secuenciadores
    
    //ADC0->SSPRI = 0x3210;  //Por default 
    ADC0->SSPRI = 0x0000;   
     
 //Pag 821 (ADCACTSS) Este registro controla la activación de los secuenciadores, ocuparé secuenciador 0, 2, 3
    //Primero deshabilito, para poder configurar 
    ADC0->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0);

    //Pag 1091 Este registro (ADCEMUX) selecciona el evento que activa la conversión (trigger) EL TRIGGER LO HARÉ POR SOFTWARE LO HARÉ POR BIT PARA QUE EMPICE A TRABAKAR
    ADC0->EMUX  |= (0x0000);



    //Sec0 voy a mandar 3 señales (11,9,6)
    //Sec2 voy a mandar 2 señales (7,4)

    //------------------------------------------------------------------------------
    //Selector 0
    //Pag 1129 Este registro (ADCSSMUX0) define las entradas analógicas con el canal y secuenciador seleccionado
    ADC0->SSMUX0 = 0x0000069B; // SEÑALES 11, 9, 6

    //pag 868 Este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción //LO DEL BIT AL ULTIMO QUE CAMBIA
    ADC0->SSCTL0 =  0X00000644;   //EL 6 se pone porque ES EL ULTIMO QUE VAS A LEER
   
    //---------------------------------------------------------------------------------------
   //Selector 2
      //Pag 1129 Este registro (ADCSSMUX0) define las entradas analógicas con el canal y secuenciador seleccionado
    ADC0->SSMUX2 = 0x0047; // SEÑALES 11, 9, 6

    //pag 868 Este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción //LO DEL BIT AL ULTIMO QUE CAMBIA
    ADC0->SSCTL2 =  0X0064;   //EL 6 se pone porque ES EL ULTIMO QUE VAS A LEER

    /* Enable ADC Interrupt */ //PONER 1
    ADC0->IM |= (1<<0)|(1<<2); /* Unmask ADC0 sequence 2 interrupt pag 1082*/
    //NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFFFF00) | 0x00000020;
    //NVIC_EN0_R = 0x00010000;
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS = (0<<3) | (1<<2) | (0<<1) | (1<<0); //SOLO ACTIVO EL SEC QUE ME TOCA 
    ADC0->PSSI |= (1<<0) | (1<<2); //Para inicializar el muestreo en el secuenciador 

//--------------------------------------------------------------------------------------------------------   
             //Configuración MODULO 1, Utilizaré el sec3
    
    //Pag 891 El registro (ADCPC) establece la velocidad de conversión por segundo
    //Voy a utilizar ADC0, Y ADC1, será a 1 millon de muestras por segundo = 0x7 = 0111bin= (1<<2)|(1<<1)|(1<<0)
    ADC1->PC = (1<<2)|(1<<1)|(1<<0);// 1msps

    //Pag 841 Este registro (ADCSSPRI) configura la prioridad de los secuenciadores
    ADC1->SSPRI = 0x000;  //Por default 

    //Pag 821 (ADCACTSS) Este registro controla la activación de los secuenciadores, ocuparé secuenciador 0, 2, 3
    //Primero deshabilito, para poder configurar 
    ADC1->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0);

    //Pag 1091 Este registro (ADCEMUX) selecciona el evento que activa la conversión (trigger) EL TRIGGER LO HARÉ POR SOFTWARE LO HARÉ POR BIT PARA QUE EMPICE A TRABAKAR
    ADC1->EMUX  |= (0x0000);
//-----------------------------------------------------------------------
//SEC3 SEÑAL 2
    //Pag 1129 Este registro (ADCSSMUX2) define las entradas analógicas con el canal y secuenciador seleccionado
    ADC1->SSMUX3 = 0x0002; //PRIMER MUESTREO AN9 SEGUNDO AN8 0X0654

    //pag 868 Este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción //LO DEL BIT AL ULTIMO QUE CAMBIA
    ADC1->SSCTL3 = 0x0006 ; //  EL 6 ES EL ULTIMO QUE VAS A LEER

    /* Enable ADC Interrupt */ //PONER 1
    ADC1->IM |= (1<<3); /* Unmask ADC0 sequence 2 interrupt pag 1082*/

    //NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFFFF00) | 0x00000020;
    //NVIC_EN0_R = 0x00010000;
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC1->ACTSS = (1<<3) | (0<<2) | (0<<1) | (0<<0); //SOLO ACTIVO EL SEC QUE ME TOCA 
    ADC1->PSSI |= (1<<3); //Para inicializar el muestreo en el secuenciador 
}

extern void ADC0_InSeq2(uint16_t *Result){

    //ADC Processor Sample Sequence Initiate (ADCPSSI)
       ADC0->PSSI = 0x00000004;
       while((ADC0->RIS&0x04)==0){}; // espera al convertidor
       Result[1] = ADC0->SSFIFO2&0xFFF; //  Leer  el resultado almacenado en la pila2
       Result[0] = ADC0->SSFIFO2&0xFFF;
       printChar('A');
       ADC0->ISC = 0x0004;  //Conversion finalizada

}


