section .text
   global _start

_start:
	; Create a file
	mov rax, 8
	mov rbx, file_name
	mov rcx, 0777
	int 0x80

	; Grab file descriptor
	mov [fd_out], rax

	; Write string to file
	mov rax, 4
	mov rbx, [fd_out]
	mov rcx, string
	mov rdx, strlen
	int 0x80

	; Close the file
	mov rax, 6
	mov rbx, [fd_out]

	; Write the message indicating the end of writing to file
	mov rax, 4
	mov rbx, 1
	mov rcx, msg_done
	mov rdx, md_len
	int 0x80

	; Reopen the file
	mov rax, 5
	mov rbx, file_name
	mov rcx, 0
	mov rdx, 0777
	int 0x80

	mov [fd_in], eax

	; Read the file
	mov rax, 3
	mov rbx, [fd_in]
	mov rcx, info
	mov rdx, 14
	int 0x80

	; Close the file
	mov rax, 6
	mov rbx, [fd_out]

	; Print the info
	mov rax, 4
	mov rbx, 1
	mov rcx, info
	mov rdx, 14
	int 0x80

	; Exit
	mov rax, 1
	mov rbx, 0
	int 0x80

section .data
	msg_done 	db 'Finished writing to file.', 10
	md_len	 	equ $ - msg_done

	string		db 'Hello, World!', 10
	strlen 		equ $ - 	string

	file_name 	db 'File.txt'
	file_len 	equ $ - file_name

section .bss
	fd_in 	resb  1
	fd_out 	resb  1
	info 	  resb 14
