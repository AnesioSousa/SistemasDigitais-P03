.equ sys_open, 5
.equ sys_map, 192
.equ munmap, 91
.equ page_len, 4096
.equ prot_read, 1
.equ prot_write, 2
.equ map_shared, 1

.global mem_map
.global mem_unmap
.global set_background_block
.global clear_background

.type mem_map, %function
mem_map:
    sub sp, sp, #28
    str r7, [sp, #24]
    str r5, [sp, #20]
    str r4, [sp, #16]
    str r3, [sp, #12]   
    str r2, [sp, #8]
    str r1, [sp, #4]
    str r0, [sp, #0]

    ldr r0, =fileName
    mov r1, #2
    mov r2, #0
    mov r7, #sys_open    
    svc 0 

    ldr r1, =file_descriptor @descritor do arquivo dev/mem
    str r0, [r1]
 
    mov r4, r0
    mov r0, #0
    mov r1, #page_len
    mov r2, #(prot_read + prot_write)
    mov r3, #map_shared
    ldr r5, =fpga_bridge
    ldr r5, [r5]
    mov r7, #sys_map      
    svc 0

    ldr r1, =mem_mapped_file_descriptor
    str r0, [r1]

    ldr r7, [sp, #24]
    ldr r5, [sp, #20]
    ldr r4, [sp, #16]
    ldr r3, [sp, #12]   
    ldr r2, [sp, #8]
    ldr r1, [sp, #4]
    ldr r0, [sp, #0]
  
    add sp, sp, #28
    
    bx lr 

.type mem_unmap %function
mem_unmap:
    sub sp, sp, #12
    str r7, [sp, #8]
    str r1, [sp, #4]
    str r0, [sp, #0]
    
    ldr r0, =mem_mapped_file_descriptor
    ldr r0, [r0]
    mov r1, #page_len
    mov r7, #munmap
    svc 0

    ldr r7, [sp, #8]
    ldr r1, [sp, #4]
    ldr r0, [sp, #0]
    add sp, sp, #12

    bx lr

@ WBR - 000 000 000 00000 0000
.type clear_background, %function
clear_background:
    sub sp, sp, #20  
    str r5, [sp, #16]
    str r4, [sp, #12]
    str r3, [sp, #8]
    str r1, [sp, #4]
    str r0, [sp, #0]

    ldr r0, =mem_mapped_file_descriptor
    ldr r0, [r0]

    add r3, r0, #0x80 @ Carrega DATA_A_BASE em r3
   
    mov r1, #0b00000 @ coluna
    lsl r1, #4
    orr r1, #0b0000  @opcode
    str r1, [r3]

    add r4, r0, #0x70 @ Carrega DATA_B_BASE em r4
    
    mov r1, #0b000
    lsl r1, #3
    orr r1, #0b000
    lsl r1, #3
    orr r1, #0b000
    str r1, [r4]

    @ ldr r0, =start_signal
    add r5, r0, #0xc0 @ Carrega start_signal em r5

    mov r1, #0 
    str r1, [r5]
 
    mov r1, #1 
    str r1, [r5]
    
    ldr r5, [sp, #16]
    ldr r4, [sp, #12]
    ldr r3, [sp, #8]
    ldr r1, [sp, #4]
    ldr r0, [sp, #0]
    add sp, sp, #20    

    bx lr

.type set_background_block, %function
set_background_block:
    ldr r4, [sp, #0]
    
    sub sp, sp, #32
    str r11, [sp, #28]
    str r10, [sp, #24]
    str r9, [sp, #20]
    str r8, [sp, #16]
    str r7, [sp, #12]
    str r6, [sp, #8]  
    str r5, [sp, #4]

    ldr r10, =mem_mapped_file_descriptor
    ldr r10, [r10]
    
    add r7, r10,  #0xc0 @ Carrega start_signal em r5
    add r6, r10, #0x70 @ Carrega DATA_B_BASE em r4
    add r5, r10, #0x80 @ Carrega DATA_A_BASE em r3
    
    @Calculo do endereco
    mov r8, r0 
    mov r9, r1
    mov r11, #80
    mul r8, r11, r8
    add r8, r8, r9
    lsl r8, #4
    orr r8, #0b0010  @opcode
    str r8, [r5] @escreve em data_A
    
    mov r8, r2
    lsl r8, #3
    orr r8, r3
    lsl r8, #3
    orr r8, r4
    str r8, [r6] @escreve em data_B

    mov r8, #0 
    str r8, [r7] @escreve em start_signal
 
    mov r8, #1 
    str r8, [r7] @escreve em start_signal
    
    ldr r11, [sp, #28]
    ldr r10, [sp, #24]
    ldr r9, [sp, #20]   
    ldr r8, [sp, #16]
    ldr r7, [sp, #12]    
    ldr r6, [sp, #8]
    ldr r5, [sp, #4]
    add sp, sp, #32

    bx lr

error:
  mov r0, #1
  ldr r1, =failed
  mov r2, #7
  mov r7, #1
  svc 0

	mov r7, #1
	mov r0, #1
	svc 0

.data
fileName: .asciz "/dev/mem"
failed: .ascii "Erro no mapeamento de memoria!!\n"
fpga_bridge: .word 0xff200

mem_mapped_file_descriptor: .space 4
file_descriptor: .space 4

start_signal: .word 0xc0
DATA_A_BASE:                     .word 0x80
DATA_B_BASE:                     .word 0x70
RESET_PULSECOUNTER_BASE:         .word 0x90
SCREEN_BASE:                     .word 0xa0
WRFULL_BASE:                     .word 0xb0
WRREG_BASE:                      .word 0xc0
