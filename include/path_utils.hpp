#pragma once

#include <string>
#include <filesystem>
#include <windows.h>
#include <stdexcept>

class PathUtils {
public:
    // Get the directory where the executable is located
    static std::string GetExecutableDirectory() {
        char buffer[MAX_PATH];
        DWORD result = GetModuleFileNameA(NULL, buffer, MAX_PATH);
        if (result == 0) {
            throw std::runtime_error("Failed to get executable path: GetModuleFileNameA failed");
        }
        if (result == MAX_PATH) {
            throw std::runtime_error("Failed to get executable path: path too long");
        }
        std::filesystem::path exePath(buffer);
        return exePath.parent_path().string();
    }

    // Resolve a path relative to the executable directory
    // Returns empty string if relativePath is empty (preserves original behavior)
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
};
