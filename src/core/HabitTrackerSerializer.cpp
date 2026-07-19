#include "HabitTrackerSerializer.hpp"
#include "HabitTracker.hpp"
#include "json.hpp"
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>

HabitTracker* HabitTrackerSerializer::LoadHabitTracker(std::filesystem::path path){
    std::ifstream input(path);
    if (!input.is_open()) {
        std::cout << "Can't open file.\n";
        return nullptr;
    }

    nlohmann::json json_file;
    input >> json_file;

    if(input.bad()){
        std::cout << "I/O error while reading\n";
        return nullptr;
    }
    if(input.fail()){
        std::cout << "A failure has occurred when reading the file at: " << path << '\n';
        return nullptr;
    }

    if(json_file.empty()){
        std::cout << "File is empty!\n";
        return nullptr;
    }

    if(!json_file.contains("information")){
        std::cout << "Json file doesn't contain a information section, aborting.\n";
        input.close();
        return nullptr;
    }

    if(!json_file.contains("data")){
        std::cout << "Json file doesn't contain a data section, aborting.\n";
        input.close();
        return nullptr;
    }

    if(!json_file["information"].contains("title")){
        std::cout << "Json file doesn't contain title in information section, aborting.\n";
        input.close();
        return nullptr;
    }
    std::string title = json_file["information"]["title"];

    if(!json_file["information"].contains("description")){
        std::cout << "Json file doesn't contain description in information section, aborting.\n";
        input.close();
        return nullptr;
    }
    std::string description = json_file["information"]["description"];

    if(!json_file["information"].contains("metric")){
        std::cout << "Json file doesn't contain metric in information section, aborting.\n";
        input.close();
        return nullptr;
    }
    std::string metric = json_file["information"]["metric"];

    std::map<std::string, double> threshold_colours_temporary;
    try {
        threshold_colours_temporary = 
        json_file["information"]["thresholds"].get<std::map<std::string, double>>();
    } catch (nlohmann::json::type_error exception) {
        std::cout << "Incorrect type expected in thresholds.\n";
        std::cout << exception.what();
        return nullptr;
    }
    
    // Convert keys from string to double.
    std::map<double, int> threshold_colours{};
    for(std::pair<std::string, double> pair : threshold_colours_temporary){
        threshold_colours.emplace(std::stoi(pair.first), pair.second);
    }

    std::map<std::string, double> data;
    try{
        data = json_file["data"].get<std::map<std::string, double>>();
    }
    catch (nlohmann::json::type_error exception){
        std::cout << "Incorrect type expected in thresholds.\n";
        std::cout << exception.what();
        return nullptr;
    }
    
    // DEBUG
    std::cout << "Loaded habit successfully!\n";
    std::cout << "Title: " << title << "\n";
    std::cout << "Description: " << description << "\n";
    std::cout << "Metric: " << metric << "\n";
    for(std::pair<std::string, double> thresholdPair : threshold_colours_temporary){
        std::cout << thresholdPair.first << ": " << thresholdPair.second << "\n";
    }

    for(std::pair<std::string, double> dataPair : data){
        std::cout << dataPair.first << ": " << dataPair.second << "\n";
    }

    input.close();

    return new HabitTracker(title, description, metric, threshold_colours, data);
}

void HabitTrackerSerializer::SaveHabitTracker(std::filesystem::path path, 
    const HabitTracker& habit_tracker
){
    // Every save completely overwrites the previous file if it existed,
    // or just creates a new file.
    std::ofstream output(path);
    nlohmann::json json_file;
    json_file["information"]["title"] = habit_tracker.GetTitle();
    json_file["information"]["description"] = habit_tracker.GetDescription();
    json_file["information"]["metric"] = habit_tracker.GetMetric();

    std::map<double, int> threshold_colours = habit_tracker.GetThresholdColours();
    for(std::pair<double, int> threshold_colour : threshold_colours){
        std::string key = std::to_string(threshold_colour.first);
        json_file["information"]["thresholds"][key] = threshold_colour.second;
    }

    std::map<std::string, double> entry_data = habit_tracker.GetData();
    for(std::pair<std::string, double> entry : entry_data){
        json_file["data"][entry.first] = entry.second;
    }
    output << json_file;
    if(output.fail()){
        std::cout << "Failed to save habit tracker to: " << path << '\n';
    }
    output.close();
}

/*
void HabitTrackerManager::SaveTitle(std::string file, std::string title){
    std::ifstream input(file);
    nlohmann::json json_file;
    input >> json_file;
    input.close();
    std::ofstream output(file);
    json_file["information"]["title"] = title;
    output << json_file;
    output.close();
    std::filesystem::rename(file, title + ".json");
}

void HabitTrackerManager::SaveDescription(std::string file, std::string description){
    std::ifstream input(file);
    nlohmann::json json_file;
    input >> json_file;
    input.close();
    std::ofstream output(file);
    json_file["information"]["description"] = description;
    output << json_file;
    output.close();
}
*/