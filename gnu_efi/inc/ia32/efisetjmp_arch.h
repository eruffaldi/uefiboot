#ifndef GNU_EFI_IA32_SETJMP_H
#define GNU_EFI_IA32_SETJMP_H

typedef struct {
	UINT32	Ebx;
	UINT32	Esi;
	UINT32	Edi;
	UINT32	Ebp;
	UINT32	Esp;
	UINT32	Eip;
} __attribute__((__aligned__(4))) jmp_buf;

#define JMPBUF_ALIGN 4

#endif /* GNU_EFI_IA32_SETJMP_H */
