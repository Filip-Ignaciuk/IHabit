#ifndef HABIT_TRACKER_SERIALIZER_HPP
#define HABIT_TRACKER_SERIALIZER_HPP

#include "HabitTracker.hpp"
#include <filesystem>

class HabitTrackerSerializer{
    public:
    static HabitTracker* LoadHabitTracker(std::filesystem::path path);
    static void SaveHabitTracker(std::filesystem::path path, const HabitTracker& habit_tracker);

    private:
        HabitTrackerSerializer() = delete;
        ~HabitTrackerSerializer() = delete;
};

#endif