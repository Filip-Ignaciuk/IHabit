#ifndef HABIT_TRACKER_SERIALIZER_HPP
#define HABIT_TRACKER_SERIALIZER_HPP

#include "HabitTracker.hpp"
#include <filesystem>

class HabitTrackerSerializer{
    public:
    virtual std::vector<HabitTracker> LoadHabitTrackers(const std::string& path) = 0;
    virtual void SaveHabitTrackers(const std::string& path,
        const std::vector<HabitTracker>& habit_trackers) = 0;
    virtual ~HabitTrackerSerializer() = default;
protected:
    static HabitTracker* ParseHabitTracker(const std::string& json);
    static std::string ConvertToJsonString(const HabitTracker& habit_tracker);
    static constexpr std::string_view json_extension = ".json";
};

#endif