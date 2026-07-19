#ifndef HABIT_TRACKER_MANAGER_HPP
#define HABIT_TRACKER_MANAGER_HPP

#include "HabitTracker.hpp"
#include <filesystem>
#include <set>

class HabitTrackerManager{
    public:
    static const std::set<HabitTracker>& LoadHabitTrackers(std::filesystem::path root_path);
    // If habit tracker with same title already exists it will overwrite it.
    static void AddHabitTracker(HabitTracker habit_tracker);
    static void RemoveHabitTracker(HabitTracker habit_tracker);
    private:
    static std::set<HabitTracker> habit_trackers;
    HabitTrackerManager();
    ~HabitTrackerManager();
    
};

#endif