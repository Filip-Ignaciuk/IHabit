#include "HabitTrackerManager.hpp"
#include "HabitTracker.hpp"
#include "HabitTrackerSerializer.hpp"
#include <filesystem>
#include <iostream>
#include <set>

std::set<HabitTracker> HabitTrackerManager::habit_trackers;

const std::set<HabitTracker>& HabitTrackerManager::LoadHabitTrackers(
    std::filesystem::path root_path){
    if(!std::filesystem::exists(root_path)){
        std::cout << "File path specified doesn't exist.\n";
        return habit_trackers;
    }
    const std::string json_extension{".json"};
    for (const std::filesystem::directory_entry& entry : 
        std::filesystem::directory_iterator(root_path)){
        if(!entry.exists()){
            std::cout << "Entry doesn't exist.\n";
            continue;
        }
        if(json_extension != entry.path().extension()){
            std::cout << "Entry isn't a json file.\n";
            continue;
        }
        std::filesystem::path file_path = entry.path();
        HabitTracker* habit = HabitTrackerSerializer::LoadHabitTracker(file_path);
        if(!habit){
            // Something wrong happened with the loading of the habit tracker.
            continue;
        }
        habit_trackers.emplace(*habit);
        delete habit;
    }
    return habit_trackers;
}