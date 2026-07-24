#ifndef HABIT_TRACKER_MANAGER_HPP
#define HABIT_TRACKER_MANAGER_HPP

#include "HabitTracker.hpp"
#include <filesystem>

class HabitTrackerManager{
    public:
    static void LoadHabitTrackers(const std::filesystem::path& root_path);
    static void SaveHabitTrackers(const std::filesystem::path& root_path);

    // If habit tracker with same title already exists it will overwrite it.
    static const std::map<std::string, HabitTracker>& GetHabitTrackers();
    static void AddHabitTracker(HabitTracker habit_tracker);
    static void RemoveHabitTracker(const HabitTracker& habit_tracker);
    static void RecordDay(const std::string& title,
        const std::chrono::year_month_day* ymd,
        double value);

    HabitTrackerManager() = delete;
    ~HabitTrackerManager() = delete;
    private:
    static std::map<std::string, HabitTracker> habit_trackers_;
    static constexpr std::string json_extension = ".json";
};

#endif