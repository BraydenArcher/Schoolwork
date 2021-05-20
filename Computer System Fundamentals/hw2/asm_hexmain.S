/* Hexdump main function, assembly language version */

	.section .text
        Space: .string " "
        Space3: .string "   "
        NewLine: .string "\n"
        ColonSpace: .string ": "
    .section .data
        sbuf: .zero 17
        offset: .zero 9
        hex: .zero 3
        end: .zero 17
	.globl main
main: /*r12,    r13,   r14,   r15,   rbx, rbp*/
      /*index, bytes, sbuf, offset,  hex, end*/
    subq $8, %rsp
    movq $0, %r10 /*Instantiate index*/
    movq $16, %rdi /*temp index variable*/
    movq $sbuf, %r14 /*define strings into registers*/
    movq $offset, %r15
    movq $hex, %rbx
    movq $end, %rbp
    movq $0, (%r14, %rdi) /*add null terminator to the end of sbuf*/
    movq $0, %r12 /*make index 0*/
    .WhileLoop:
        movq $0, %r13 /*set bytes to 0*/
        movq $sbuf, %rdi /*Move sbuf into rdi to prepare to be read*/
        call hex_read /*call read on sbuf/rdi*/
        movq %rax, %r13 /*move bytes back into r13 from rax*/
        cmp $0, %r13
        je .End
        /*Prepare to call hex format offset*/
        movq %r12, %rdi /*Move index into rdi to prepare for call to format_offset*/
        shl $4, %rdi
        movq %r15, %rsi /*Move offset into rsi to feed to format_offset*/
        call hex_format_offset /*calls method with rdi and rsi as input*/
        movq %rax, %r15 /*move the returned string to offset*/
        movq %r15, %rdi /*move offset into rdi to write*/
        call hex_write_string /*Write rdi/offset*/
        movq $ColonSpace, %rdi /*prepare to write colon space*/
        call hex_write_string /*Write a colon and space*/
        pushq %r12 /*temporarily push r12 to use as index for for loop*/
        movq $0, %r12 /*make r12 0 for index */
        .ForLoop:
            cmp %r13, %r12 /*This is a for loop (int i = 0; i < bytes; i++) r12 is i*/
            jge .OutOfForLoop /*jump to end loop*/
            movq (%r14, %r12), %rdi /*move sbuf[i] into register to prepare for call */
            movq %rbx, %rsi /*move hex into rsi to prepare for call*/
            call hex_format_byte_as_hex /*call*/
            movq %rsi, %rbx /*move returned value to hex*/
            movq %rsi, %rdi /*move new array into rdi to prepare for print*/
            call hex_write_string /*write stirng*/
            movq $Space, %rdi /*move space into argument position*/
            call hex_write_string /*write space*/
            movb (%r14, %r12), %dil /*prepare sbuf[i] to be fed to hex_to_printable*/
            call hex_to_printable /*call*/
            movb %al, (%rbp, %r12) /*move returned char into end[i]*/
            inc %r12 /* i++ */
            jmp .ForLoop /*repeat loop*/
        .OutOfForLoop:
        movb $0, (%rbp, %r12) /*move null terminator into the string wherever the bytes end*/
        movq $16, %r12 /* move 16 into r12 to prepare for some caculations below*/
        cmp %r12, %r13 /*check if bytes are less than 16*/
        jge .Not
        .If:
            subq %r13, %r12 /*do 16 - bytes calculation*/
            movq %r12, %r13 /*move value to bytes*/
            movq $0, %r12 /*make r12 0 as a temp index*/
            .ForLoop2:
                cmp %r12, %r13 /*if i < 16-bytes, loop*/
                je .Not /*if loop condition is over break loop*/
                movq $Space3, %rdi /*prepare three spaces to be printed*/
                call hex_write_string /*write spaces*/
                inc %r12 /* i++ */
                jmp .ForLoop2 /*start loop over*/
        .Not:
        popq %r12 /*restore index to r12*/
        movq $Space, %rdi /*prepare a space to be written*/
        call hex_write_string /*write a space*/
        movq %rbp, %rdi /*prepare to write end string*/
        call hex_write_string /*Write end string*/
        movq $NewLine, %rdi /*prepare to write new line*/
        call hex_write_string /*write new line*/
        inc %r12 /*increment index for offset*/
        jmp .WhileLoop
        .End:
        addq $8, %rsp /*Realign stack*/
	    ret

/* vim:ft=gas:
 */
