section .data

msg1 : db'Enter the size of the array:'
l1 : equ $-msg1

msg2 : db'The element '
l2 : equ $-msg2

msg3 : db' of the given array:'
l3 : equ $-msg3

msg4 : db'Enter the search element:'
l4 : equ $-msg4

msg5 : db'FOUND'
l5 : equ $-msg5
	
msg6 : db'NOT FOUND'
l6 : equ $-msg6

nwl : db 10
l_ : equ $-nwl

space : db' '
ls : equ $-space


section .bss


Read : resd 1
Print : resd 1
n : resd 1
arr : resd 100
counter : resd 1
temp : resd 1
s : resd 1
i : resd 1
a : resd 1
c : resd 1
num : resd 1
zero : resb 1
mid : resd 1
f : resd 1
l : resd 1

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

	mov eax,4
	mov ebx,1
	mov ecx,msg4
	mov edx,l4
	int 80h

	call read_num

	mov eax,dword[Read]
	mov dword[s],eax
	
	mov dword[f],0

	mov eax,dword[n]
	dec eax
	mov dword[l],eax

	
binarysearch:

	mov eax,dword[f]
	cmp dword[l],eax
	jb endsearch

	mov eax,dword[f]
	add eax,dword[l]
	mov ebx,2
	mov edx,0
	div ebx
	mov dword[mid],eax

	mov ebx,arr
	mov eax,dword[mid]
	mov ecx,dword[ebx+eax*4]
	cmp dword[s],ecx
	je hurray
	
	cmp dword[s],ecx
	jb leftsearch

	jmp rightsearch

hurray:
	mov eax,4
	mov ebx,1
	mov ecx,msg5
	mov edx,l5
	int 80h
	
	jmp label

leftsearch:
	
	mov eax,dword[mid]
	dec eax
	mov dword[l],eax
	jmp binarysearch

rightsearch:

	mov eax,dword[mid]
	inc eax
	mov dword[f],eax
	jmp binarysearch
endsearch:
	
	mov eax,4
	mov ebx,1
	mov ecx,msg6
	mov edx,l6
	int 80h
		
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
	mov edx,l_
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
	
