#include "HabitTrackerManager.hpp"
#include "HabitTracker.hpp"
#include "HabitTrackerSerializer.hpp"
#include <filesystem>
#include <iostream>
#include <set>

std::map<std::string, HabitTracker> HabitTrackerManager::habit_trackers_;

void HabitTrackerManager::LoadHabitTrackers(
    const std::filesystem::path& root_path,
    const std::unique_ptr<HabitTrackerSerializer>& habit_tracker_serializer){
    std::vector<HabitTracker> habit_trackers =
        habit_tracker_serializer->LoadHabitTrackers(root_path);
    habit_trackers_.clear();
    for (const auto& habit_tracker : habit_trackers) {
        habit_trackers_.emplace(habit_tracker.GetTitle(), habit_tracker);
    }

}

void HabitTrackerManager::SaveHabitTrackers(const std::filesystem::path& root_path,
    const std::unique_ptr<HabitTrackerSerializer>& habit_tracker_serializer) {
    std::vector<HabitTracker> habit_trackers;
    habit_trackers.reserve(habit_trackers_.size());
    for (const auto& data_pair : habit_trackers_) {
        habit_trackers.emplace_back(data_pair.second);
    }
    habit_tracker_serializer->SaveHabitTrackers(root_path, habit_trackers);

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