#include "stm32f4xx.h"            //bibliotecas padrão STM32F4
#include <stdio.h>
#include "Utility.h"        //biblioteca de funções utilitárias

int jogarDado(){
    if(RNG->SR & RNG_SR_DRDY){
        uint32_t number = RNG->DR;
        int dado = number % 6;
        return dado + 1;
    }
}

//jogador = 0 computador
//jogador = 1 humano
int jogarRound(int jogador) {
    int resultado = jogarDado();
    if(jogador == 0)
        printf("O Computador jogou o dado e obteve o numero: %d!\n", resultado);
    else
        printf("Voce jogou o dado e obteve o numero: %d!\n", resultado);
    return resultado;
}


//função principal
int main(void)
{
    Configure_Clock();                //configura o sistema de clock
    USART1_Init();
    Delay_Start();

    printf("\n--------  Bem Vindo ao Jogo de Dados - Melhor de 3  --------\n\n");
    Delay_ms(2000);

    RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;    //habilita o clock do RNG
    RNG->CR |= RNG_CR_RNGEN;            //liga o RNG
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;//LIGA CLOCK na porta E
	GPIOE->PUPDR |= (0b01 << 8);//ativa o pull up em PE4 = k0

    int computador = 0, jogador = 0, rodada = 1;

    while(1)
    {
        if(!(GPIOE->IDR & (1 << 4))){
            printf("---- Rodada %d ----\n", rodada);
            int resultado1 = jogarRound(0);
            int resultado2 = jogarRound(1);

            // Verifica quem ganhou a rodada
            if (resultado1 > resultado2) {
                printf("O computador venceu a rodada!\n");
                computador++;
            } else if (resultado2 > resultado1) {
                printf("Voce venceu a rodada!\n");
                jogador++;
            } else {
                printf("Empate na rodada!\n");
            }

            // Imprime a pontuação atual
            printf("Pontuacaoo atual: Computador: %d, Voce: %d\n\n", computador, jogador);

            rodada++;
            Delay_ms(75);
			while(!(GPIOE->IDR & (1 << 4)));
        }
        if(computador == 2 || jogador == 2){
            if (computador > jogador) {
                printf("O computador venceu o jogo!\n");
            } else {
                printf("Você venceu o jogo!\n");
            }
            computador = 0;
            jogador = 0;
            rodada = 1;
        }
    }
}
