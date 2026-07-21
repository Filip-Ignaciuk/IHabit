#ifndef IHABIT_APP_HPP
#define IHABIT_APP_HPP
#include <map>
#include <raylib.h>
#include <string>
#include <vector>

#include "GridMaker.hpp"

class HabitTracker;
class Square;

namespace IHabitApp {
    // GUI
    #define reference_width_resolution 1280.0f
    #define reference_height_resolution 720.0f

    enum class WindowGuiState{
        None,
        SettingsBox,
        HabitsBox,
        HelpBox
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


        // Adding Habit data
        std::string* new_habit_title_;
        std::string* new_habit_description_;
        std::string* new_habit_metric_;
        std::map<double, int>* new_habit_threshold_colours_;
        std::map<std::string, double>* new_habit_data_;

        // Viewing Habit data
        const HabitTracker* current_habit_;

        void RefreshHabitUIStates();

        static std::string GetDropDownBoxTitle(std::vector<std::string> years);

        void ShowSettingsMenu();
        void ShowHabitsMenu();
        void ShowHelpMenu();
    };
}



#endif
