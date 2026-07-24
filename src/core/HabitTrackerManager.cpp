#include "HabitTrackerManager.hpp"
#include "HabitTracker.hpp"
#include "HabitTrackerSerializer.hpp"
#include <filesystem>
#include <iostream>
#include <set>

std::map<std::string, HabitTracker> HabitTrackerManager::habit_trackers_;

void HabitTrackerManager::LoadHabitTrackers(
    const std::filesystem::path& root_path){
    if(!std::filesystem::exists(root_path)){
        std::cout << "File path specified doesn't exist.\n";
        return;
    }
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
        const std::filesystem::path& file_path = entry.path();
        HabitTracker* habit = HabitTrackerSerializer::LoadHabitTracker(file_path);
        if(!habit){
            // Something wrong happened with the loading of the habit tracker.
            continue;
        }
        habit_trackers_.emplace(habit->GetTitle(), *habit);
        delete habit;
    }
}

void HabitTrackerManager::SaveHabitTrackers(const std::filesystem::path& root_path) {
    if(!std::filesystem::exists(root_path)) {
        std::cout << "File path specified doesn't exist.\n";
        return;
    }
    for (const std::pair<const std::string, HabitTracker>& habit_tracker : habit_trackers_) {
        std::filesystem::path habit_path = root_path / habit_tracker.first;
        const std::filesystem::path file_path = habit_path.string() + ".json";
        HabitTrackerSerializer::SaveHabitTracker(file_path, habit_tracker.second);
    }
}

const std::map<std::string, HabitTracker>& HabitTrackerManager::GetHabitTrackers(){
    return habit_trackers_;
}

void HabitTrackerManager::AddHabitTracker(HabitTracker habit_tracker){
    habit_trackers_[habit_tracker.GetTitle()] = std::move(habit_tracker);
}

void HabitTrackerManager::RemoveHabitTracker(const HabitTracker& habit_tracker){
    habit_trackers_.erase(habit_tracker.GetTitle());
}

void HabitTrackerManager::RecordDay(const std::string& title,
    const std::chrono::year_month_day* ymd,
    const double value) {
    habit_trackers_[title].AddDay(*ymd, value);
}