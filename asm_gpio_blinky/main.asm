;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section.
            .retainrefs                     ; And retain any sections that have
                                            ; references to current section.

;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer


;-------------------------------------------------------------------------------
; Main
;-------------------------------------------------------------------------------

setup:
			bic.b	#0FFh, P4OUT			; Clear port 4 to avoid issues.
			bis.b	#BIT7, P4DIR			; Set P4.7 (LED2) to output.
main:
			xor.b	#BIT7, P4OUT			; Toggle P4.7 (LED2).
			call	#delay					; Delay for a short time.
			jmp		main


;-------------------------------------------------------------------------------
; Subroutines
;-------------------------------------------------------------------------------

delay:		; Delay for a short time.
			mov.w	#0000h, R4				; Set general register R4 to 0.
delay_loop:
			inc.w	R4						; Increment R4.
			cmp		#0FFFFFFh, R4			; Check if R4 is full.
			jne		delay_loop

			ret								; Return from subroutine.

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
            
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            
