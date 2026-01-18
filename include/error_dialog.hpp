#pragma once

#include <string>

/**
 * Displays an error message box using GTK.
 * This function initializes GTK, shows a modal error dialog, and then cleans up.
 * 
 * @param title The title of the error dialog
 * @param message The error message to display
 */
void ShowErrorMessage(const std::string& title, const std::string& message);
