using System;
using uc_online;

namespace uc_onlineLauncher
{
    class Program64
    {
        static void Main(string[] args)
        {
            Console.WriteLine("uc-online Launcher - 64-bit");
            Console.WriteLine("============================");

            try
            {
                using (var uc_online = new uc_online64("config.ini"))
                {
                    Console.WriteLine("\nCurrent configuration:");
                    Console.WriteLine($"  AppID: {uc_online.GetCurrentAppID()}");
                    Console.WriteLine($"  Game Executable: {(string.IsNullOrEmpty(uc_online.GetGameExecutable()) ? "(not configured)" : uc_online.GetGameExecutable())}");
                    Console.WriteLine($"  Game Arguments: {(string.IsNullOrEmpty(uc_online.GetGameArguments()) ? "(none)" : uc_online.GetGameArguments())}");

                    Console.WriteLine($"\nUsing AppID: {uc_online.GetCurrentAppID()} (Spacewar)");
                    uc_online.GetLogger().Log("Now starting uc-online initialization");

                    if (uc_online.Initializeuc_online())
                    {
                        Console.WriteLine("Steam initialized successfully!");

                        for (int i = 0; i < 5; i++)
                        {
                            uc_online.RunSteamCallbacks();
                            System.Threading.Thread.Sleep(100);
                        }

                        if (!string.IsNullOrEmpty(uc_online.GetGameExecutable()))
                        {
                            if (uc_online.LaunchGame())
                            {
                                Console.WriteLine("Game launched successfully!");
                            }
                            else
                            {
                                Console.WriteLine("Failed to launch game.");
                            }
                        }

                        uc_online.Shutdownuc_online();
                        Console.WriteLine("This window is now safe to close.");
                    }
                    else
                    {
                        Console.WriteLine("Failed to initialize Steam");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
            }

            Console.WriteLine("\nPress any key to exit...");
            Console.ReadKey();
        }
    }
}