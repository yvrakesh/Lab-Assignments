section .data

msg1 : db'Enter the number of rows in the first matrix:'
l1 : equ $-msg1

msg2 : db'Enter the number of columns in the first matrix:'
l2 : equ $-msg2

msg3 : db'Enter the element '
l3 : equ $-msg3

msg4 : db' of the required matrix:'
l4 : equ $-msg4

msg5 : db'The elements of the matrix resulted by multiplying the two matrices',10
l5 : equ $-msg5

msg6 : db'The elements of the first matrix are ',10
l6 : equ $-msg6

msg7 : db'The elements of the second matrix are ',10
l7 : equ $-msg7

msg8 : db'Enter the number of rows of the second matrix:'
l8 : equ $-msg8

msg9 : db'Enter the number of columns of the second matrix:'
l9 : equ $-msg9

msg10 : db'Number of columns in the first matrix must be equal to the number of rows in the second matrix for matrix multiplication to be possible'
l10 : equ $-msg10

msg11 : db'Invalid Input for matrix multiplication',10
l11 : equ $-msg11

nwl : db 10
l : equ $-nwl

space : db ' '
ls : equ $-space

section .bss

Read : resd 1
Print : resd 1
row : resd 1
column : resd 1
row1 : resd 1
column1 : resd 1
row2 : resd 1
column2 : resd 2
n : resd 1
temp : resd 1
counter : resd 1
arr : resd 200
arr1 : resd 200
arr2 : resd 200
arr3 : resd 200
i : resd 1
j : resd 1
k : resd 1
x : resd 1
y : resd 1
z : resd 1
sum : resd 1
zero : resd 1

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
	mov dword[row1],eax

	mov eax,4
	mov ebx,1
	mov ecx,msg2
	mov edx,l2
	int 80h

	call read_num

	mov eax,dword[Read]
	mov dword[column1],eax
	

	mov eax,dword[row1]
	mov edx,0
	mov ebx,dword[column1]
	mul ebx
	mov dword[n],eax

	mov eax,dword[row1]
	mov dword[row],eax

	mov eax,dword[column1]
	mov dword[column],eax

	
	mov ebx,arr1

	call read_array


	mov eax,4
	mov ebx,1
	mov ecx,msg8
	mov edx,l8
	int 80h

	call read_num
	
	mov eax,dword[Read]
	mov dword[row2],eax
	
	mov eax,dword[column1]
	cmp dword[row2],eax
	jne endprogram

	mov eax,4
	mov ebx,1
	mov ecx,msg9
	mov edx,l9
	int 80h

	call read_num
	
	mov eax,dword[Read]
	mov dword[column2],eax
	

	mov eax,dword[row2]
	mov edx,0
	mov ebx,dword[column2]
	mul ebx
	mov dword[n],eax

	mov eax,dword[row2]
	mov dword[row],eax

	mov eax,dword[column2]
	mov dword[column],eax

	mov ebx,arr2

	call read_array

	mov eax,4
	mov ebx,1
	mov ecx,msg6
	mov edx,l6
	int 80h

	mov ebx,arr1
	
	mov eax,dword[row1]
	mov dword[row],eax

	mov eax,dword[column1]
	mov dword[column],eax
	
	call print_array

	mov eax,4
	mov ebx,1
	mov ecx,msg7
	mov edx,l7
	int 80h

	mov ebx,arr2
	
	mov eax,dword[row2]
	mov dword[row],eax

	mov eax,dword[column2]
	mov dword[column],eax

	call print_array
	
	mov dword[i],0

	


matrixmultiplication:
	
	
	loop1 : 
		mov eax,dword[i]
		cmp dword[row1],eax
		je endloop1

		mov dword[j],0
		loop2:
			mov eax,dword[j]
			cmp dword[column2],eax
			je endloop2
			
			mov eax,dword[i]
			mov ebx,dword[column2]
			mul ebx
			add eax,dword[j]
			mov dword[x],eax
			
			mov dword[k],0
			mov dword[sum],0

			loop3:
				mov eax,dword[k]
				cmp dword[column1],eax
				je endloop3

				mov eax,dword[i]
				mov ebx,dword[column1]
				mov edx,0
				mul ebx
				add eax,dword[k]
				mov dword[y],eax


				mov eax,dword[k]
				mov ebx,dword[column2]
				mov edx,0
				mul ebx
				add eax,dword[j]
				mov dword[z],eax

				mov ebx,arr1
				mov eax,dword[y]
				mov ecx,dword[ebx+eax*4]
				mov ebx,arr2
				mov eax,dword[z]
				mov edx,dword[ebx+eax*4]
				mov eax,edx
				mul ecx
				add eax,dword[sum]
				mov dword[sum],eax

				inc dword[k]
				jmp loop3
		
			
