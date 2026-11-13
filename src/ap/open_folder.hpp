#pragma once

// Shows a folder in the system's file browser (Explorer, Finder, or whatever xdg-open picks).
// Kept apart from the game's headers, which don't get along with windows.h.

#include <filesystem>

namespace ap {

// Creates the folder first if it's missing. False if it couldn't be shown.
bool open_folder(const std::filesystem::path& folder);

}  // namespace ap
