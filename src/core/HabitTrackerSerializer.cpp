#include "HabitTrackerSerializer.hpp"
#include "json.hpp"
#include <iostream>

HabitTracker* HabitTrackerSerializer::ParseHabitTracker(const std::string &json) {
    HabitTracker* habit_tracker = nullptr;
    nlohmann::json json_file;

    try {
        json_file = nlohmann::json::parse(json);
    } catch (const nlohmann::json::parse_error& e) {
        std::cout << "Failed to parse JSON: " << e.what() << '\n';
        return nullptr;
    }

    if(json_file.empty()){
        std::cout << "File is empty!\n";
        return habit_tracker;
    }

    if(!json_file.contains("information")){
        std::cout << "Json file doesn't contain a information section, aborting.\n";
        return habit_tracker;
    }

    if(!json_file.contains("data")){
        std::cout << "Json file doesn't contain a data section, aborting.\n";
        return habit_tracker;
    }

    if(!json_file["information"].contains("title")){
        std::cout << "Json file doesn't contain title in information section, aborting.\n";
        return habit_tracker;
    }
    std::string title = json_file["information"]["title"];

    if(!json_file["information"].contains("description")){
        std::cout << "Json file doesn't contain description in information section, aborting.\n";
        return habit_tracker;
    }
    std::string description = json_file["information"]["description"];

    if(!json_file["information"].contains("metric")){
        std::cout << "Json file doesn't contain metric in information section, aborting.\n";
        return habit_tracker;
    }
    std::string metric = json_file["information"]["metric"];

    std::map<std::string, std::string> threshold_colours_temporary;
    try {
        threshold_colours_temporary =
        json_file["information"]["thresholds"].get<std::map<std::string, std::string>>();
    } catch (const nlohmann::json::type_error& exception) {
        std::cout << "Incorrect type expected in thresholds.\n";
        std::cout << exception.what();
        return habit_tracker;
    }

    // Converting thresholds from string to double.
    std::map<double, std::string> threshold_colours;
    for(std::pair<std::string, std::string> threshold : threshold_colours_temporary){
        // Stripping # in colour string.
        std::string clean = threshold.second;
        if (clean[0] == '#'){
            clean = clean.substr(1);
        }

        threshold_colours.emplace(std::stod(threshold.first), clean);
    }

    std::map<std::string, double> data_temporary;
    try{
        data_temporary = json_file["data"].get<std::map<std::string, double>>();
    }
    catch (const nlohmann::json::type_error& exception){
        std::cout << "Incorrect type expected in thresholds.\n";
        std::cout << exception.what();
        return habit_tracker;
    }

    // Converting date from string to chrono format.
    std::map<std::chrono::year_month_day, double> data;
    for(std::pair<std::string, double> data_pair : data_temporary){
        std::string date = data_pair.first;
        size_t first_slash = date.find('/');
        size_t second_slash = date.find('/', first_slash + 1);

        std::string day_str = date.substr(0, first_slash);
        std::string month_str = date.substr(first_slash + 1, second_slash - first_slash - 1);
        std::string year_str = date.substr(second_slash + 1);

        auto unsigned_day = static_cast<unsigned int>(std::stoi(day_str));
        auto unsigned_month = static_cast<unsigned int>(std::stoi(month_str));
        std::chrono::year_month_day first_day{
        std::chrono::year{std::stoi(year_str)},
        std::chrono::month{unsigned_month},
        std::chrono::day{unsigned_day}};
        data.emplace(first_day, data_pair.second);
    }

    habit_tracker = new HabitTracker(
        title,
        description,
        metric,
        threshold_colours,
        data
        );

    return habit_tracker;
}

std::string HabitTrackerSerializer::ConvertToJsonString(
    const HabitTracker &habit_tracker) {
    nlohmann::json json_file;

    json_file["information"]["title"] = habit_tracker.GetTitle();
    json_file["information"]["description"] = habit_tracker.GetDescription();
    json_file["information"]["metric"] = habit_tracker.GetMetric();

    std::map<double, std::string> threshold_colours = habit_tracker.GetThresholdColours();
    for(std::pair<double, std::string> threshold_colour : threshold_colours){
        std::string key = std::to_string(threshold_colour.first);
        json_file["information"]["thresholds"][key] = threshold_colour.second;
    }

    for(std::map<std::chrono::year_month_day, double> entry_data =
        habit_tracker.GetData();
        std::pair<std::chrono::year_month_day, double> entry : entry_data){
        std::chrono::year_month_day chrono_date = entry.first;
        std::string date = std::to_string(static_cast<unsigned int>(chrono_date.day())) +
            "/" + std::to_string(static_cast<unsigned int>(chrono_date.month())) +
                "/" + std::to_string(static_cast<int>(chrono_date.year()));
        json_file["data"][date] = entry.second;
    }

    return json_file.dump(4);
}
