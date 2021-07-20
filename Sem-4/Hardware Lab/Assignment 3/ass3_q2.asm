section .data

msg1: db 'Enter the size of array:'
l1: equ $-msg1

msg2: db 'Enter the element '
l2: equ $-msg2

msg3 : db ' of the given array:'
l3 : equ $-msg3

msg4 : db'The Elements of the given array in the sorted order',10
l4 : equ $-msg4

msg5 : db'The Elements of the given array',10
l5 : equ $-msg5

space: db ' '
ls: equ $-space

nwl : db 10
l : equ $-nwl

section .bss

n:resd 1
arr:resd 100
arr1 : resd 100
i:resd 1
j : resd 1
Read:resd 1
Print:resd 1
temp:resd 1
zero:resd 1
counter:resd 1
s:resd 1
key : resd 1
count: resd 1


section .text

global _start:

_start:


mov eax,4
mov ebx,1
mov ecx,msg1
mov edx,l1
int 80h

call read_num

mov eax,dword[Read]
mov dword[n],eax

mov ebx,arr

call read_arr

mov eax,4
mov ebx,1
mov ecx,msg5
mov edx,l5
int 80h

mov ebx,arr

call print_array

call newline

mov dword[i],1


loop1:
	mov eax,dword[i]
	cmp eax,dword[n]
	je endloop1

	mov ebx,arr
	mov ecx,dword[ebx+eax*4]
	mov dword[key],ecx
	dec eax
	mov dword[j],eax

	loop2:
		cmp dword[j],0
		jl endloop2
		mov eax,dword[j]
		mov ecx,dword[ebx+eax*4]
		cmp ecx,dword[key]
		ja swap

endloop2:

	mov eax,dword[j]
	mov ecx,dword[key]
	mov dword[ebx+eax*4+4],ecx
	inc dword[i]
	jmp loop1


swap:
	mov eax,dword[j]
	mov ecx,dword[ebx+eax*4]
	mov dword[ebx+eax*4+4],ecx
	dec dword[j]
	jmp loop2

endloop1:
	
	mov eax,4
	mov ebx,1
	mov ecx,msg4
	mov edx,l4
	int 80h
	
	mov ebx,arr

	call print_array

	call newline

	mov eax,1
	mov ebx,0
	int 80h


newline :
	pusha
	
	mov eax,4
	mov ebx,1
	mov ecx,nwl
	mov edx,l
	int 80h
	
	popa
	ret


read_arr:

	pusha

	mov dword[i],0

	reading:
		
		mov eax,dword[i]
		cmp dword[n],eax
		je end_array

		call mes1

		mov eax,dword[i]
		add eax,1
		mov dword[Print],eax

		call print_num

		call mes2

		mov eax,dword[i]

		call read_num

		mov ecx,dword[Read]
		mov dword[ebx+eax*4],ecx

		inc dword[i]

		jmp reading
end_array : 
		popa
		ret

read_num:
	pusha
	
	mov dword[Read],0
	
	read:
		mov eax,3
		mov ebx,0
		mov ecx,temp
		mov edx,1
		int 80h

		cmp dword[temp],10
		je end_readnum

		sub dword[temp],30h
		
		mov eax,dword[Read]
		mov edx,0
		mov ebx,10
		mul ebx
		add eax,dword[temp]
		
		mov dword[Read],eax

		jmp read
end_readnum:
		popa
		ret

print_num:
	pusha
	
	mov dword[counter],0

	cmp dword[Print],0
	jne extract

	mov dword[zero],30h

	mov eax,4
	mov ebx,1
	mov ecx,zero
	mov edx,1
	int 80h

	jmp end_printnum

	extract:
		cmp dword[Print],0
		je print

		mov eax,dword[Print]
		mov edx,0
		mov ebx,10
		div ebx
		
		push edx
		
		mov dword[Print],eax
	
		inc dword[counter]
		jmp extract

print:
	cmp dword[counter],0
	je end_printnum

	pop edx

	mov dword[temp],edx
	add dword[temp],30h

	mov eax,4
	mov ebx,1
	mov ecx,temp
	mov edx,1
	int 80h

	dec dword[counter]

	jmp print

end_printnum:
	popa
	ret

print_array:
	pusha
	mov dword[i],0
	
	pril:
		mov eax,dword[i]
		cmp dword[n],eax
		je end_print_array

		mov eax,dword[i]
		mov ecx,dword[ebx+eax*4]

		mov dword[Print],ecx
		call print_num
		
		call spacing

		inc dword[i]
		jmp pril

end_print_array:
	popa
	ret

spacing:
	pusha
	
	mov eax,4
	mov ebx,1
	mov ecx,space
	mov edx,ls
	int 80h
	
	popa
	ret

mes1:
	pusha
	mov eax,4
	mov ebx,1
	mov ecx,msg2
	mov edx,l2
	int 80h
	popa
	ret
mes2:
	pusha
	mov eax,4
	mov ebx,1
	mov ecx,msg3
	mov edx,l3
	int 80h
	popa
	ret

