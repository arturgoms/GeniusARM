#include<TM4C123GH6PM.h>
#include<stdlib.h>
#include<inttypes.h>
#include<stdbool.h>
#include <time.h>

void delay(int valor);
void led(int pinLed, int delayValue);

int main(void) {
    SYSCTL_RCGCGPIO_R |= 0b001001; // Ativa o port A e D Decimal = 9

    GPIO_PORTD_DIR_R = 0b00000000; // Seta o Port D para Input BTN Decimal = 0
    GPIO_PORTA_DIR_R = 0b11110000; // Seta o port A para Output LED Decimal = 240

    GPIO_PORTD_DATA_R = 0;         // Zera o port D Decimal = 0
    GPIO_PORTA_DATA_R = 0;         // Zera o port A - acende os leds Decimal = 0

    GPIO_PORTD_PUR_R = 0b00001111; // Zeta os Btn para PULL UP Decimal = 15

    GPIO_PORTA_DEN_R = 0b11110000; // Habilita os Leds Decimal = 240
    GPIO_PORTD_DEN_R = 0b00001111; // Habilita os Btn Decimal = 15

    uint32_t rodada;
    uint32_t venceu;
    bool perdeu = false;
    uint32_t random;
    uint8_t cont = 0;
    uint32_t portDread;
    uint32_t handler;
    uint32_t randomArray[20];
    uint32_t zeraVetor = 0;
    for(zeraVetor = 0; zeraVetor < 19; zeraVetor++){randomArray[zeraVetor] = 0;}

    while(1) {
        for(rodada = 0; rodada < cont; rodada++) {
            GPIO_PORTA_DATA_R = 0b11110000; // Apaga Leds Decimal = 240
            led(randomArray[rodada], 10);
        }

        GPIO_PORTA_DATA_R = 0b11110000; // Decimal = 240
        GPIO_PORTD_DATA_R = 0b00000000; // Decimal = 0

        int timeValue;
        timeValue = (unsigned)time(NULL);
        srand(timeValue);
        random = (rand() % 4);
        randomArray[cont] = random;

        led(random, 10);
        for(rodada = 0; rodada <= cont; rodada++) {

            while(1) {
                if(perdeu == true){break;}

                bool btnUp = false;
                portDread = GPIO_PORTD_DATA_R;
                handler = portDread << 4;

                switch(handler) {
                        case 16:
                            if((randomArray[rodada] == 0) == false) {
                                perdeu = true;
                            }
                            led(0, 5);
                            btnUp = true;
                            break;
                        case 32:
                            if((randomArray[rodada] == 1) == false) {
                                perdeu = true;
                            }
                            led(1, 5);
                            btnUp = true;
                            break;
                        case 64:
                            if((randomArray[rodada] == 2) == false) {
                                perdeu = true;
                            }
                            led(2, 5);
                            btnUp = true;
                            break;
                        case 128:
                            if((randomArray[rodada] == 3) == false) {
                                perdeu = true;
                            }
                            led(3, 5);
                            btnUp = true;
                            break;
                        case !0:
                            perdeu = true;
                            break;
                }
                if(btnUp == true) {
                    while(handler != 0) {
                        portDread = GPIO_PORTD_DATA_R;
                        handler = portDread >> 4;
                    }
                    break;
                }
            }
        }
        if(perdeu == false) {
             // PROXIMO NIVEL
            if (rodada == 19){ // VENCEU TODOS OS 20 NIVEIS
                led(6, 3);
                led(6, 3);
                led(6, 3);
                led(6, 3);
                led(6, 3);
                led(6, 3);
                cont = 0;
            }else{
            led(4, 5);}
            cont++;
        } else {
            cont = 0; // REINICIA O JOGO
            perdeu = false;
            led(5, 5);
        }
    }

}

void delay(int valor){
    uint32_t i;
    valor = valor * 100000;
    for(i = 0; i < valor; i++);
}

void led(int pinLed, int delayValue) {
    switch(pinLed) {
        case 0://LED 1 - VERMELHO
            GPIO_PORTA_DATA_R = 0b01110000;
            break;
        case 1://LED 2 - VERDE
            GPIO_PORTA_DATA_R = 0b10110000;
            break;
        case 2://LED 3 - VERMELHO
            GPIO_PORTA_DATA_R = 0b11010000;
            break;
        case 3://LED 4 - VERDE
            GPIO_PORTA_DATA_R = 0b11100000;
            break;
        case 4:// ACERTOU
            GPIO_PORTA_DATA_R = 0b01010000;
            break;
        case 5:// PERDEUUUU
            GPIO_PORTA_DATA_R = 0b10100000;
            break;
        case 6:// ACENDE TODOS OS LEDS
            GPIO_PORTA_DATA_R = 0b00000000;
            break;
    }
    delay(delayValue);
    // Apaga os leds
    GPIO_PORTA_DATA_R = 0b11110000;
    // Zera o valor nos btn
    GPIO_PORTD_DATA_R = 0b00000000;

    delay(delayValue);
}
