#pragma once

#include <string>
#include <filesystem>
#include <windows.h>

class PathUtils {
public:
    // Get the directory where the executable is located
    static std::string GetExecutableDirectory() {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        std::filesystem::path exePath(buffer);
        return exePath.parent_path().string();
    }

    // Resolve a path relative to the executable directory
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
