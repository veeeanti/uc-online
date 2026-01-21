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
        // Use a larger buffer to support long paths on Windows 10+
        char buffer[32768]; // Maximum path length on modern Windows
        DWORD result = GetModuleFileNameA(NULL, buffer, sizeof(buffer));
        if (result == 0) {
            throw std::runtime_error("Failed to get executable path: GetModuleFileNameA failed");
        }
        // If the buffer is exactly filled, it might indicate truncation
        // but in practice, this buffer should be more than sufficient
        if (result >= sizeof(buffer)) {
            throw std::runtime_error("Failed to get executable path: path too long");
        }
        std::filesystem::path exePath(buffer);
        return exePath.parent_path().string();
    }
};
