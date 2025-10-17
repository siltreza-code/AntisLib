# AntisLib

AntisLib is a lightweight DLL library designed for **Windows** that provides various utility functions for use in [**Lua Love2D**](https://love2d.org/) or other languages capable of calling C-style DLLs. It’s intended for educational purposes, game development, or general experimentation — **not for invasive or harmful use**.

---

## Features

### System Info
- `GetUserNameC()` – Returns the current Windows username.  
- `GetHostNameC()` – Returns the computer’s hostname.  
- `GetOSVersionC()` – Returns the Windows version as a string.  
- `GetCPUCountC()` – Returns the number of CPU cores.  
- `GetCurrentDirectoryC()` – Returns the current working directory.  

### Network
- `GetIPAddressesC()` – Returns a comma-separated list of the local machine’s IP addresses.  
- `HttpGetC(url)` – Performs a simple HTTP GET request to the given URL.  

### Clipboard
- `GetClipboardTextC()` – Returns the current clipboard text.  
- `SetClipboardTextC(text)` – Sets the clipboard text.  

### Notifications
- `ShowNotificationC(title, message)` – Shows a simple Windows message box notification.  

### Sound
- `PlaySystemBeepC()` – Plays the standard system beep.  
- `PlayWavFileC(path)` – Plays a `.wav` file asynchronously.  

### Utilities
- `OpenURLC(url)` – Opens the given URL in the default browser.

---

## Usage in Lua Love2D

Example using `ffi`:

```lua
local ffi = require("ffi")

ffi.cdef[[
const char* GetUserNameC();
const char* GetHostNameC();
void OpenURLC(const char* url);
]]

local antis = ffi.load("AntisLib.dll")

print(ffi.string(antis.GetUserNameC()))
print(ffi.string(antis.GetHostNameC()))
antis.OpenURLC("https://example.com")
