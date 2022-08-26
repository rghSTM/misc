# misc
maldev en ASM et C


# Documentation

- Custom_func.c: GetProcAddress Custom 

- Adownload.asm : MASM

- Shellcoding_C2 montre de quoi est constitué un shellcode de cobalt strike, il utilise des fonctions de wininet pour créer des socket + support HTTP, pour faire vos shellcode pour vos C2, faite le en asm (voir Adownload.asm)

- avast hooks comporte tout les hooks d'avast, on peut utiliser un syscall stub avec le SSN, Convention d'appel et `syscall; ret` pour l'execution direct.

- SleepCase Comporte des techniques de sleeping, on peut aussi utiliser la méthode de Ekko 'Sleep Encryption' qui utilise un style de ROP chains (indirect execution) et `SystemFunction032`, pour la ROP chains qui est une indirect execution, on peut bypass CFG avec SetProcessValidCallTargets + ajouts d'exception



### DEFCON Map Blink Code Explanation

soon
