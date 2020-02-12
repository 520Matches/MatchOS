#include "../drivers/screen.h"
#include "util.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"

void main()
{
//	clear_screen();
	isr_install();

	asm volatile("sti");
//	init_timer(50);
	init_keyboard();
	/*
	__asm__ __volatile__("int $2");
	__asm__ __volatile__("int $3");
	__asm__ __volatile__("int $33");
	__asm__ __volatile__("int $42");
	*/
}
