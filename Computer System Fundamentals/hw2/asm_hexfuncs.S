/* Assembly language implementation of hexdump functions */

	.section  .rodata

	/* TODO: add read-only data such as strings here */
	HexValue: .string "123456789abcdef"


	.section .text

	/* TODO: add functions here */
	.globl convertBaseTentoHex
	convertBaseTentoHex:
	    movq %rdi, %rax /*move input minto output */
        cmp $9, %rax
        jle .Num
        addq $87, %rax /*If the input becomes a letter than add 87 to it */
        ret
        .Num: /*If the input is a valid number, return it plus 48 */
            addq $48, %rax
            ret
    .globl hex_read
    hex_read: /*rdi, rsi, rdx, r8, and r9 are used to pass input */
        subq $8, %rsp /* Align stack */
        subq $16, %rsp /*Allocate space for rdi on stack*/
        pushq %rdi /*Push rdi to stack because it must be moved to rsi as part of syscall input*/
        movq $0, %rax /*Signal that syscall should be read*/
        movq %rdi, %rsi /*Prepare for sys call because rdi must be rsi for syscall input*/
        movq $0, %rdi /*An rdi of 0 indicates that it should be STDIN*/
        movq $16, %rdx /*Parameter for number of bytes to read is 16*/
        syscall /*call syscall with rdi, rsi, and rdx as input */
        popq %rdi /*restore rdi by popping from stack*/
        addq $16, %rsp /*realign stack from previous allocation*/
        addq $8, %rsp /*restore stack from previous alignment */
        ret
    .globl hex_write_string /*rdi, rsi, rdx, r8, and r9 are used to pass input */
    hex_write_string: /*For refernce: rdi is the input string, rsi, r8, r9 are all caller saved and can be used*/
        subq $8, %rsp /*Align the stack*/
        movq %rdi, %rsi /*Move the char input to rsi to prepapre for write*/
        movq $0, %r8 /*Set a counter for the string to find null terminator*/
        .Loop:
            cmpb $0, (%rsi, %r8) /*Compare current char to null terminator*/
            je .OutOfLoop /*If its the null terminator, jump to out of the loop*/
            inc %r8 /*Increment the counter variable*/
            jmp .Loop /*Jump back to top of the loop */
        .OutOfLoop:
        movq %r8, %rdx /*move the counter variable to rdx to prepare for system call*/
        movq $1, %rdi /*Signals that syscall is using STDIN_FILENO arguement*/
        movq $1, %rax /*This indicates to syscall it should be the write method*/
        syscall /* perform syscall which essentially executes write(%rdi, %rsi, %rdx) */
        addq $8, %rsp /*restore stack pointer*/
        ret
    .globl hex_format_offset
    hex_format_offset: /*rdi, rsi, rdx, r8, and r9 are used to pass input */
        subq $8, %rsp /*Align the stack*/
        pushq %r10 /*r10 is callee saved, must push before and pop after*/
        pushq %r11 /*r10 is callee saved, must push before and pop after*/
        /*Offset is in rdi, string is in rsi. I can use rdx, r8 and r9 at my disposal*/
        movq $0, %r9 /*temp variable i for a for loop*/
        .Loop1:
            cmpq $8, %r9 /*check if index is equal to 8*/
            je .OutOfLoop1 /*If index is 8 than jump out of loop*/
            movq $48, (%rsi, %r9) /*Make stirng at current index equla to ascii zero*/
            inc %r9 /*increment index*/
            jmp .Loop1 /*Loop again*/
        .OutOfLoop1:
            movq $0, %r8 /*prepare to use r8 as a remainder variable*/
            movq $7, %r9 /*prepare to use r9 as an index variable*/
            .While:
                cmpq $0, %rdi /*Compare the offset to zero*/
                je .OutOfWhile /* if they are equal, exit loop*/
                movq %rdi, %rax
                cqto
                movq $16, %r11
                idivq %r11 /*divide the offset, quotient stored at %rax, remainder at %rdx*/
                movq %rdx, %r8 /*move the remainder to the remainder variable*/
                movq %rdx, %rdi /*move remainder to prepare for call*/
                movq %rax, %r10 /*move quotient to temp location*/
                call convertBaseTentoHex /*execute call, new value stored at %rax*/
                movb %al, (%rsi, %r9) /*move new value from call onto string at index, has to be movb becuase its a char*/
                movq %r10, %rdi /*move quotient onto offset for next loop iteration*/
                dec %r9 /*decrement index*/
                jmp .While
            .OutOfWhile:
                movq $8, %r9
                movq $0, (%rsi, %r9) /*add null terminator to end of string*/
                movq %rsi, %rax /*move the string into the output string*/
        popq %r11 /*pop r10 back because its callee saved*/
        popq %r10 /*pop r10 back because its callee saved*/
        addq $8, %rsp /*Restore stack*/
        ret
    .globl hex_to_printable /*This is due as part of milestone 1 */
    hex_to_printable:
        subq $8, %rsp /*Align stack*/
        cmpq $126, %rdi /* compare input to ascii table to determine whether it is valid*/
        jg .Invalid
        cmpq $32, %rdi /*Second compare to make sure its valid */
        jl .Invalid

        movq %rdi, %rax /*If the input is a valid printable character, return the input in %rax */
        addq $8 ,%rsp/*Restore stack*/
        ret

        .Invalid: /*If invalid input, return the ascii value for '.' in %rax*/
            movq $46, %rax
            addq $8 ,%rsp/*Restore stack*/
            ret

    .globl hex_format_byte_as_hex
    hex_format_byte_as_hex: /*This is due as part of milestone 1 */
        subq $8, %rsp
        movq %rdi, %r9 /*Make a copy of the byteval for shifting*/
        shr $4, %rdi /*Shift right 4 bits to get the first char in rdi */
        and $15, %rdi /*Eliminate last 4 bits*/
        and $15, %r9 /*Do a bitwise AND to eliminate the last 4bits and get the second digit */
        call convertBaseTentoHex /*Takes in input in %rdi and returns it in %rax */
        movq %r9, %rdi /*Move r9 into %rdi in prep for calling convertBaseTentoHex again*/
        movb %al, 0(%rsi) /* %al is the 0th byte of %rax, moved that into the array */
        call convertBaseTentoHex /*Takes in input in %rdi and returns it in %rax */
        movb %al, 1(%rsi) /*Move second char into array*/
        movb $0, 2(%rsi) /*Move null temrinator to end of array*/
        addq $8, %rsp /*Restore stack*/
        ret

/* vim:ft=gas:
 */
