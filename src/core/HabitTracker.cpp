#include "HabitTracker.hpp"
#include <iterator>
#include <string>
#include <utility>

HabitTracker::HabitTracker(std::string title, 
        std::string description, 
        std::string metric, 
        std::map<double, int> threshold_colours) :
        m_title(title),
        m_description(description),
        m_metric(metric),
        m_threshold_colours(threshold_colours){
        UpdateAvailableYears();
}

HabitTracker::HabitTracker(std::string title, 
        std::string description, 
        std::string metric, 
        std::map<double, int> threshold_colours,
        std::map<std::string, double> data) :
        m_title(title),
        m_description(description),
        m_metric(metric),
        m_threshold_colours(threshold_colours),
        m_data(data){
        UpdateAvailableYears();
}

const std::string& HabitTracker::GetTitle() const{
    return m_title;
}

const std::string& HabitTracker::GetDescription() const{
    return m_description;
}

const std::string& HabitTracker::GetMetric() const{
    return m_metric;
}

const std::map<double, int>& HabitTracker::GetThresholdColours() const{
    return m_threshold_colours;
}

const std::map<std::string, double>& HabitTracker::GetData() const{
    return m_data;
}

int HabitTracker::GetColour(double value) const{
    std::pair<double, int> previous;
    for(std::pair<double, int> pair : m_threshold_colours){
        if(value > pair.first){
            break;
        }
        previous = pair;
    }
    return previous.second;
}

void HabitTracker::SetTitle(std::string title){
    m_title = title;
}

void HabitTracker::SetDescription(std::string description){
    m_description = description;
}

void HabitTracker::SetMetric(std::string metric){
    m_metric = metric;
}

void HabitTracker::AddThresholdColour(std::pair<double, int> threshold_colour){
    m_threshold_colours.emplace(threshold_colour);
}

void HabitTracker::AddDay(std::pair<std::string, int> day_paring){
    m_data.emplace(day_paring);
}

bool HabitTracker::operator<(const HabitTracker& other) const{
    return m_title < other.m_title;
}


void HabitTracker::UpdateAvailableYears(){
    if(m_data.size() == 0){
        return;
    }
    std::string first = (*m_data.begin()).first;
    const int size = m_data.size();
    std::string last =  (*std::prev(m_data.end())).first;
    std::string start_year = first.substr(0, 4);
    std::string last_year = first.substr(0, 4);
    int start_year_int = std::stoi(start_year);
    int last_year_int = std::stoi(last_year);
    for(; start_year_int <= last_year_int; ++start_year_int){
        m_available_years.emplace_back(std::to_string(start_year_int));
    }


}