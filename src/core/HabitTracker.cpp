#include "HabitTracker.hpp"
#include <chrono>
#include <iterator>
#include <ranges>
#include <set>
#include <string>
#include <utility>

HabitTracker::HabitTracker(std::string title,
        std::string description, 
        std::string metric, 
        std::map<double, std::string> threshold_colours) :
        title_(std::move(title)),
        description_(std::move(description)),
        metric_(std::move(metric)),
        threshold_colours_(std::move(threshold_colours)){
        UpdateAvailableYears();
}

HabitTracker::HabitTracker(std::string title,
        std::string description,
        std::string metric,
        std::map<double, std::string> threshold_colours,
        std::map<std::chrono::year_month_day, double> data) :
        title_(std::move(title)),
        description_(std::move(description)),
        metric_(std::move(metric)),
        threshold_colours_(std::move(threshold_colours)),
        data_(std::move(data)){
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

std::string HabitTracker::GetColour(const double value) const{
    std::pair<double, std::string> previous = *threshold_colours_.begin();
    for(const std::pair<double, std::string> pair : threshold_colours_){
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

void HabitTracker::SetTitle(const std::string &title){
    title_ = title;
}

void HabitTracker::SetDescription(const std::string &description){
    description_ = description;
}

void HabitTracker::SetMetric(const std::string &metric){
    metric_ = metric;
}

void HabitTracker::AddThresholdColour(std::pair<double, std::string> threshold_colour){
    threshold_colours_.emplace(threshold_colour);
}

void HabitTracker::AddDay(std::chrono::year_month_day date, double value){
    data_.emplace(date, value);
    UpdateAvailableYears();
}

bool HabitTracker::operator<(const HabitTracker& other) const{
    return title_ < other.title_;
}


void HabitTracker::UpdateAvailableYears(){
    if(data_.empty()){
        return;
    }
    available_years_.clear();
    std::set<std::string> available_years;
    for (const auto &key: data_ | std::views::keys) {
        available_years.insert(std::to_string(static_cast<int>(key.year())));
    }

    for (const std::string& year : available_years) {
        available_years_.push_back(year);
    }

}