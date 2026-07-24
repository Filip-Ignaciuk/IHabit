#include "GridMaker.hpp"
#include "HabitTracker.hpp"
#include "raylib.h"
#include <string>
#include <utility>
#include <chrono>

std::map<std::string, Grid> GridMaker::MakeGrids(const HabitTracker& habit_tracker){
    std::map<std::string, Grid> result;


    for(std::pair<std::chrono::year_month_day, double> data_pair :
        habit_tracker.GetData()){

        const std::chrono::year& current_year = data_pair.first.year();
        std::string current_year_string = std::to_string(static_cast<int>(current_year));
        bool is_leap = current_year.is_leap();
        std::chrono::year_month_day jan_1_ymd = std::chrono::year(
            current_year) /
                std::chrono::January / 1;
        auto jan_1 = std::chrono::sys_days(jan_1_ymd);
        auto current_day = std::chrono::sys_days(data_pair.first);
        auto day_index = (current_day - jan_1).count();
        int starting_day = static_cast<int>(std::chrono::weekday(jan_1).c_encoding());
        int column = (day_index + starting_day) / 7;
        int row = static_cast<int>(std::chrono::weekday(current_day).c_encoding());
        //int row = (((day_index % 7) + starting_day) % 7) - 1;
        // Create Square



        const Color& color = StringToColor(habit_tracker.GetColour(data_pair.second));
        const Square square {
            .rectangle = {
                .x = static_cast<float>(column * 16),
                .y = static_cast<float>(row * 16),
                .width = 8,
                .height = 8
            },
            .color = color
        };

        // Add to correct position

        if (!result.contains(current_year_string)) {
            // Year doesnt exist yet.
            Grid grid{
                .squares = {},
                .is_leap = current_year.is_leap(),
                .starting_day = starting_day
            };
            result.emplace(current_year_string, grid);
        }
        result[current_year_string].squares[day_index] = square;
    }

    if (result.empty()) {
        // Habit has no data. Create empty grid for this year.
        auto today =
            std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
        auto year_month_day = std::chrono::year_month_day(today);
        Grid grid{{},
            year_month_day.year().is_leap(),
            static_cast<int>(std::chrono::weekday(year_month_day).c_encoding())};
        result.emplace(std::to_string(static_cast<int>(year_month_day.year())),
            grid);
    }

    // Correct positions for missing days
    for (std::pair<const std::string, Grid>& data_pair : result) {
        for (int i {0}; i < 367; ++i) {
            Square square = data_pair.second.squares[i];
            if (square.color.r == grey.r &&
                square.color.g == grey.g &&
                square.color.b == grey.b) {
                int column = (i + data_pair.second.starting_day) / 7;
                auto days = std::chrono::sys_days(
                    std::chrono::year(
                        std::stoi(
                            data_pair.first)) /
                            std::chrono::January / 1) +
                            std::chrono::days(i);

                int row = static_cast<int>(
                    std::chrono::weekday(
                        days).c_encoding());
                square.rectangle = {
                    static_cast<float>((column) * 16),
                    static_cast<float>((row) * 16),
                    8,
                    8
                };
            }
            data_pair.second.squares[i] = square;
        }

        std::chrono::year year { std::stoi(data_pair.first)};
        if (!year.is_leap()) {
            data_pair.second.squares[365].color.a = 0;
        }
    }

    return result;
}

Color GridMaker::StringToColor(std::string string_color){
    if (string_color[0] == '#'){
        string_color = string_color.substr(1);
    }
    const unsigned int color = std::stoul(string_color, nullptr, 16);
    return Color{
        .r = static_cast<unsigned char>((color >> 16) & 0xFF),
        .g = static_cast<unsigned char>((color >> 8) & 0xFF),
        .b = static_cast<unsigned char>(color & 0xFF),
        .a = 255
    };
}