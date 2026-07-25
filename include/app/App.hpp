#ifndef IHABIT_APP_HPP
#define IHABIT_APP_HPP
#include <filesystem>
#include <map>
#include <raylib.h>
#include <string>
#include <vector>
#include <string_view>

#include "GridMaker.hpp"
#include "HabitTrackerSerializer.hpp"

namespace IApp {
    class Error;
}

class HabitTracker;
struct Square;

namespace IHabitApp {
    enum class WindowGuiState{
        None,
        SettingsBox,
        HabitsBox,
        HelpBox,
        RecordDayBox,
        AddHabitBox
    };

     inline Rectangle standard_box{
        .x = 16,
        .y = 80,
        .width = 1248,
        .height = 624
    };

    inline Rectangle standard_box_inner{
        .x = 16,
        .y = 104,
        .width = 1248,
        .height = 600
    };

    inline Rectangle small_box_inner{
        .x = 480,
        .y = 270,
        .width = 320,
        .height = 180
    };

    inline constexpr std::array<std::string_view, 7> weekdays = {
        "Sun", "Tue", "Wed", "Thu", "Fri", "Sat", "Mon"
    };

    struct HabitUIState {
        std::map<std::string, Grid> grids;
        bool drop_down_box_edit_mode = false;
        int drop_down_box_active = 0;
        std::vector<std::string> drop_down_box_years;

    };

    class IHabitApp {
    public:
        void Run();
        IHabitApp();
        ~IHabitApp();
    private:
        WindowGuiState window_gui_state_;
        std::map<std::string, HabitUIState> habit_ui_states_;

        std::vector<IApp::Error> errors_;

        std::filesystem::path current_path_ = std::filesystem::current_path();
        std::filesystem::path habit_path_ = std::filesystem::current_path() /= "data";
        std::unique_ptr<HabitTrackerSerializer> habit_tracker_serializer_;

        // Adding Habit data
        char new_habit_title_[20] = {};
        char new_habit_description_[200] = {};
        char new_habit_metric_[20] = {};
        std::map<double, std::string>* new_habit_threshold_colours_;
        Color new_threshold_colour_ = {};
        char new_threshold_value_[6] = {};

        bool new_habit_title_edit_mode_ = false;
        bool new_habit_description_edit_mode_ = false;
        bool new_habit_metric_edit_mode_ = false;
        bool new_habit_threshold_value_edit_mode_ = false;

        Vector2 new_habit_panel_offset_ = { .x = 0, .y = 0 };

        // Recording Day data
        std::string selected_habit_title_;
        std::string previous_selected_habit_title_;
        char new_record_day_[3] = {};
        char new_record_month_[3] = {};
        char new_record_year_[5] = {};
        char new_record_value_[6] = {};
        double new_record_value_double_ = 0.0;

        bool day_edit_mode_ = false;
        bool month_edit_mode_ = false;
        bool year_edit_mode_ = false;
        bool value_edit_mode_ = false;



        // Viewing Habit data
        const HabitTracker* current_habit_;
        Vector2 habit_panel_offset_ = { .x = 0, .y = 0 };

        // Settings
        bool wants_european_weekday_ = false;
        bool wants_weekday_displayed_ = false;

        void RefreshHabitUIStates();

        static std::string GetDropDownBoxTitle(const std::vector<std::string> &years);

        void ShowSettingsMenu();
        void ShowHabitsMenu();
        void ShowHelpMenu();
        void ShowRecordDayMenu();
        void ShowAddHabitMenu();

        // Returns pointer if valid else nullptr
        std::chrono::year_month_day* IsRecordInputValid();
        void RecordDay(const std::chrono::year_month_day* ymd);

        HabitTracker* IsAddHabitInputValid();
        void AddHabit(const HabitTracker* habit_tracker);

        double* IsThresholdValueValid() const;
    };
}



#endif
