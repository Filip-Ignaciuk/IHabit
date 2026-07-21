#include "App.hpp"
#define RAYGUI_IMPLEMENTATION
#include <iostream>

#include "GridMaker.hpp"
#include "raygui.h"
#include "HabitTrackerManager.hpp"

void IHabitApp::IHabitApp::Run() {
    InitWindow(1280, 720, "Habit Tracker");

    // Gui data
    Rectangle scroll_panel_view = { 0, 0, 0, 0 };
    Vector2 scroll_panel_offset = { 0, 0 };
    Vector2 scroll_panel_bounds = { 0, 0 };

    while (!WindowShouldClose())
    {
        const size_t number_of_habits = HabitTrackerManager::GetHabitTrackers().size();
        BeginDrawing();

        ClearBackground(RAYWHITE);
        //GuiScrollPanel(Rectangle bounds, empty, Rectangle content, *scroll, Rectangle *view);

        // Settings button
        if(GuiButton({16, 16, 32, 32}, "#142#")){
            window_gui_state_ = (window_gui_state_ == WindowGuiState::SettingsBox)
            ? WindowGuiState::None : WindowGuiState::SettingsBox;
        }

        // Habit management button
        if(GuiButton({64, 16, 32, 32}, "#214#")){
            window_gui_state_ = (window_gui_state_ == WindowGuiState::HabitsBox)
            ? WindowGuiState::None : WindowGuiState::HabitsBox;
        }

        // Help button
        if(GuiButton({112, 16, 32, 32}, "#193#")){
            window_gui_state_ = (window_gui_state_ == WindowGuiState::HelpBox)
            ? WindowGuiState::None : WindowGuiState::HelpBox;
        }



        GuiScrollPanel((Rectangle){
            0,
            64,
            1280 - scroll_panel_bounds.x,
            656 - scroll_panel_bounds.y},
            nullptr,
            (Rectangle){ 0, 0, 1264, 720 },
            &scroll_panel_offset, &scroll_panel_view);

        BeginScissorMode(
            static_cast<int>(scroll_panel_view.x),
            static_cast<int>(scroll_panel_view.y),
            static_cast<int>(scroll_panel_view.width),
            static_cast<int>(scroll_panel_view.height));

        const Vector2 initial_habit_position = {
            .x = scroll_panel_view.x,
            .y = scroll_panel_view.y + scroll_panel_offset.y
        };

        const auto begin = HabitTrackerManager::GetHabitTrackers().begin();
        float count = 0.0f;
        for(auto iterator{begin};
            iterator != HabitTrackerManager::GetHabitTrackers().end();){
            const std::string& title = iterator->GetTitle();
            HabitUIState& habit_ui_state = habit_ui_states_[title];

            Rectangle habit_panel {
                .x = initial_habit_position.x,
                .y = initial_habit_position.y,
                .width = 624,
                .height = 48
            };


            GuiLabel({
                .x = initial_habit_position.x + 8,
                .y = initial_habit_position.y + 8 + ((192.0f) * count),
                .width = 1264,
                .height = 24},
                title.c_str());

            std::string drop_down_box_title;
            for (const std::string& year : (*iterator).GetAvailableYears()) {
                drop_down_box_title += year + ";";
            }

            GuiDropdownBox({
                .x = initial_habit_position.x + 1184,
                .y = initial_habit_position.y + 8 + ((192.0f) * count),
                .width = 72,
                .height = 24},
                drop_down_box_title.c_str(),
                &habit_ui_state.drop_down_box_active,
                habit_ui_state.drop_down_box_edit_mode);

            GuiLabel({
                .x = initial_habit_position.x + 8,
                .y = initial_habit_position.y + 40 + ((192.0f) * count),
                .width = 1264,
                .height = 24},
                iterator->GetMetric().c_str());

            const std::string& selectedYear =
                habit_ui_state.drop_down_box_years[habit_ui_state.drop_down_box_active];

            const std::vector<Square>& squares = habit_ui_state.grids.at(selectedYear);
            for(Square square : squares){
                DrawRectangle(
                    static_cast<int>(initial_habit_position.x + square.rectangle.x) + 8,
                    static_cast<int>(initial_habit_position.y + square.rectangle.y +
                        72 + ((192) * count)),
                    static_cast<int>(square.rectangle.width),
                    static_cast<int>(square.rectangle.height),
                    square.color);
            }

            ++iterator;
            ++count;
        }
        EndScissorMode();

        if(window_gui_state_ == WindowGuiState::SettingsBox){
            ShowSettingsMenu();
        }
        if(window_gui_state_ == WindowGuiState::HabitsBox){
            ShowHabitsMenu();
        }
        if(window_gui_state_ == WindowGuiState::HelpBox){
            ShowHelpMenu();
        }

        EndDrawing();
    }

    CloseWindow();
}

