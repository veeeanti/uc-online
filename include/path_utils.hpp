#pragma once

#include <string>
#include <filesystem>
#include <windows.h>
#include <stdexcept>

class PathUtils {
public:
    // Get the directory where the executable is located
    static std::string GetExecutableDirectory() {
        static std::string cachedExeDir = InitializeExecutableDirectory();
        return cachedExeDir;
    }

    // Resolve a path relative to the executable directory
    // If relativePath is empty, returns empty string (allows optional file paths)
    // If relativePath is absolute, returns it unchanged
    // Otherwise, returns the path relative to the executable directory
    static std::string ResolveRelativeToExecutable(const std::string& relativePath) {
        if (relativePath.empty()) {
            return relativePath;
        }

        // If the path is already absolute, return it as-is
        std::filesystem::path path(relativePath);
        if (path.is_absolute()) {
            return relativePath;
        }

        // Otherwise, make it relative to the executable directory
        std::filesystem::path exeDir = GetExecutableDirectory();
        std::filesystem::path fullPath = exeDir / relativePath;
        return fullPath.string();
    }

private:
    // Initialize the executable directory (called once via static initialization)
    static std::string InitializeExecutableDirectory() {
        char buffer[MAX_PATH];
        DWORD result = GetModuleFileNameA(NULL, buffer, MAX_PATH);
        if (result == 0) {
            throw std::runtime_error("Failed to get executable path: GetModuleFileNameA failed");
        }
        // Check for buffer overflow: if result == MAX_PATH, the buffer may have been truncated
        // In this case, GetLastError() will return ERROR_INSUFFICIENT_BUFFER
        if (result == MAX_PATH && GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
            throw std::runtime_error("Failed to get executable path: path too long");
        }
        std::filesystem::path exePath(buffer);
        return exePath.parent_path().string();
    }
};
