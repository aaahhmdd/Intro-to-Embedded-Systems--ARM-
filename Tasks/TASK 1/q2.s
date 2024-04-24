
			AREA RESET, DATA, READONLY 
			EXPORT __Vectors
__Vectors
			DCD 0x20008000 
			DCD Reset_Handler
			align


			AREA mydata, data, Readwrite
			align

			
			AREA myCode, CODE, ReadOnly
			ENTRY
			EXPORT Reset_Handler

		
Reset_Handler

		LDR R0,=0x20000004;
		MOV R1,#0; value of j
		MOV R3,#0; value of i
FOR		CMP R3,#15; i<15
		BGE EXIT
		ADD R1,R1,R3;
		ADD R3,R3,#1;
		B   FOR
EXIT	STR R1,[R0];
		end