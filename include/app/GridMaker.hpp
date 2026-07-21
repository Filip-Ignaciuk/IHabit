#ifndef GRID_MAKER_HPP
#define GRID_MAKER_HPP

#include "raylib.h"
#include "HabitTracker.hpp"
#include <map>

struct Grid;

class GridMaker{
public:
    static std::map<std::string, Grid> MakeGrids(const HabitTracker& habit_tracker);
    static Color StringToColor(std::string string_color);

    static constexpr Rectangle standard_rectangle{0,0,8,8};
    static constexpr Color grey{128, 128, 128, 255};

    GridMaker() = delete;
    ~GridMaker() = delete;
};

struct Square{
    Rectangle rectangle;
    // We assume that the square is initially not populated.
    Color color = GridMaker::grey;
};

struct Grid {
    std::array<Square, 366> squares;
    bool is_leap;
    // Indicates which day in the week it starts, sun - 0, mon - 1 etc
    int starting_day;
};





#endif