#ifndef HABIT_TRACKER_MANAGER_HPP
#define HABIT_TRACKER_MANAGER_HPP

#include "HabitTracker.hpp"
#include <filesystem>
#include <set>

class HabitTrackerManager{
    public:
    static void LoadHabitTrackers(std::filesystem::path root_path);
    // If habit tracker with same title already exists it will overwrite it.
    static const std::set<HabitTracker>& GetHabitTrackers();
    static void AddHabitTracker(HabitTracker habit_tracker);
    static void RemoveHabitTracker(const HabitTracker& habit_tracker);

    HabitTrackerManager() = delete;
    ~HabitTrackerManager() = delete;
    private:
    static std::set<HabitTracker> habit_trackers_;
};

#endif