endloop3:
		inc dword[j]
		
		mov ebx,arr3
		mov eax,dword[x]
		mov ecx,dword[sum]
		mov dword[ebx+eax*4],ecx
		
		jmp loop2
endloop2 : 
		inc dword[i]
		jmp loop1
endloop1:
					
	mov eax,4
	mov ebx,1
	mov ecx,msg5
	mov edx,l5
	int 80h
	
	mov eax,dword[row1]
	mov dword[row],eax
	
	mov eax,dword[column2]
	mov dword[column],eax

	
	mov eax,dword[row]
	mov ebx,dword[column]
	mul ebx
	mov dword[n],eax

	mov ebx,arr3
	
	call print_array

label:
	mov eax,1
	mov ebx,0
	int 80h

endprogram:
	
	
	mov eax,4
	mov ebx,1
	mov ecx,msg11
	mov edx,l11
	int 80h

	mov eax,4
	mov ebx,1
	mov ecx,msg10
	mov edx,l10
	int 80h
	
	call newline

	jmp label

print_array:
	pusha

	mov dword[i],0
	mov dword[j],0
	mov dword[k],0
	
	printing:
		mov eax,dword[i]
		cmp dword[n],eax
		je end_array_print	
		
		mov eax,dword[i]
		
		mov ecx,dword[ebx+eax*4]
		mov dword[Print],ecx
		
		call print_num

		mov eax,dword[k]
		inc eax
		cmp dword[column],eax
		je print_newline

		call spacing

		inc dword[k]
		inc dword[i]
		jmp printing

		
print_newline:
		mov dword[k],0
		inc dword[j]
		inc dword[i]
		call newline
		jmp printing
	
end_array_print:
		popa
		ret
		

read_array:

	pusha

	mov dword[i],0
	mov dword[j],0
	mov dword[k],0

	reading:

		mov eax,dword[i]
		cmp dword[n],eax
		je end_array

		call m1
	
		mov eax,dword[j]
		mov dword[Print],eax

		call print_num

		mov eax,dword[k]
		mov dword[Print],eax
		
		call print_num
		
		call m2
		
		call read_num

		mov ecx,dword[Read]
		mov eax,dword[i]

		mov dword[ebx+eax*4],ecx

		mov eax,dword[k]
		inc eax
		cmp dword[column],eax
		je change_row
		
		inc dword[k]
		inc dword[i]
		jmp reading


change_row:
		inc dword[j]
		mov dword[k],0
		inc dword[i]
		jmp reading
		
end_array:
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
			je end_read
			
			sub dword[temp],30h

			mov eax,dword[Read]
			mov edx,0
			mov ebx,10
			mul ebx
			add eax,dword[temp]
			mov dword[Read],eax

			jmp read
end_read:
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
		
		jmp end_print

		extract:
			cmp dword[Print],0
			je print
		
			mov eax,dword[Print]
			mov edx,0
			mov ebx,10
			div ebx
			mov dword[Print],eax
			push edx
			
			inc dword[counter]
			jmp extract

print:
	cmp dword[counter],0
	je end_print
	
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

end_print:
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
	
newline:
	pusha
	
	mov eax,4
	mov ebx,1
	mov ecx,nwl
	mov edx,l
	int 80h
	
	popa
	ret

m1:
	pusha

	mov eax,4
	mov ebx,1
	mov ecx,msg3
	mov edx,l3
	int 80h
	
	popa
	ret

m2:
	
	pusha

	mov eax,4		
	mov ebx,l
	mov ecx,msg4
	mov edx,l4
	int 80h

	popa
	ret
		
