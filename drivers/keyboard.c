#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "../libc/mem.h"

#define BACKSPACE 0x0E
#define ENTER 0x1c

#define SC_MAX 57

static char key_buffer[256];

const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6", 
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", 
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", 
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", 
        "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
        'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 
        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
        'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

static void user_input(char *input)
{
	if(strcmp(input,"END") == 0)
	{
		kprint("Stopping the CPU.Bye!\n");
		asm volatile("hlt");
	}
	else if(strcmp(input,"PAGE") == 0)
	{
		u32 phys_addr;
		u32 page = kmalloc(1000,1,&phys_addr);
		char page_str[16];
		hex_to_ascii(page,page_str);
		char phys_str[16] = "";
		hex_to_ascii(phys_addr,phys_str);
		kprint("Page: ");
		kprint(page_str);
		kprint(",physical address: ");
		kprint(phys_str);
		kprint("\n");
	}
	kprint("you said: ");
	kprint(input);
	kprint("\n");
}

static void keyboard_callback(registers_t regs)
{
	u8 scancode = port_byte_in(0x60);
	if(scancode > SC_MAX)
	{
		return;
	}
	if(scancode == BACKSPACE)
	{
		backspace(key_buffer);
		kprint_backspace();
	}
	else if(scancode == ENTER)
	{
		kprint("\n");
		user_input(key_buffer);
		key_buffer[0] = '\0';
	}
	else
	{
		char letter = sc_ascii[(int)scancode];
		char str[2] = {letter,'\0'};
		append(key_buffer,letter);
		kprint(str);
	}
	UNUSED(regs);
}

void init_keyboard()
{
	register_interrupt_handler(IRQ1, keyboard_callback);
}

/*
static void print_letter(u8 scancode) {
	switch (scancode) {
	case 0x0:
		kprint("ERROR");
		break;
	case 0x1:
		kprint("ESC");
		break;
	case 0x2:
		kprint("1");
		break;
	case 0x3:
		kprint("2");
		break;
	case 0x4:
		kprint("3");
		break;
	case 0x5:
		kprint("4");
		break;
	case 0x6:
		kprint("5");
		break;
	case 0x7:
		kprint("6");
		break;
	case 0x8:
		kprint("7");
		break;
	case 0x9:
		kprint("8");
		break;
	case 0x0A:
		kprint("9");
		break;
	case 0x0B:
		kprint("0");
		break;
	case 0x0C:
		kprint("-");
		break;
	case 0x0D:
		kprint("+");
		break;
	case 0x0E:
		kprint("Backspace");
		break;
	case 0x0F:
		kprint("Tab");
		break;
	case 0x10:
		kprint("Q");
		break;
	case 0x11:
		kprint("W");
		break;
	case 0x12:
		kprint("E");
		break;
	case 0x13:
		kprint("R");
		break;
	case 0x14:
		kprint("T");
		break;
	case 0x15:
		kprint("Y");
		break;
	case 0x16:
		kprint("U");
		break;
	case 0x17:
		kprint("I");
		break;
	case 0x18:
		kprint("O");
		break;
	case 0x19:
		kprint("P");
		break;
	case 0x1A:
		kprint("[");
		break;
	case 0x1B:
		kprint("]");
		break;
	case 0x1C:
		kprint("ENTER");
		break;
	case 0x1D:
		kprint("LCtrl");
		break;
	case 0x1E:
		kprint("A");
		break;
	case 0x1F:
		kprint("S");
		break;
	case 0x20:
		kprint("D");
		break;
	case 0x21:
		kprint("F");
		break;
	case 0x22:
		kprint("G");
		break;
	case 0x23:
		kprint("H");
		break;
	case 0x24:
		kprint("J");
		break;
	case 0x25:
		kprint("K");
		break;
	case 0x26:
		kprint("L");
		break;
	case 0x27:
		kprint(";");
		break;
	case 0x28:
		kprint("'");
		break;
	case 0x29:
		kprint("`");
		break;
	case 0x2A:
		kprint("LShift");
		break;
	case 0x2B:
		kprint("\\");
		break;
	case 0x2C:
		kprint("Z");
		break;
	case 0x2D:
		kprint("X");
		break;
	case 0x2E:
		kprint("C");
		break;
	case 0x2F:
		kprint("V");
		break;
	case 0x30:
		kprint("B");
		break;
	case 0x31:
		kprint("N");
		break;
	case 0x32:
		kprint("M");
		break;
	case 0x33:
		kprint(",");
		break;
	case 0x34:
		kprint(".");
		break;
	case 0x35:
		kprint("/");
		break;
	case 0x36:
		kprint("Rshift");
		break;
	case 0x37:
		kprint("Keypad *");
		break;
	case 0x38:
		kprint("LAlt");
		break;
	case 0x39:
		kprint("Spc");
		break;
	default:
		if (scancode <= 0x7f)
		{
			kprint("Unknown key down");
		}
		else if (scancode <= 0x39 + 0x80)
		{
			kprint("key up ");
			print_letter(scancode - 0x80);
		} 
		else
		{
			kprint("Unknown key up");
		}
		break;
	}
}
*/
