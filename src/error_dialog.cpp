#include "error_dialog.hpp"
#include <iostream>
#include <cstdlib>
#include <string>

void ShowErrorMessage(const std::string& title, const std::string& message) {
    // Print error message to standard error stream
    std::cerr << "ERROR: " << title << std::endl;
    std::cerr << message << std::endl;

    // Try to show a graphical error message using xmessage (if available)
    std::string command = "xmessage -center -title \"" + title + "\" \"" + message + "\" 2>/dev/null";
    int result = system(command.c_str());

    // If xmessage failed (e.g., not available), try zenity (if available)
    if (result != 0) {
        command = "zenity --error --title=\"" + title + "\" --text=\"" + message + "\" 2>/dev/null";
        system(command.c_str());
    }
}
