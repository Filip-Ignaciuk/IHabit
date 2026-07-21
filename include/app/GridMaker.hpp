#ifndef GRID_MAKER_HPP
#define GRID_MAKER_HPP

#include "raylib.h"
#include "HabitTracker.hpp"
#include <map>
#include <vector>

struct Square{
    Rectangle rectangle;
    Color color;
};

class GridMaker{
    public:
    static std::map<std::string, std::vector<Square>> MakeGrids(const HabitTracker& habit_tracker);
    static Color StringToColor(std::string string_color);

    GridMaker() = delete;
    ~GridMaker() = delete;

    private:
    static constexpr Rectangle standard_rectangle{0,0,8,8};
    static constexpr Color grey{128, 128, 128, 255};
};

#endif