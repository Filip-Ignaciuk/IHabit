#include "HabitTrackerSerializerFileSystem.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../../cmake-build-release/_deps/ierrormanage-src/include/ErrorManager.hpp"


std::vector<HabitTracker> HabitTrackerSerializerFileSystem::LoadHabitTrackers(const std::string& path) {
    std::filesystem::path root_path(path);
    std::vector<HabitTracker> habit_trackers;

    if(!std::filesystem::exists(root_path)){
        std::cout << "File path specified doesn't exist.\n";
        return habit_trackers;
    }

    for (const std::filesystem::directory_entry& entry :
        std::filesystem::directory_iterator(root_path)){
        std::cout << entry.path().string() << "\n";
        if(!entry.exists()){
            std::cout << "Entry doesn't exist.\n";
            continue;
        }
        if(json_extension != entry.path().extension()){
            std::cout << "Entry isn't a json file.\n";
            continue;
        }

        const std::filesystem::path& file_path = entry.path();
        std::ifstream input(file_path);

        if (!input.is_open()) {
            std::cout << "Can't open file.\n";
            continue;
        }
        std::stringstream buffer;
        buffer << input.rdbuf();
        std::string json_string = buffer.str();

        if (HabitTracker* habit_tracker = ParseHabitTracker(json_string)) {
            habit_trackers.emplace_back(*habit_tracker);
            delete habit_tracker;
        }
        input.close();
    }

    return habit_trackers;
}

void HabitTrackerSerializerFileSystem::SaveHabitTrackers(const std::string &path,
    const std::vector<HabitTracker>& habit_trackers) {
    // Every save completely overwrites the previous file if it existed,
    // or just creates a new file.
    if(!std::filesystem::exists(path)) {
        std::cout << "File path specified doesn't exist.\n";
        return;
    }

    for (const HabitTracker& habit_tracker : habit_trackers) {
        std::filesystem::path habit_path = std::filesystem::path(path) /
            habit_tracker.GetTitle();
        const std::filesystem::path file_path = habit_path.string() + ".json";
        std::ofstream output(file_path);

        if (!output.is_open()) {
            std::cout << "Failed to open file for writing: " << file_path << '\n';
            continue;
        }

        output << ConvertToJsonString(habit_tracker);

        if(output.fail()){
            std::cout << "Failed to save habit tracker to: " << path << '\n';
        }
        output.close();
    }
}