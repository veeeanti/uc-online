using System;
using System.Threading;
using uc_online;

namespace uc_onlineLauncher
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("uc-online Launcher");
            Console.WriteLine("===================\n");

            using (var uc_online = new uc_online.uc_online())
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

                    uc_online.GetLogger().Log("Now starting uc-online initialization");
                    uc_online.GetLogger().Log($"AppID set to: {uc_online.GetCurrentAppID()}");

                    if (!uc_online.Initializeuc_online())
                    {
                        uc_online.GetLogger().LogError("uc-online initialization failed");
                        Console.WriteLine("Failed to initialize Steam");
                        return;
                    }

                    Console.WriteLine("Steam initialized successfully!");

                    if (!string.IsNullOrEmpty(uc_online.GetGameExecutable()))
                    {
                        Console.WriteLine("Attempting to launch configured game...");
                        if (uc_online.LaunchGame())
                        {
                            Console.WriteLine("Game launched! Press any key to continue...");
                            Console.ReadKey();
                            return;
                        }
                    }
                    else
                    {
                        Console.WriteLine("No game executable configured in config.ini");
                        Console.WriteLine("You need to set a game's executable in the config.ini for this to work. There's no default exe.");
                    }

                    Console.WriteLine("Game should now launch and work fine through Steam.");

                    for (int i = 0; i < 5; i++)
                    {
                        if (uc_online.IsSteamInitialized())
                        {
                            uc_online.RunSteamCallbacks();
                        }
                        Thread.Sleep(1000);
                        Console.WriteLine($"Game running... ({i + 1}/5 seconds)");
                    }

                    Console.WriteLine("\nThis window is now safe to close.");
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"An error occurred: {ex.Message}");
                }
            }

            Console.WriteLine("Done!");
        }
    }
}