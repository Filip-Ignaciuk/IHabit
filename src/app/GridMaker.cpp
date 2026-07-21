#include "GridMaker.hpp"
#include "HabitTracker.hpp"
#include "raylib.h"
#include <string>
#include <utility>
#include <vector>
#include <chrono>

std::map<std::string, std::vector<Square>> GridMaker::MakeGrids(const HabitTracker& habit_tracker){
    std::map<std::string, std::vector<Square>> grids;
    std::chrono::year current_year_chrono;
    int current_year = 0;
    std::vector<Square> squares;
    int column = 0;
    for(std::pair<std::chrono::year_month_day, double> data_pair : habit_tracker.GetData()){
        int current_data_pair_year = static_cast<int>(data_pair.first.year());
        if (current_year == 0) {
            current_year = current_data_pair_year;
            current_year_chrono = data_pair.first.year();
        }
        if(current_data_pair_year != current_year){
            // We finished the previous year off.
            int leftover = (data_pair.first.year().is_leap()) 
            ? 366 : 365;
            leftover -= squares.size();

            Square last_square = squares.back();
            // Position of last square
            int column = (last_square.rectangle.x - 8) / 16;
            int row = (last_square.rectangle.y - 8) / 16;

            // Adding remaining leftover squares
            for(; leftover != 0; leftover--){
                Square square;
                square.color = grey;
                square.rectangle = {(float)(8 + (column * 16)),
                    (float)(8 + (row * 16)),
                    8,
                    8
                };
                row++;
                if(row == 7){
                    row = row % 7;
                    column++;
                }
                squares.emplace_back(square);
            }
            
            // Clearing and moving to next one.
            grids.emplace(std::to_string(current_year), squares);
            current_year = current_data_pair_year;
            current_year_chrono = data_pair.first.year();
            squares.clear();
            column = 0;

        }

        Square square;
        square.color = StringToColor(
            habit_tracker.GetColour(data_pair.second));
        
        std::chrono::weekday weekday{std::chrono::sys_days{data_pair.first}};
        int starting_day_number = static_cast<int>(weekday.iso_encoding());
        
        Rectangle square_rectangle {(float)8 + (column * 16), 
            (float)(8 + ((starting_day_number - 1) * 16)), 
            8, 
            8};

        square.rectangle = square_rectangle;
        squares.emplace_back(square);
        if(starting_day_number == 7) {
            column++;
        }
    }

    // We finished the previous year off.
    int leftover = (current_year_chrono.is_leap())
    ? 366 : 365;
    leftover -= squares.size();
    Square last_square = squares.back();
    // Position of last square
    column = (last_square.rectangle.x - 8) / 16;
    int row = (last_square.rectangle.y - 8) / 16;

    // Adding remaining leftover squares
    for(; leftover != 0; leftover--){
        Square square;
        square.color = grey;
        square.rectangle = {(float)(8 + (column * 16)),
            (float)(8 + (row * 16)),
            8,
            8
        };
        row++;
        if(row == 7){
            row = row % 7;
            column++;
        }
        squares.emplace_back(square);
    }

    // Clearing and moving to next one.
    grids.emplace(std::to_string(current_year), squares);

    return grids;
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