#define DEREF( name )*(UINT_PTR *)(name)
#define DEREF_64( name )*(DWORD64 *)(name)
#define DEREF_32( name )*(DWORD *)(name)
#define DEREF_16( name )*(WORD *)(name)
#define DEREF_8( name )*(BYTE *)(name)

FARPROC WINAPI GetForwardedProcAddress(char* forwardStr) {
	HMODULE hDll = NULL;
	FARPROC pProc = NULL;
	size_t size = strlen((char*)forwardStr);
	char* tmp = (char*)malloc(size + 1);
	memset(tmp, 0, size + 1);
	memcpy_s(tmp, size, (char*)forwardStr, size);

	char del[] = ".";
	char* dllToken = NULL;
	char* nextToken = NULL;
	char* funcToken = NULL;
	wchar_t* wDll = NULL;

	dllToken = strtok_s((char*)tmp, del, &nextToken);
	if (dllToken != NULL)
	{
		funcToken = strtok_s(NULL, del, &nextToken);
		if (strlen(nextToken) > 0)
		{
			printf(":)");
		}

	}
	if (dllToken != NULL && funcToken != NULL)
	{
		wDll = utf8_to_wchar(dllToken);
		hDll = hlpGetModuleHandle(wDll);

		if (hDll == NULL)
		{
			hDll = LoadLibraryW(wDll);
		}
		//We got it
		if (hDll)
		{
			pProc = hlpGetProcAddress(hDll, funcToken);
		}

		free(wDll);

	}


	free(tmp);
	return pProc;
}


FARPROC WINAPI hlpGetProcAddress(HMODULE hModule, LPCSTR lpProcName) {
	UINT64 dllAddress = (UINT64)hModule,
		symbolAddress = 0,
		exportedAddressTable = 0,
		namePointerTable = 0,
		ordinalTable = 0;

	if (hModule == NULL) {
		return 0;
	}

	PIMAGE_NT_HEADERS ntHeaders             = NULL;
	PIMAGE_DATA_DIRECTORY dataDirectory     = NULL;
	PIMAGE_EXPORT_DIRECTORY exportDirectory = NULL;

	ntHeaders  = (PIMAGE_NT_HEADERS)(dllAddress + ((PIMAGE_DOS_HEADER)dllAddress)->e_lfanew);
	dataDirectory  = (PIMAGE_DATA_DIRECTORY)&ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	exportDirectory = (PIMAGE_EXPORT_DIRECTORY)(dllAddress + dataDirectory->VirtualAddress);

	UINT64 orig_exportedAddressTable = (dllAddress + exportDirectory->AddressOfFunctions);
	exportedAddressTable = (dllAddress + exportDirectory->AddressOfFunctions);
	namePointerTable = (dllAddress + exportDirectory->AddressOfNames);
	ordinalTable = (dllAddress + exportDirectory->AddressOfNameOrdinals);

	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(ntHeaders);
	PIMAGE_SECTION_HEADER textSection = section;
	PIMAGE_SECTION_HEADER rdataSection = section;

	std::string fncR = make_string(".rdata"); 
	for (int i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++)
	{
		if (_strcmpi((CHAR*)section->Name, (CHAR*)fncR.c_str()) == 0) {
			rdataSection = section;
			break;
		}
		section++;
	}
	
	if (((UINT64)lpProcName & 0xFFFF0000) == 0x00000000) {
		exportedAddressTable += ((IMAGE_ORDINAL((UINT64)lpProcName) - exportDirectory->Base) * sizeof(DWORD));
		symbolAddress = (UINT64)(dllAddress + DEREF_32(exportedAddressTable));
	}
	else {
		DWORD dwCounter = exportDirectory->NumberOfNames;
		while (dwCounter--) {
			char* cpExportedFunctionName = (char*)(dllAddress + DEREF_32(namePointerTable));
			if (_stricmp(cpExportedFunctionName, lpProcName) == 0) {
				exportedAddressTable += (DEREF_16(ordinalTable) * sizeof(DWORD));
				symbolAddress = (UINT64)(dllAddress + DEREF_32(exportedAddressTable));

				if (dataDirectory->VirtualAddress <= DEREF_32(exportedAddressTable))
				{
				
					if (strstr((char*)symbolAddress, "api-ms-win") != NULL)
					{
						symbolAddress = (UINT64)GetProcAddress(hModule, lpProcName);
					}
					else
					{
						symbolAddress = (UINT64)GetForwardedProcAddress((char*)symbolAddress);
					}
					


				}
				
				break;
			}
			namePointerTable += sizeof(DWORD);
			ordinalTable += sizeof(WORD);
		}
	}
	return (FARPROC)symbolAddress;
}
