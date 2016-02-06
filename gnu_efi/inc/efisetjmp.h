#ifndef GNU_EFI_SETJMP_H
#define GNU_EFI_SETJMP_H

#include "efisetjmp_arch.h"

extern UINTN setjmp(jmp_buf *env);
extern VOID longjmp(jmp_buf *env, UINTN value);

#endif /* GNU_EFI_SETJMP_H */
