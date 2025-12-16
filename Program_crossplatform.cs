using System;
using System.Threading;
using uc_online;

namespace uc_onlineLauncher
{
    class Program_crossplatform
    {
        static void Main(string[] args)
        {
            Console.WriteLine("uc-online Crossplatform Launcher");
            Console.WriteLine("=================================\n");

            // Display platform information
            DisplayPlatformInfo();

            using (var uc_online = new uc_online.uc_online_crossplatform())
            {
                try
                {
                    Console.WriteLine($"Current configuration:");
                    Console.WriteLine($"  AppID: {uc_online.GetCurrentAppID()}");
                    Console.WriteLine($"  Game Executable: {(string.IsNullOrEmpty(uc_online.GetGameExecutable()) ? "(not configured)" : uc_online.GetGameExecutable())}");
                    Console.WriteLine($"  Game Arguments: {(string.IsNullOrEmpty(uc_online.GetGameArguments()) ? "(none)" : uc_online.GetGameArguments())}");
                    Console.WriteLine();

                    uc_online.SetCustomAppID(480);
                    Console.WriteLine($"Using AppID: {uc_online.GetCurrentAppID()} (Spacewar)");

                    uc_online.GetLogger().Log("Now starting uc-online crossplatform initialization");
                    uc_online.GetLogger().Log($"AppID set to: {uc_online.GetCurrentAppID()}");
                    uc_online.GetLogger().Log($"Platform: {uc_online.GetCurrentPlatform()}");

                    // Display Steam installation path hint
                    string steamPath = uc_online.GetDefaultSteamInstallationPath();
                    if (!string.IsNullOrEmpty(steamPath))
                    {
                        Console.WriteLine($"Steam installation path: {steamPath}");
                    }

                    if (!uc_online.Initializeuc_online())
                    {
                        uc_online.GetLogger().LogError("uc-online crossplatform initialization failed");
                        Console.WriteLine("Failed to initialize Steam");
                        Console.WriteLine("\nTroubleshooting tips:");
                        Console.WriteLine("1. Make sure Steam is installed and running");
                        Console.WriteLine("2. Check that steam_appid.txt can be created");
                        Console.WriteLine("3. Verify Steam API libraries are available");
                        return;
                    }

                    Console.WriteLine("Steam initialized successfully!");

                    if (!string.IsNullOrEmpty(uc_online.GetGameExecutable()))
                    {
                        Console.WriteLine("Attempting to launch configured game...");
                        if (uc_online.LaunchGame())
                        {
                            Console.WriteLine("Game launched! Press any key to continue...");
                            
                            // Platform-specific input handling
                            if (IsUnixPlatform())
                            {
                                Console.ReadLine(); // Use ReadLine on Unix systems
                            }
                            else
                            {
                                Console.ReadKey();
                            }
                            return;
                        }
                    }
                    else
                    {
                        Console.WriteLine("No game executable configured in config.ini");
                        Console.WriteLine("You need to set a game's executable in the config.ini for this to work.");
                        
                        // Platform-specific executable extensions
                        string exeExtension = GetExecutableExtension();
                        if (!string.IsNullOrEmpty(exeExtension))
                        {
                            Console.WriteLine($"Example game path: /path/to/game{exeExtension}");
                        }
                    }

                    Console.WriteLine("Steam integration is ready. Game should work through Steam now.");

                    // Run Steam callbacks for a short time to demonstrate functionality
                    Console.WriteLine("Running Steam callbacks for demonstration...");
                    for (int i = 0; i < 5; i++)
                    {
                        if (uc_online.IsSteamInitialized())
                        {
                            uc_online.RunSteamCallbacks();
                        }
                        Thread.Sleep(1000);
                        Console.WriteLine($"Steam callbacks running... ({i + 1}/5 seconds)");
                    }

                    Console.WriteLine("\nThis window is now safe to close.");
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"An error occurred: {ex.Message}");
                    Console.WriteLine($"Stack trace: {ex.StackTrace}");
                    
                    uc_online.GetLogger().LogException(ex, "Launcher error");
                }
            }

            Console.WriteLine("Done!");
            
            // Platform-specific exit
            if (IsUnixPlatform())
            {
                Console.WriteLine("Press Enter to exit...");
                Console.ReadLine();
            }
            else
            {
                Console.WriteLine("Press any key to exit...");
                Console.ReadKey();
            }
        }

        static void DisplayPlatformInfo()
        {
            Console.WriteLine("Platform Information:");
            
            // Detect and display OS
            if (System.Runtime.InteropServices.RuntimeInformation.IsOSPlatform(
                System.Runtime.InteropServices.OSPlatform.Linux))
            {
                Console.WriteLine("  OS: Linux");
            }
            else if (System.Runtime.InteropServices.RuntimeInformation.IsOSPlatform(
                System.Runtime.InteropServices.OSPlatform.Windows))
            {
                Console.WriteLine("  OS: Windows");
            }
            else if (System.Runtime.InteropServices.RuntimeInformation.IsOSPlatform(
                System.Runtime.InteropServices.OSPlatform.OSX))
            {
                Console.WriteLine("  OS: macOS");
            }
            else
            {
                Console.WriteLine($"  OS: {System.Runtime.InteropServices.RuntimeInformation.OSDescription}");
            }

            // Display architecture
            Console.WriteLine($"  Architecture: {System.Runtime.InteropServices.RuntimeInformation.OSArchitecture}");
            Console.WriteLine($"  Process Architecture: {System.Runtime.InteropServices.RuntimeInformation.ProcessArchitecture}");
            Console.WriteLine($"  Framework: {System.Runtime.InteropServices.RuntimeInformation.FrameworkDescription}");
            Console.WriteLine();
        }

        static bool IsUnixPlatform()
        {
            return System.Runtime.InteropServices.RuntimeInformation.IsOSPlatform(
                System.Runtime.InteropServices.OSPlatform.Linux) ||
                   System.Runtime.InteropServices.RuntimeInformation.IsOSPlatform(
                System.Runtime.InteropServices.OSPlatform.OSX);
        }

        static string GetExecutableExtension()
        {
            if (System.Runtime.InteropServices.RuntimeInformation.IsOSPlatform(
                System.Runtime.InteropServices.OSPlatform.Windows))
            {
                return ".exe";
            }
            else if (System.Runtime.InteropServices.RuntimeInformation.IsOSPlatform(
                System.Runtime.InteropServices.OSPlatform.Linux) ||
                     System.Runtime.InteropServices.RuntimeInformation.IsOSPlatform(
                System.Runtime.InteropServices.OSPlatform.OSX))
            {
                return ""; // No extension on Unix-like systems
            }
            return "";
        }
    }
}