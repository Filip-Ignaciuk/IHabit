#ifndef IHABIT_HABIT_TRACKER_SERIALIZER_FACTORY_HPP
#define IHABIT_HABIT_TRACKER_SERIALIZER_FACTORY_HPP

#include "HabitTrackerSerializer.hpp"

#ifdef __EMSCRIPTEN__
#include <functional>
#endif

class HabitTrackerSerializerFactory
{
public:
#ifdef __EMSCRIPTEN__
    static void InitializeEmscripten(std::function<void()> on_ready);
#endif

    static std::unique_ptr<HabitTrackerSerializer> CreateSerializer();
};

#endif
