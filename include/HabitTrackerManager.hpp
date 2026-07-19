#ifndef HABIT_TRACKER_MANAGER_HPP
#define HABIT_TRACKER_MANAGER_HPP

#include "HabitTracker.hpp"
#include <filesystem>
#include <set>

class HabitTrackerManager{
    public:
    static const std::set<HabitTracker>& LoadHabitTrackers(std::filesystem::path root_path);
    private:
    static std::set<HabitTracker> habit_trackers;
    HabitTrackerManager();
    ~HabitTrackerManager();
    
};

#endif