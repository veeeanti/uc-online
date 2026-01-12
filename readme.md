This version for Linux only uses one launcher to cover both x86 and x64 versions, and instead of using libsteam_api.so entirely, it uses the steamclient.so files in the sdk32 and sdk64 folders respectively when it detects which architecture the game's executable is.

Credit: ahmedvflame