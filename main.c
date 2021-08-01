
#include "stm32f4xx_hal.h"

void SysTick_Handler() {
	GPIOG->ODR ^= (1 << 14);

}
UART_HandleTypeDef myuart;
int main()
{
	HAL_Init();

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;

	GPIOG->MODER |= GPIO_MODER_MODE13_0 | GPIO_MODER_MODE14_0;
	__GPIOA_CLK_ENABLE();

	GPIOA->MODER |= (1 << 21) | (1 << 19);     //AF modes for PA9 and PA10
	GPIOA->AFR[1] |= (GPIO_AF7_USART1 << 8) | (GPIO_AF7_USART1 << 4);     //USART1 AF, PA9 and PA10
	GPIOA->OTYPER |= (1 << 9);

	myuart.Instance = USART1;
	myuart.Init.BaudRate = 115200;
	myuart.Init.WordLength = UART_WORDLENGTH_8B;
	myuart.Init.StopBits = UART_STOPBITS_1;
	myuart.Init.Parity = UART_PARITY_NONE;
	myuart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	myuart.Init.Mode = UART_MODE_TX_RX;
	__HAL_RCC_USART1_CLK_ENABLE();
	SysTick_Config(SystemCoreClock);

	if (HAL_UART_Init(&myuart) != HAL_OK) {
		asm("bkpt 255");
	}
	volatile int countdown = 1000000;
	for (;;)
	{
		while (countdown > 0) {
			countdown--;
		}
		GPIOG->ODR ^= (1 << 13);
		countdown = 1000000;

		HAL_UART_Transmit(&myuart, "test ", 6, 1);

	}

}
