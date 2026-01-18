#include "uc_online64.hpp"
#include "error_dialog.hpp"
#include <thread>
#include <chrono>

int main() {
    UCOnline64 uc_online;

    try {
        uc_online.SetCustomAppID(480);
        uc_online.GetLogger()->Log("Now starting uc-online64 initialization");
        uc_online.GetLogger()->Log("Appid set to: " + std::to_string(uc_online.GetCurrentAppID()));

        if (!uc_online.InitializeUCOnline()) {
            uc_online.GetLogger()->LogError("uc-online64 initialization failed");
            ShowErrorMessage("Steam Initialization Error", "Failed to initialize Steam");
            return 1;
        }

        if (!uc_online.GetGameExecutable().empty()) {
            if (uc_online.LaunchGame()) {
                return 0;
            } else {
                uc_online.GetLogger()->LogError("Game launch failed");
                ShowErrorMessage("Game Launch Error", "Failed to launch the configured game executable");
            }
        } else {
            uc_online.GetLogger()->LogError("No game executable configured in config.ini");
            ShowErrorMessage("Configuration Error", "No game executable configured in config.ini file.\nYou need to set a game's executable in the config.ini to use this launcher.");
        }
    } catch (const std::exception& ex) {
        ShowErrorMessage("Error", std::string("An error occurred: ") + ex.what());
    }

    return 1;
}
