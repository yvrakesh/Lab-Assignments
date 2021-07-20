section .data

msg1 : db'Enter the size of the array:'
l1 : equ $-msg1

msg2 : db'The element '
l2 : equ $-msg2

msg3 : db' of the given array:'
l3 : equ $-msg3

msg4 : db'The prime numbers present in the given array are ',10
l4 : equ $-msg4

nwl : db 10
l : equ $-nwl

space : db' '
ls : equ $-space


section .bss


Read : resd 1
Print : resd 1
n : resd 1
arr : resd 100
counter : resd 1
temp : resd 1
i : resd 1
a : resd 1
c : resd 1
num : resd 1
zero : resb 1

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

	call read_arr

	mov dword[i],0
	

primechecker:
	
	mov eax,dword[i]
	cmp dword[n],eax
	je label
	
	mov ebx,arr
	mov eax,dword[i]
	mov ecx,dword[ebx+eax*4]
	mov dword[a],ecx


	mov eax,dword[a]
	mov edx,0
	mov ebx,2
	div ebx
	mov dword[num],eax

	mov dword[c],2
	


	for:
		mov eax,dword[c]
		cmp dword[num],eax
		jb end1
		
		mov eax,dword[a]
		mov edx,0
		mov ebx,dword[c]
		div ebx
		cmp edx,0
		je end2
	
		inc dword[c]
		jmp for
end1:
	mov eax,dword[a]
	mov dword[Print],eax

	call print_num
	
	call spacing

	inc dword[i]
	jmp primechecker
	
end2 : 
	inc dword[i]
	jmp primechecker
	
label :
	call new_line

	mov eax,1
	mov ebx,0
	int 80h

new_line :
	pusha

	mov eax,4
	mov ebx,1
	mov ecx,nwl
	mov edx,l
	int 80h

	popa
	ret

spacing :
	pusha
	
	mov eax,4
	mov ebx,1
	mov ecx,space
	mov edx,ls
	int 80h
	
	popa
	ret

read_arr:

	pusha

	mov ebx,arr
	mov dword[i],0

	reading:
		
		mov eax,dword[i]
		cmp dword[n],eax
		je end_array

		mov eax,4
		mov ebx,1
		mov ecx,msg2
		mov edx,l2
		int 80h

		mov eax,dword[i]
		add eax,1
		mov dword[Print],eax

		call print_num

		mov eax,4
		mov ebx,1
		mov ecx,msg3
		mov edx,l3
		int 80h

		mov eax,dword[i]
		mov ebx,arr

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
		je end
		
		sub dword[temp],30h
		
		mov eax,dword[Read]
		mov edx,0
		mov ebx,10
		mul ebx
		add eax,dword[temp]
		mov dword[Read],eax
		
		jmp read
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
		
		jmp end
	
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
		je end
		
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

end:
	popa 
	ret
	
