# uc-online (Old C# Source Code, deprecated - not recommended for use)


## The only two things I ask of anyone who wants to use this code is that you leave the name the same (unless I allow otherwise) and do not add malware. with that being said, go nuts. make this better than what I can make of it. 
## - This doesn't apply anymore to this outdated code, it more so applies to the other more recently updated and better maintained branches / variants. If you are wanting to use this for any reason, it is yours my friend. I'm sticking to the C++ codebases and will never touch this one again.

## Building instructions:

### What's needed:
 - .NET SDK 8.0 (latest SDK will work fine, 8.0 is just the version specified)

### Actually building it:
 - Run the ``build.bat`` file after installing the .NET SDK.
 - Or, if you prefer, run in a cmd window ``dotnet build --configuration Release``

=======================================================

This is made mainly for use by Union-Crax team members for our releases, but can be used by whoever wants to use it. It should work with any game that uses "SteamAPI_Init" when launching in the dll exports. 
I have tested it with a handful of games (both single player and multiplayer) just to make sure it works as a universal tool.
 - Sonic Adventure 2*
 - Jet Set Radio
 - PropHunter
 - Rivals of Aether

*Sonic Adventure 2 required Steamless to be used on the ``sonic2app.exe`` and only worked when launching through ``Launcher.exe``. Steamless may not be required for games with SteamStub DRM when using uc-online, I haven't tested it yet fully.
