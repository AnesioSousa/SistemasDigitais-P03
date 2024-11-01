

.type set_background_block, %function
set_background_block:
    ldr r4, [sp, #0] @pega o param R da pilha
    sub sp, sp, #12
    str r10, [sp, #8]
    str r5, [sp, #4]
  
    ldr r10, =mem_mapped_file_descriptor
    ldr r10, [r10]
    
    @Calculo do endereco de onde vai ficar o bloco
    mov r5, #80
    mul r0, r5, r0 @r0 eh o param linha
    add r0, r0, r1 @r1 eh o param coluna
    lsl r0, #4
    orr r0, #0b0010  @opcode
    
    str r0, [r10, #0x80] @escreve em data_A
    
    mov r0, r2
    lsl r0, #3
    orr r0, r3
    lsl r0, #3
    orr r0, r4

    str r0, [r10, #0x70] @escreve em data_B
    
    sub sp, sp, #4
    str lr, [sp, #0]
    bl check_write
    ldr lr, [sp, #0]
    add sp, sp, #4

    mov r0, #0
    str r0, [r10, #0xc0] @escreve em start_signal
  
    mov r0, #1
    str r0, [r10, #0xc0] @escreve em start_signal
    
    ldr r10, [sp, #8]
    ldr r5, [sp, #4]
    add sp, sp, #12

    bx lr

set_sprite:
    ldr r4, [sp, #0] @pega o param sp na pilha
    sub sp, sp, #12
    str r10, [sp, #8]
    str r5, [sp, #4]
     
    ldr r10, =mem_mapped_file_descriptor
    ldr r10, [r10]

    @Calculo do endereco de onde vai ficar o sprite
    lsl r0, #4
    orr r0, #0b0000  @opcode
    
    str r0, [r10, #0x80] @escreve em data_A
    
    mov r0, r4
    lsl r0, #10
    orr r0, r1
    lsl r0, #10
    orr r0, r2
    lsl r0, #9
    orr r0, r3
    str r0, [r10, #0x70] @escreve em data_B    
    
    sub sp, sp, #4
    str lr, [sp, #0]
    bl check_write
    ldr lr, [sp, #0]
    add sp, sp, #4

    
    mov r0, #0
    str r0, [r10, #0xc0] @escreve em start_signal
  
    mov r0, #1
    str r0, [r10, #0xc0] @escreve em start_signal
    
    
    ldr r10, [sp, #8]
    ldr r5, [sp, #4]
    add sp, sp #12
  
    bx lr  


.type set_sprite, %function
set_sprite:
    ldr r4, [sp, #0] @pega o param sp na pilha
    sub sp, sp, #12
    str r10, [sp, #8]
    str r5, [sp, #4]
     
    ldr r10, =mem_mapped_file_descriptor
    ldr r10, [r10]

    @Calculo do endereco de onde vai ficar o sprite
    lsl r0, #4
    orr r0, #0b0000  @opcode
    
    str r0, [r10, #0x80] @escreve em data_A
    
    mov r0, r4
    lsl r0, #10
    orr r0, r1
    lsl r0, #10
    orr r0, r2
    lsl r0, #9
    orr r0, r3
    str r0, [r10, #0x70] @escreve em data_B    
    
    sub sp, sp, #4
    str lr, [sp, #0]
    bl check_write
    ldr lr, [sp, #0]
    add sp, sp, #4

    
    mov r0, #0
    str r0, [r10, #0xc0] @escreve em start_signal
  
    mov r0, #1
    str r0, [r10, #0xc0] @escreve em start_signal
    
    
    ldr r10, [sp, #8]
    ldr r5, [sp, #4]
    add sp, sp, #12
  
    bx lr
