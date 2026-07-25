#include "HabitTrackerSerializerIndexedDB.hpp"
#include "HabitTrackerSerializerFileSystem.hpp"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <fstream>
#include "json.hpp"
#include <iostream>
#endif

std::vector<HabitTracker> HabitTrackerSerializerIndexedDB::LoadHabitTrackers(
        const std::string& path) {
        std::vector<HabitTracker> habit_trackers;
#ifdef __EMSCRIPTEN__
        // Get the JSON string from localStorage
        char* result = (char*)emscripten_run_script_string(
            "localStorage.getItem('all_habits') || '[]'"
        );
        std::string json_string = std::string(result);

        try {
                auto all_habits_json = nlohmann::json::parse(json_string);

                // Parse each habit
                for (const auto& habit_json : all_habits_json) {
                        std::string habit_string = habit_json.dump();
                        if (HabitTracker* habit = ParseHabitTracker(habit_string)) {
                                habit_trackers.emplace_back(*habit);
                                delete habit;
                        }
                }
        } catch (const nlohmann::json::parse_error& e) {
                std::cout << "Failed to parse habits from localStorage: " << e.what() << '\n';
        }
#endif
        return habit_trackers;
}

void HabitTrackerSerializerIndexedDB::SaveHabitTrackers(const std::string& path,
        const std::vector<HabitTracker>& habit_trackers) {
#ifdef __EMSCRIPTEN__
        nlohmann::json all_habits = nlohmann::json::array();

        for (const auto& habit : habit_trackers) {
                all_habits.push_back(nlohmann::json::parse(ConvertToJsonString(habit)));
        }

        std::string json_string = all_habits.dump();
        EM_ASM({
            localStorage.setItem('all_habits', UTF8ToString($0));
        }, json_string.c_str());
#endif
}