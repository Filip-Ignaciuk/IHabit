#ifndef IHABIT_HABIT_TRACKER_SERIALIZER_FILESYSTEM_HPP
#define IHABIT_HABIT_TRACKER_SERIALIZER_FILESYSTEM_HPP

#include "HabitTrackerSerializer.hpp"

class HabitTrackerSerializerFileSystem : public HabitTrackerSerializer {
     public:
     std::vector<HabitTracker> LoadHabitTrackers(const std::string& path) override;
     void SaveHabitTrackers(const std::string& path,
        const std::vector<HabitTracker>& habit_trackers) override;
};

#endif
