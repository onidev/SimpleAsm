SimpleAsm
=========

A minimalist C++ ASM assembler and interpreter for learning purpose.

Five registers: ax, bx, cx, dx, ex.
A stack of size 1024.
10 instructions: mov, push, load, store, add, sub, mul, jmp, jz, print.
Instructions are 32 bits, and splitted in 4 bytes for the instruction number
and the registres id: (Instr, Reg1, Reg2, Reg3).

Example of Fibonacci in Simple Asm
```
	mov ax 0  ; a = 0
	mov bx 1  ; b = 1
	mov cx 10 ; i = 10

loop:
	; while(i)
	load cx
	jz end
	
	; t = a + b
	load ax
	load bx
	add
	; a = b
	mov ax bx
	; b = t
	store bx
	
	load bx
	print

	; i--
	load cx
	push 1
	sub
	store cx
	
	jmp loop

end:

```

Examples of [factorial](Examples/Factorial.txt) in Simple Asm.