IHabitApp::IHabitApp::IHabitApp() {
    window_gui_state_ = WindowGuiState::None;

    std::filesystem::path path = std::filesystem::current_path();
    path /= "data";

    if (std::filesystem::exists(path)) {
        HabitTrackerManager::LoadHabitTrackers(path);
    }

    new_habit_title_ = nullptr;
    new_habit_description_ = nullptr;
    new_habit_metric_ = nullptr;
    new_habit_threshold_colours_ = nullptr;
    new_habit_data_ = nullptr;
    current_habit_ = nullptr;

    RefreshHabitUIStates();
}
IHabitApp::IHabitApp::~IHabitApp() {

    delete new_habit_title_;
    delete new_habit_description_;
    delete new_habit_metric_;
    delete new_habit_threshold_colours_;
    delete new_habit_data_;
}

void IHabitApp::IHabitApp::RefreshHabitUIStates() {
    for(const HabitTracker& habit_tracker : HabitTrackerManager::GetHabitTrackers()){
        const std::map<std::string, std::vector<Square>> grids =
            GridMaker::MakeGrids(habit_tracker);
        HabitUIState habit_ui_state{
            grids,
            false,
            0,
            habit_tracker.GetAvailableYears()
        };
        habit_ui_states_.emplace(habit_tracker.GetTitle(), habit_ui_state);
    }
}

void IHabitApp::IHabitApp::ShowSettingsMenu(){
    Rectangle settings_panel = {
        .x = standard_box_inner.x,
        .y = standard_box_inner.y,
        .width = standard_box_inner.width,
        .height = standard_box_inner.height
    };
    Rectangle content = {
        .x = 0,
        .y = 0,
        .width = 672,
        .height = static_cast<float>(40 * HabitTrackerManager::GetHabitTrackers().size())
    };
    Rectangle view;
    Vector2 habit_panel_offset = { .x = 0, .y = 0 };

    window_gui_state_ = (GuiWindowBox(standard_box, "Settings")) ?
    WindowGuiState::None : WindowGuiState::SettingsBox;
}

