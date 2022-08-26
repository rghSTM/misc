- **Object Manager**: provides kernel mode resource management - processes, files, registry keys, mutexes, Defender supports 5 types of objects File, Thread, Event, Mutant (Mutex), Semaphore
  - Object are stored in a map tracked by PID & Handle.
```c
// Windows Debugger
dt mpengine!ObjectManager::FileObject
dt mpengine!ObjectManager::Object
dt mpengine!ObjectManager::MutantObject
```

- MpReportEvent: Used to communicate informations about malware binary actions with Defender's heuristic detection engine
```c
// pseudo generated code
if (ProgramPath
	&& !memicmp(ProgramPath, "C:\\WINDOWS\\system32\\cmd.exe"
	&& !memicmp(ProgramPath + 31, "C:\\malicious.exe", 0xCu) )
{
	if ( Str1 )
		MpReportEvent(12312, Str1, ProgramPath);
	else
		MpReportEvent(12312, ProgramPath, 0);
	Str1 = 0;
	ProgramPath += 31;
}
```


- apicall stubs are located throughout VDLL,
  - parse stub in VDLL

Sandbox Evasion :  https://github.com/gh0stfaceee/misc/blob/main/Windows%20Defender%20Sandbox%20Evasion/check.md
