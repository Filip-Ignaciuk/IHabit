#include "HabitTracker.hpp"
#include "HabitTrackerManager.hpp"
#include "raylib.h"
#include <cstddef>
#include <filesystem>
#include <set>
#include <string>
#include <vector>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <chrono>

#include <map>

// GUI
#define reference_width_resolution 1280.0f
#define reference_height_resolution 720.0f

// Ensures that only one box can be open at a time.
enum class WindowGuiState{
    None,
    SettingsBox,
    HabitsBox,
    HelpBox
};

WindowGuiState window_gui_state;

const Rectangle standard_box{16, 80, 1248, 624};

// Habits Box GUI
const HabitTracker* current_habit = nullptr;



void ShowSettingsMenu();
void ShowHabitsMenu();
void ShowHelpMenu();

std::set<HabitTracker> habits;

int main()
{
    InitWindow(1280, 720, "Habit Tracker");
    float scale = GetScreenWidth() / reference_width_resolution;  // Could be 0.87, 1.43, etc.

    // Data for creating a new habit.
    std::string title;
    std::string description;
    std::string metric;
    std::map<double, int> threshold_colours;
    std::map<std::string, double> data;

    // Gui data
    Rectangle scroll_panel_view = { 0, 0, 0, 0 };
    Vector2 scroll_panel_offset = { 0, 0 };
    Vector2 scroll_panel_bounds = { 0, 0 };
    std::map<HabitTracker*, std::vector<Rectangle*>> grids;
    std::filesystem::path path;
    habits = HabitTrackerManager::LoadHabitTrackers(path);

    

    while (!WindowShouldClose())
    {

        BeginDrawing();
            ClearBackground(RAYWHITE);
            //GuiScrollPanel(Rectangle bounds, empty, Rectangle content, *scroll, Rectangle *view);

            


            // Settings button
            if(GuiButton({16 * scale, 16 * scale, 32 * scale, 32 * scale}, "#142#")){
                window_gui_state = (window_gui_state == WindowGuiState::SettingsBox) 
                ? WindowGuiState::None : WindowGuiState::SettingsBox;
            }

            // Habit management button
            if(GuiButton({64 * scale, 16 * scale, 32 * scale, 32 * scale}, "#214#")){
                window_gui_state = (window_gui_state == WindowGuiState::HabitsBox) 
                ? WindowGuiState::None : WindowGuiState::HabitsBox;
            }

            // Help button
            if(GuiButton({112 * scale, 16 * scale, 32 * scale, 32 * scale}, "#193#")){
                window_gui_state = (window_gui_state == WindowGuiState::HelpBox) 
                ? WindowGuiState::None : WindowGuiState::HelpBox;
            }

            int xPosition;
            int yPosition;
            for(HabitTracker habit : habits){
                // Title
                //GuiLabel({1,1,100,10}, habit->GetTitle().c_str());
                // Description
                //GuiLabel(Rectangle bounds, habit->GetDescription().c_str());
                // Metric
                //GuiLabel(Rectangle bounds, habit->GetMetric().c_str());
                // Grid
                /* std::vector<Rectangle*> grid = grids[habit];
                for(int i {0}; i < 53; ++i){
                        std::chrono::year_month_day first_day{
                            std::chrono::year{2024}, 
                            std::chrono::January, 
                            std::chrono::day{1}};
                        std::chrono::weekday wd{std::chrono::sys_days{first_day}};
                        wd.iso_encoding();
                } */

            }

            GuiScrollPanel((Rectangle){ 0, 64, 1280 - scroll_panel_bounds.x, 656 - scroll_panel_bounds.y}, NULL, (Rectangle){ 0, 0, 1264, 720 }, &scroll_panel_offset, &scroll_panel_view);


            if(window_gui_state == WindowGuiState::SettingsBox){
                ShowSettingsMenu();
            }
            if(window_gui_state == WindowGuiState::HabitsBox){
                ShowHabitsMenu();
            }
            if(window_gui_state == WindowGuiState::HelpBox){
                ShowHelpMenu();
            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

inline void ShowSettingsMenu(){

}

inline void ShowHabitsMenu(){
    Rectangle habit_panel = { standard_box.x, standard_box.y + 24, standard_box.width, standard_box.height - 24 };
    Rectangle content = { 0, 0, 672, (float)(40 * habits.size()) };
    Rectangle view;
    Vector2 habit_panel_offset = { 0, 0 };


    if(GuiWindowBox(standard_box, "Habits")){
        window_gui_state = WindowGuiState::None;
        current_habit = nullptr;
    }
    else{
        window_gui_state = WindowGuiState::HabitsBox;
    }

    GuiScrollPanel(habit_panel, NULL, content, &habit_panel_offset, &view);
    /*
    BeginScissorMode(view.x, view.y, view.width, view.height);
    Vector2 initial_habit_position = {habit_panel.x, habit_panel.y + habit_panel_offset.y};
 
    auto begin = habits.begin();
    for(auto iterator{begin}; iterator != habits.end();){
        Rectangle habit_panel {initial_habit_position.x, initial_habit_position.y, 624, 48};
        Rectangle name_panel {habit_panel.x + 8, habit_panel.y + 8, 232, 24};
        Rectangle view_button_panel {habit_panel.x + 560, habit_panel.y + 8, 24, 24};
        Rectangle delete_button_panel {habit_panel.x + 592, habit_panel.y + 8, 24, 24};
        
        DrawRectangle(habit_panel.x, habit_panel.y, habit_panel.width, habit_panel.height, LIGHTGRAY);
        DrawRectangleLines(habit_panel.x, habit_panel.y, habit_panel.width, habit_panel.height, DARKGRAY);
        GuiLabel(name_panel, iterator->GetTitle().c_str());

        if(GuiButton(view_button_panel, "#42#")){
            current_habit = &(*iterator);
        }

        if(GuiButton(delete_button_panel, "#143#")){
            iterator = habits.erase(iterator);
        }
        else{
            iterator++;
        }
        initial_habit_position.y += 40;
    }
   
    EndScissorMode();
     */
}

inline void ShowHelpMenu(){
    
}

std::vector<Rectangle*> GenerateGrid(){
    
}