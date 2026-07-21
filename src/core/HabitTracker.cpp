#include "HabitTracker.hpp"
#include <chrono>
#include <iterator>
#include <string>
#include <utility>

HabitTracker::HabitTracker(std::string title, 
        std::string description, 
        std::string metric, 
        std::map<double, std::string> threshold_colours) :
        title_(title),
        description_(description),
        metric_(metric),
        threshold_colours_(threshold_colours){
        UpdateAvailableYears();
}

HabitTracker::HabitTracker(std::string title, 
        std::string description, 
        std::string metric, 
        std::map<double, std::string> threshold_colours,
        std::map<std::chrono::year_month_day, double> data) :
        title_(title),
        description_(description),
        metric_(metric),
        threshold_colours_(threshold_colours),
        data_(data){
        UpdateAvailableYears();
}

const std::string& HabitTracker::GetTitle() const{
    return title_;
}

const std::string& HabitTracker::GetDescription() const{
    return description_;
}

const std::string& HabitTracker::GetMetric() const{
    return metric_;
}

const std::map<double, std::string>& HabitTracker::GetThresholdColours() const{
    return threshold_colours_;
}

const std::map<std::chrono::year_month_day, double>& HabitTracker::GetData() const{
    return data_;
}

const std::string& HabitTracker::GetColour(double value) const{
    std::pair<double, std::string> previous = *threshold_colours_.begin();
    for(std::pair<double, std::string> pair : threshold_colours_){
        if(value <= pair.first){
            break;
        }
        previous = pair;
    }
    return previous.second;
}

[[nodiscard]] const std::vector<std::string>& HabitTracker::GetAvailableYears() const {
    return available_years_;
}

void HabitTracker::SetTitle(std::string title){
    title_ = title;
}

void HabitTracker::SetDescription(std::string description){
    description_ = description;
}

void HabitTracker::SetMetric(std::string metric){
    metric_ = metric;
}

void HabitTracker::AddThresholdColour(std::pair<double, std::string> threshold_colour){
    threshold_colours_.emplace(threshold_colour);
}

void HabitTracker::AddDay(std::pair<std::chrono::year_month_day, int> day_paring){
    data_.emplace(day_paring);
}

bool HabitTracker::operator<(const HabitTracker& other) const{
    return title_ < other.title_;
}


void HabitTracker::UpdateAvailableYears(){
    if(data_.empty()){
        return;
    }
    const size_t size = data_.size();
    std::chrono::year_month_day first = (*data_.begin()).first;
    std::chrono::year_month_day last =  (*std::prev(data_.end())).first;
    int start_year = static_cast<int>(first.year());
    int last_year = static_cast<int>(last.year());
    for(; start_year <= last_year; ++start_year){
        available_years_.emplace_back(std::to_string(start_year));
    }


}