#include "../drivers/screen.h"
#include "../cpu/isr.h"

void main()
{
//	clear_screen();
	isr_install();
	irq_install();

	/*
	__asm__ __volatile__("int $2");
	__asm__ __volatile__("int $3");
	__asm__ __volatile__("int $33");
	__asm__ __volatile__("int $42");
	*/
}
