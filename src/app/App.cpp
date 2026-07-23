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

        // Add Habit button
        if(GuiButton({160, 16, 32, 32}, "#8#")){
            window_gui_state_ = (window_gui_state_ == WindowGuiState::AddHabitBox)
            ? WindowGuiState::None : WindowGuiState::AddHabitBox;
        }

        // Disable scroll as it disrupts window scrollbars.
        if (window_gui_state_ == WindowGuiState::None) {
            GuiScrollPanel((Rectangle){
            0,
            64,
            1280 - scroll_panel_bounds.x,
            656 - scroll_panel_bounds.y},
            nullptr,
            (Rectangle){
                .x = 0,
                .y = 0,
                .width = 1264,
                .height = static_cast<float>(
                    192 * HabitTrackerManager::GetHabitTrackers().size())},
            &scroll_panel_offset, &scroll_panel_view);
        }


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
            const std::string& title = iterator->first;
            const HabitTracker& habit_tracker = iterator->second;
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

            std::string drop_down_box_title = GetDropDownBoxTitle(iterator->second.GetAvailableYears());

            if (GuiButton({initial_habit_position.x + 1104,
                initial_habit_position.y + 8 + ((192.0f) * count),
            72,
            24},
                "Record day")) {
                window_gui_state_ = (window_gui_state_ == WindowGuiState::RecordDayBox)
                ? WindowGuiState::None : WindowGuiState::RecordDayBox;
                selected_habit_title_ = habit_tracker.GetTitle();
            }

            if (GuiDropdownBox({
                .x = initial_habit_position.x + 1184,
                .y = initial_habit_position.y + 8 + ((192.0f) * count),
                .width = 72,
                .height = 24},
                drop_down_box_title.c_str(),
                &habit_ui_state.drop_down_box_active,
                habit_ui_state.drop_down_box_edit_mode)) {
                habit_ui_state.drop_down_box_edit_mode = !habit_ui_state.drop_down_box_edit_mode;
            }

            GuiLabel({
                .x = initial_habit_position.x + 8,
                .y = initial_habit_position.y + 40 + ((192.0f) * count),
                .width = 1264,
                .height = 24},
                habit_tracker.GetMetric().c_str());

            const std::string& selectedYear =
                habit_ui_state.drop_down_box_years[habit_ui_state.drop_down_box_active];

            const Grid& grid = habit_ui_state.grids.at(selectedYear);
            int xDisplacement = 8;
            if (wants_weekday_displayed_) {
                xDisplacement = 40;
            }

            for(Square square : grid.squares) {
                Color color = square.color;
                DrawRectangle(
                    static_cast<int>(initial_habit_position.x + square.rectangle.x) + xDisplacement,
                    static_cast<int>(initial_habit_position.y + square.rectangle.y +
                        72 + ((192) * count)),
                    static_cast<int>(square.rectangle.width),
                    static_cast<int>(square.rectangle.height),
                    color);
            }

            if (wants_weekday_displayed_) {
                int weekday_index = 0;
                for (const std::string_view& weekday : weekdays) {
                    GuiSetStyle(LABEL, TEXT_SIZE, 8);
                    GuiLabel({ initial_habit_position.x + 8,
                        initial_habit_position.y + (16 * weekday_index) + 64 +((192) * count),
                        24,
                        24
                    },
                    std::string(weekday).c_str());
                    GuiSetStyle(LABEL, TEXT_SIZE, 16);
                    ++weekday_index;
                }
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
        if(window_gui_state_ == WindowGuiState::RecordDayBox){
            ShowRecordDayMenu();
        }

        if(window_gui_state_ == WindowGuiState::AddHabitBox){
            ShowAddHabitMenu();
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

    new_habit_threshold_colours_ = nullptr;
    current_habit_ = nullptr;
    new_habit_threshold_colours_ = nullptr;

    RefreshHabitUIStates();
}
IHabitApp::IHabitApp::~IHabitApp() {
    delete new_habit_threshold_colours_;
}

void IHabitApp::IHabitApp::RefreshHabitUIStates() {
    for(std::pair<std::string, HabitTracker> data_pair : HabitTrackerManager::GetHabitTrackers()){
        const std::map<std::string, Grid> grids =
            GridMaker::MakeGrids(data_pair.second);
        HabitUIState habit_ui_state{
            grids,
            false,
            0,
            data_pair.second.GetAvailableYears()
        };
        habit_ui_states_[data_pair.first] = habit_ui_state;
    }
}

std::string IHabitApp::IHabitApp::GetDropDownBoxTitle(std::vector<std::string> years) {
    const size_t size = years.size();
    std::string title;
    for (int i = 0; i < size; ++i) {
        title += years[i];
        if (i < years.size() - 1) {
            title += ';';
        }
    }
    return title;
}

void IHabitApp::IHabitApp::ShowSettingsMenu(){
    window_gui_state_ = (GuiWindowBox(standard_box, "Settings")) ?
    WindowGuiState::None : WindowGuiState::SettingsBox;

    GuiLabel({standard_box_inner.x + 8,
        standard_box_inner.y + 8,
        standard_box_inner.width / 2,
        24
    },
    "Display days of the week: ");

    GuiCheckBox({standard_box_inner.x + standard_box_inner.width / 2 + 8,
    standard_box_inner.y + 8,
        24,
        24
    },
    "",
    &wants_weekday_displayed_);

    /*
    GuiLabel({standard_box_inner.x + 8,
        standard_box_inner.y + 40,
        standard_box_inner.width / 2,
        24
    },
    "European weekday format: ");

    GuiCheckBox({standard_box_inner.x + standard_box_inner.width / 2 + 8,
    standard_box_inner.y + 40,
        24,
        24
    },
    "",
    &wants_european_weekday_);
    */
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

    Rectangle content = {
        .x = 0,
        .y = 0,
        .width = habit_view_panel.width,
        .height =
            static_cast<float>(40 * (HabitTrackerManager::GetHabitTrackers().size()))
    };

    Rectangle view;

    window_gui_state_ = (GuiWindowBox(standard_box, "Habits")) ?
    WindowGuiState::None : WindowGuiState::HabitsBox;

    GuiScrollPanel(
        habit_view_panel,
        nullptr,
        content,
        &habit_panel_offset_, &view);

    BeginScissorMode(
        static_cast<int>(view.x),
        static_cast<int>(view.y),
        static_cast<int>(view.width),
        static_cast<int>(view.height));

    Vector2 initial_habit_position = {
        .x = habit_view_panel.x,
        .y = habit_view_panel.y + habit_panel_offset_.y
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
            .x = habit_panel.x + 528,
            .y = habit_panel.y + 8,
            .width = 24,
            .height = 24
        };
        Rectangle delete_button_panel {
            .x = habit_panel.x + 560,
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

        GuiLabel(name_panel, iterator->first.c_str());

        if(GuiButton(view_button_panel, "#42#")){
            current_habit_ = &iterator->second;
        }

        if(GuiButton(delete_button_panel, "#143#")){
            if (current_habit_ != nullptr &&
                current_habit_->GetTitle() == iterator->first) {
                current_habit_ = nullptr;
            }
            HabitTrackerManager::RemoveHabitTracker(iterator->second);
            RefreshHabitUIStates();
            break;
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
    window_gui_state_ = (GuiWindowBox(standard_box, "Help")) ?
    WindowGuiState::None : WindowGuiState::HelpBox;

    GuiLabel({
        standard_box_inner.x + 8,
        standard_box_inner.y + 8,
        standard_box_inner.width,
        24,
    },
        "Welcome to an example of the habit tracker library!");
    GuiLabel({
        standard_box_inner.x + 8,
        standard_box_inner.y + 40,
        standard_box_inner.width,
        24,
    },
    "Here you can track your habits, create and delete them.");
    GuiLabel({
        standard_box_inner.x + 8,
        standard_box_inner.y + 72,
        standard_box_inner.width,
        24,
    },
    "Click on the Habit menu, the middle button on the top left of the screen, to add, remove, and see information about your habits. Enjoy :)");

    GuiLabel({
        standard_box_inner.x + 8,
        standard_box_inner.y + 104,
        standard_box_inner.width,
        24,
    },
    "Credits: Filip Ignaciuk");

}

void IHabitApp::IHabitApp::ShowRecordDayMenu() {
    std::string window_box_title = "Record Day for " + selected_habit_title_;
    if (GuiWindowBox(small_box_inner, window_box_title.c_str())) {
        window_gui_state_ = WindowGuiState::None;
        previous_selected_habit_title_ = selected_habit_title_;
        selected_habit_title_.clear();
    }
    else {
        window_gui_state_ = WindowGuiState::RecordDayBox;
    }

    if (previous_selected_habit_title_ != selected_habit_title_) {
        memset(new_record_day_, 0, sizeof(new_record_day_));
        memset(new_record_month_, 0, sizeof(new_record_month_));
        memset(new_record_year_, 0, sizeof(new_record_year_));
        memset(new_record_value_, 0, sizeof(new_record_value_));
        previous_selected_habit_title_ = selected_habit_title_;
    }

    // Date
    // Day
    GuiLabel({small_box_inner.x + 32,
        small_box_inner.y + 32,
        72,
        24
    },
    "Day");

    if (GuiTextBox({small_box_inner.x + 32,
    small_box_inner.y + 64,
        72,
        24},
        new_record_day_,
            3,
            day_edit_mode_
    )) {
        day_edit_mode_ = !day_edit_mode_;
    }

    // Month
    GuiLabel({small_box_inner.x + 120,
        small_box_inner.y + 32,
        72,
        24
    },
    "Month");

    if (GuiTextBox({small_box_inner.x + 120,
    small_box_inner.y + 64,
        72,
        24},
        new_record_month_,
            3,
            month_edit_mode_
    )) {
        month_edit_mode_ = !month_edit_mode_;
    }

    // Year
    GuiLabel({small_box_inner.x + 208,
        small_box_inner.y + 32,
        72,
        24
    },
    "Year");

    if (GuiTextBox({small_box_inner.x + 208,
    small_box_inner.y + 64,
        72,
        24},
        new_record_year_,
            5,
            year_edit_mode_
    )) {
        year_edit_mode_ = !year_edit_mode_;
    }

    // Value
    GuiLabel({small_box_inner.x + 32,
        small_box_inner.y + 96,
        256,
        24
    },
    "Value");

    if (GuiTextBox({small_box_inner.x + 32,
    small_box_inner.y + 128,
        160,
        24},
        new_record_value_,
            6,
            value_edit_mode_
    )) {
        value_edit_mode_ = !value_edit_mode_;
    }

    if (GuiButton({small_box_inner.x + 208,
        small_box_inner.y + 128,
        72,
        24},
        "Add")) {
        std::chrono::year_month_day* result = IsRecordInputValid();
        if (result) {
            RecordDay(result);
        }
    }

}

void IHabitApp::IHabitApp::ShowAddHabitMenu() {
    window_gui_state_ = (GuiWindowBox(standard_box, "Add habit")) ?
    WindowGuiState::None : WindowGuiState::AddHabitBox;

    GuiLabel({ standard_box_inner.x + 8,
        standard_box_inner.y + 8,
        608,
        24
    },
    "Name");

    if (GuiTextBox({ standard_box_inner.x + 8,
        standard_box_inner.y + 48,
        608,
        24
    },
    new_habit_title_,
    20,
    new_habit_title_edit_mode_)) {
        new_habit_title_edit_mode_ = !new_habit_title_edit_mode_;
    }

    GuiLabel({ standard_box_inner.x + 8,
        standard_box_inner.y + 88,
        608,
        24
    },
    "Description");

    if (GuiTextBox({standard_box_inner.x + 8,
        standard_box_inner.y + 128,
        608,
        24
    },
    new_habit_description_,
    200,
    new_habit_description_edit_mode_)) {
        new_habit_description_edit_mode_ = !new_habit_description_edit_mode_;
    }

    GuiLabel({ standard_box_inner.x + 8,
        standard_box_inner.y + 168,
        608,
        24
    },
    "Metric");

    if (GuiTextBox({standard_box_inner.x + 8,
        standard_box_inner.y + 208,
        608,
        24
    },
    new_habit_metric_,
    20,
    new_habit_metric_edit_mode_)) {
        new_habit_metric_edit_mode_ = !new_habit_metric_edit_mode_;
    }

    GuiLabel({ standard_box_inner.x + 632,
        standard_box_inner.y + 8,
        608,
        24
    },
    "Threshold Colours");

    GuiColorPicker({standard_box_inner.x + 1050,
        standard_box_inner.y + 16,
        152,
        152
    },"",
    &new_threshold_colour_
    );

    Rectangle threshold_colour_view;

    int scroll_size = 0;
    if (new_habit_threshold_colours_) {
        scroll_size = 192 * new_habit_threshold_colours_->size();
    }

    GuiScrollPanel((Rectangle){
            standard_box_inner.x + 632,
            standard_box_inner.y + 48,
            400,
        552
            },
            nullptr,
            (Rectangle){
                .x = 0,
                .y = 0,
                .width = 1264,
                .height = static_cast<float>(scroll_size)},
            &new_habit_panel_offset_, &threshold_colour_view);

    /*
    BeginScissorMode();

    EndScissorMode();
    */
    if (GuiButton({standard_box_inner.x + 1152,
        standard_box_inner.y + 552,
        72,
        24
    },
    "Add")) {
        HabitTracker* habit_tracker = IsAddHabitInputValid();
        if (habit_tracker) {
            AddHabit(habit_tracker);
        }
    }
}

std::chrono::year_month_day* IHabitApp::IHabitApp::IsRecordInputValid() {
    // Check Day
    if (!isdigit(new_record_day_[0]) || !isdigit(new_record_day_[1])) {
        return nullptr;
    }
    std::string day_string;
    day_string.push_back(new_record_day_[0]);
    day_string.push_back(new_record_day_[1]);
    int day_int = std::stoi(day_string);

    // Check Month
    if (!isdigit(new_record_month_[0]) || !isdigit(new_record_month_[1])) {
        return nullptr;
    }
    std::string month_string;
    month_string.push_back(new_record_month_[0]);
    month_string.push_back(new_record_month_[1]);
    int month_int = std::stoi(month_string);

    // Check Year
    if (!isdigit(new_record_year_[0]) ||
        !isdigit(new_record_year_[1]) ||
        !isdigit(new_record_year_[2]) ||
        !isdigit(new_record_year_[3])) {
        return nullptr;
    }
    std::string year_string;
    year_string.push_back(new_record_year_[0]);
    year_string.push_back(new_record_year_[1]);
    year_string.push_back(new_record_year_[2]);
    year_string.push_back(new_record_year_[3]);
    int year_int = std::stoi(year_string);

    // Check Value
    std::string value_string;
    value_string.push_back(new_record_value_[0]);
    value_string.push_back(new_record_value_[1]);
    value_string.push_back(new_record_value_[2]);
    value_string.push_back(new_record_value_[3]);
    value_string.push_back(new_record_value_[4]);
    try {
        new_record_value_double_ = std::stod(value_string);
    }
    catch (std::invalid_argument& e) {
        return nullptr;
    }


    // Check if date is valid
    std::chrono::year_month_day* ymd = new std::chrono::year_month_day(std::chrono::year(year_int) /
        std::chrono::month(month_int) /
            std::chrono::day(day_int));
    if (ymd->ok()) {
        return ymd;
    }
    else {
        return nullptr;
    }
}

void IHabitApp::IHabitApp::RecordDay(std::chrono::year_month_day* ymd) {
    HabitTrackerManager::RecordDay(selected_habit_title_, ymd, new_record_value_double_);
    RefreshHabitUIStates();
}

HabitTracker* IHabitApp::IHabitApp::IsAddHabitInputValid() {
    std::string title;
    std::string description;
    std::string metric;

    // Check if title is not empty
    if (new_habit_title_[0] == '\0') {
        return nullptr;
    }

    for (int i {0}; i < 20; ++i) {
        title.push_back(new_habit_title_[i]);
    }

    // Description can be empty
    for (int i {0}; i < 200; ++i) {
        description.push_back(new_habit_description_[i]);
    }

    //Check if metric is not empty
    if (new_habit_metric_[0] == '\0') {
        return nullptr;
    }

    for (int i {0}; i < 20; ++i) {
        metric.push_back(new_habit_metric_[i]);
    }
    return nullptr;
}
void IHabitApp::IHabitApp::AddHabit(HabitTracker* habit_tracker) {

}