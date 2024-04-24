		AREA RESET, DATA, READONLY 
		EXPORT __Vectors
__Vectors
		DCD 0x20008000 
		DCD Reset_Handler
		ALIGN
			
		AREA myCode, CODE, ReadOnly
		ENTRY
		EXPORT Reset_Handler
			
Reset_Handle

array_size EQU 10
	
		    ; Initialize registers
		LDR R0, =array      ; Load the address of the array into r0
		MOV R1, #10         ; Initialize loop counter (number of elements in the array)
		MOV R2, #0          ; Initialize sum register
		
		LDR R4, =result     ; Load the address of the result variable into r3
sum_loop
		LDR R3, [R0], #4   ; Load the next element of the array into r3 and increment the array pointer
		ADD R2, R2, R3      ; Add the element to the sum in r2
		SUBS R1, R1, #1     ; Decrement the loop counter
		BNE sum_loop        ; Branch back to sum_loop if counter != 0

		STR R2, [R4]        ; Store the sum in the memory location reserved for the result
				
		
		ALIGN
		AREA myconstData,Data,Readonly
	
array   DCD  1, 2, 3, 4, 5, 6, 7, 8, 9, 10    ; Define array with 10 elements
	
		ALIGN
		AREA myVarData,Data,READWRITE 
result  SPACE 4                               ; Store the result
		
		END

