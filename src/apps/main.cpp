#include "HabitTracker.hpp"
#include "HabitTrackerManager.hpp"
#include "raylib.h"
#include <cstddef>
#include <filesystem>
#include <iostream>
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
const Rectangle standard_box_inner{16, 104, 1248, 600};

// Habits Box GUI
const HabitTracker* current_habit = nullptr;



void ShowSettingsMenu();
void ShowHabitsMenu();
void ShowHelpMenu();

std::set<HabitTracker> habits;

// Data for creating a new habit.
std::string title;
std::string description;
std::string metric;
std::map<double, int> threshold_colours;
std::map<std::string, double> data;

int main()
{
    InitWindow(1280, 720, "Habit Tracker");

    // Gui data
    Rectangle scroll_panel_view = { 0, 0, 0, 0 };
    Vector2 scroll_panel_offset = { 0, 0 };
    Vector2 scroll_panel_bounds = { 0, 0 };
    std::map<HabitTracker*, std::vector<Rectangle*>> grids;
    std::filesystem::path path = std::filesystem::current_path();
    path /= "data";
     for (const std::filesystem::directory_entry& entry : 
        std::filesystem::directory_iterator(path)){
            std::cout << entry << "\n";
        }
    habits = HabitTrackerManager::LoadHabitTrackers(path);

    while (!WindowShouldClose())
    {

        BeginDrawing();
            ClearBackground(RAYWHITE);
            //GuiScrollPanel(Rectangle bounds, empty, Rectangle content, *scroll, Rectangle *view);

            // Settings button
            if(GuiButton({16, 16, 32, 32}, "#142#")){
                window_gui_state = (window_gui_state == WindowGuiState::SettingsBox) 
                ? WindowGuiState::None : WindowGuiState::SettingsBox;
            }

            // Habit management button
            if(GuiButton({64, 16, 32, 32}, "#214#")){
                window_gui_state = (window_gui_state == WindowGuiState::HabitsBox) 
                ? WindowGuiState::None : WindowGuiState::HabitsBox;
            }

            // Help button
            if(GuiButton({112, 16, 32, 32}, "#193#")){
                window_gui_state = (window_gui_state == WindowGuiState::HelpBox) 
                ? WindowGuiState::None : WindowGuiState::HelpBox;
            }


            

            GuiScrollPanel((Rectangle){ 0, 64, 1280 - scroll_panel_bounds.x, 656 - scroll_panel_bounds.y}, NULL, (Rectangle){ 0, 0, 1264, 720 }, &scroll_panel_offset, &scroll_panel_view);

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
    Rectangle settings_panel = { standard_box_inner.x, standard_box_inner.y, standard_box_inner.width, standard_box_inner.height};
    Rectangle content = { 0, 0, 672, (float)(40 * habits.size()) };
    Rectangle view;
    Vector2 habit_panel_offset = { 0, 0 };

    window_gui_state = (GuiWindowBox(standard_box, "Settings")) ? 
    WindowGuiState::None : WindowGuiState::SettingsBox; 

}

inline void ShowHabitsMenu(){
    Rectangle habit_view_panel = { standard_box_inner.x, standard_box_inner.y, standard_box_inner.width / 2, standard_box_inner.height};
    Rectangle habit_information_panel = { standard_box_inner.x + standard_box_inner.width / 2, standard_box_inner.y, standard_box_inner.width / 2, standard_box_inner.height};
    // TO UNDERSTAND: Have to negate 2 in order for lower scroll bar to not be present.
    Rectangle content = { 0, 0, habit_view_panel.width - 2, (float)(40 * habits.size()) };
    Rectangle view;
    Vector2 habit_panel_offset = { 0, 0 };


    window_gui_state = (GuiWindowBox(standard_box, "Habits")) ? 
    WindowGuiState::None : WindowGuiState::HabitsBox; 

    GuiScrollPanel(habit_view_panel, NULL, content, &habit_panel_offset, &view);

    BeginScissorMode(view.x, view.y, view.width, view.height);
    Vector2 initial_habit_position = {habit_view_panel.x, habit_view_panel.y + habit_panel_offset.y};
 
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
    if(current_habit !=nullptr){
        std::string title = "Title: " + current_habit->GetTitle();
        GuiLabel({habit_information_panel.x + 8, habit_information_panel.y + 8, habit_information_panel.width, 24}, title.c_str());
        std::string metric = "Metric: " + current_habit->GetMetric();
        GuiLabel({habit_information_panel.x + 8, habit_information_panel.y + 40, habit_information_panel.width, 24}, metric.c_str());
        std::string description = "Description: " + current_habit->GetDescription();
        GuiLabel({habit_information_panel.x + 8, habit_information_panel.y + 72, habit_information_panel.width, 24}, description.c_str());
        std::map<double, std::string> colour_thresholds = current_habit->GetThresholdColours();
        GuiLabel({habit_information_panel.x + 8, habit_information_panel.y + 104, habit_information_panel.width, 24}, "Colour Thresholds");

        int new_position { (int)habit_information_panel.y + 136 };

        for(std::pair<double, std::string> threshold : colour_thresholds){
            GuiLabel({habit_information_panel.x + 40, (float)new_position, habit_information_panel.width, 24}, std::to_string(threshold.first).c_str());
            std::string clean = threshold.second;
            if (clean[0] == '#'){
                clean = clean.substr(1);
            }
            unsigned int color = std::stoul(clean, nullptr, 16);

            DrawRectangle(habit_information_panel.x + 16, new_position + 8, 8, 8, {(unsigned char)((color >> 16) & 0xFF), (unsigned char)((color >> 8) & 0xFF), (unsigned char)(color & 0xFF), 255} );
            new_position += 32;
        }
        
    }
}

inline void ShowHelpMenu(){
    Rectangle settings_panel = { standard_box_inner.x, standard_box_inner.y, standard_box_inner.width, standard_box_inner.height};
    Rectangle content = { 0, 0, 672, (float)(40 * habits.size()) };
    Rectangle view;
    Vector2 habit_panel_offset = { 0, 0 };

    window_gui_state = (GuiWindowBox(standard_box, "Help")) ? 
    WindowGuiState::None : WindowGuiState::HelpBox; 
}

std::vector<Rectangle*> GenerateGrid(){
    
}