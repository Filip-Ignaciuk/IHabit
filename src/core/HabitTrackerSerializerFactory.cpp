#include "HabitTrackerSerializerFactory.hpp"
#include "HabitTrackerSerializerFileSystem.hpp"
#include "HabitTrackerSerializerIndexedDB.hpp"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

std::unique_ptr<HabitTrackerSerializer> HabitTrackerSerializerFactory::CreateSerializer() {
#ifdef __EMSCRIPTEN__
    return std::make_unique<HabitTrackerSerializerIndexedDB>();
#else
    return std::make_unique<HabitTrackerSerializerFileSystem>();
#endif
}