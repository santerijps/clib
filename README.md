# clib

Add the compiler option for includes:

```shell
# Link the libcurl-x64 DLL if needed
gcc -I/path/to/clib -L/path/to/clib/clib/Http -llibcurl-x64
```

Then include header files:

```c
#include <clib/Mime.h>
```
