SimpleAsm
=========

A simple C++ ASM assembler and interpreter for learning purpose.

Examples of SimpleAsm sources codes: [factorial](Examples/Factorial.txt), [fibonacci](Examples/Fib.txt)

```
	mov ax 8 ; counter
	mov bx 1 ; result storing
	
loop:
	load ax  ; push the counter on top of the stack
	jz end   ; if top stack is zero the calcul is completed
	
	load bx
	print
	
	; result *= counter
	; bx *= ax
	load ax
	load bx
	mul
	store bx
	
	; counter--
	; bx--
	load ax
	push 1
	sub
	store ax
	
	jmp loop

end:
```