void IHabitApp::IHabitApp::ShowHabitsMenu(){
    Rectangle habit_view_panel = {
        .x = standard_box_inner.x,
        .y = standard_box_inner.y,
        .width = standard_box_inner.width / 2,
        .height = standard_box_inner.height
    };

    Rectangle habit_information_panel = {
        .x = standard_box_inner.x + standard_box_inner.width / 2,
        .y = standard_box_inner.y,
        .width = standard_box_inner.width / 2,
        .height = standard_box_inner.height
    };

    // TO UNDERSTAND: Have to negate 2 in order for lower scroll bar to not be present.
    Rectangle content = {
        .x = 0,
        .y = 0,
        .width = habit_view_panel.width - 2,
        .height =
            static_cast<float>(40 * HabitTrackerManager::GetHabitTrackers().size())
    };

    Rectangle view;
    Vector2 habit_panel_offset = { .x = 0, .y = 0 };

    window_gui_state_ = (GuiWindowBox(standard_box, "Habits")) ?
    WindowGuiState::None : WindowGuiState::HabitsBox;

    GuiScrollPanel(
        habit_view_panel,
        nullptr,
        content,
        &habit_panel_offset, &view);

    BeginScissorMode(
        static_cast<int>(view.x),
        static_cast<int>(view.y),
        static_cast<int>(view.width),
        static_cast<int>(view.height));

    Vector2 initial_habit_position = {
        .x = habit_view_panel.x,
        .y = habit_view_panel.y + habit_panel_offset.y
    };

    auto begin = HabitTrackerManager::GetHabitTrackers().begin();

    for(auto iterator{begin};
        iterator != HabitTrackerManager::GetHabitTrackers().end();){

        Rectangle habit_panel {
            .x = initial_habit_position.x,
            .y = initial_habit_position.y,
            .width = 624,
            .height = 48
        };

        Rectangle name_panel {
            .x = habit_panel.x + 8,
            .y = habit_panel.y + 8,
            .width = 232,
            .height = 24
        };

        Rectangle view_button_panel {
            .x = habit_panel.x + 560,
            .y = habit_panel.y + 8,
            .width = 24,
            .height = 24
        };
        Rectangle delete_button_panel {
            .x = habit_panel.x + 592,
            .y = habit_panel.y + 8,
            .width = 24,
            .height = 24
        };

        DrawRectangle(
            static_cast<int>(habit_panel.x),
            static_cast<int>(habit_panel.y),
            static_cast<int>(habit_panel.width),
            static_cast<int>(habit_panel.height),
            LIGHTGRAY);

        DrawRectangleLines(
            static_cast<int>(habit_panel.x),
            static_cast<int>(habit_panel.y),
            static_cast<int>(habit_panel.width),
            static_cast<int>(habit_panel.height),
            DARKGRAY);

        GuiLabel(name_panel, iterator->GetTitle().c_str());

        if(GuiButton(view_button_panel, "#42#")){
            current_habit_ = &(*iterator);
        }

        if(GuiButton(delete_button_panel, "#143#")){
            HabitTrackerManager::RemoveHabitTracker(*iterator);
        }
        else{
            ++iterator;
        }
        initial_habit_position.y += 40;
    }
    EndScissorMode();

    if(current_habit_ !=nullptr){
        std::string title = "Title: " + current_habit_->GetTitle();
        GuiLabel({
            .x = habit_information_panel.x + 8,
            .y = habit_information_panel.y + 8,
            .width = habit_information_panel.width,
            .height = 24},
            title.c_str());

        std::string metric = "Metric: " + current_habit_->GetMetric();
        GuiLabel({
            .x = habit_information_panel.x + 8,
            .y = habit_information_panel.y + 40,
            .width = habit_information_panel.width,
            .height = 24},
            metric.c_str());

        std::string description = "Description: " + current_habit_->GetDescription();
        GuiLabel({
            .x = habit_information_panel.x + 8,
            .y = habit_information_panel.y + 72,
            .width = habit_information_panel.width,
            .height = 24},
            description.c_str());

        std::map<double, std::string> colour_thresholds =
            current_habit_->GetThresholdColours();
        GuiLabel({
            .x = habit_information_panel.x + 8,
            .y = habit_information_panel.y + 104,
            .width = habit_information_panel.width,
            .height = 24},
            "Colour Thresholds");

        int new_position { static_cast<int>(habit_information_panel.y) + 136 };

        for(std::pair<double, std::string> threshold : colour_thresholds){
            GuiLabel({
                .x = habit_information_panel.x + 40,
                .y = static_cast<float>(new_position),
                .width = habit_information_panel.width,
                .height = 24},
                std::to_string(threshold.first).c_str());

            Color color = GridMaker::StringToColor(threshold.second);

            DrawRectangle(static_cast<int>(habit_information_panel.x) + 16,
                new_position + 8,
                8,
                8,
        {.r = color.r,
                .g = color.g,
                .b = color.b,
                .a = 255} );
            new_position += 32;
        }

    }
}

void IHabitApp::IHabitApp::ShowHelpMenu(){
    Rectangle content = {
        .x = 0,
        .y = 0,
        .width = 672,
        .height = static_cast<float>(40 * HabitTrackerManager::GetHabitTrackers().size())
    };
    Rectangle view;
    Vector2 habit_panel_offset = { .x = 0, .y = 0 };

    window_gui_state_ = (GuiWindowBox(standard_box, "Help")) ?
    WindowGuiState::None : WindowGuiState::HelpBox;

}