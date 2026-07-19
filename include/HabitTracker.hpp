#ifndef HABIT_TRACKER_HPP
#define HABIT_TRACKER_HPP

#include <map>
#include <string>
#include <utility>
#include <vector>

class HabitTracker{
    public:
    HabitTracker(std::string title, 
        std::string description, 
        std::string metric, 
        std::map<double, int> threshold_colours);

    // Constructor for if you have data to fill out initially.
    HabitTracker(std::string title, 
        std::string description, 
        std::string metric, 
        std::map<double, int> threshold_colours,
        std::map<std::string, double> data);

    const std::string& GetTitle() const;
    const std::string& GetDescription() const;
    const std::string& GetMetric() const;
    const std::map<double, int>& GetThresholdColours() const;
    const std::map<std::string, double>& GetData() const;
    int GetColour(double value) const;

    void SetTitle(std::string title);
    void SetDescription(std::string description);
    void SetMetric(std::string metric);
    void AddThresholdColour(std::pair<double, int> threshold_colour);
    void AddDay(std::pair<std::string, int> day_paring);

    bool operator<(const HabitTracker& other) const;
    
    private:
    std::string m_title;
    std::string m_description;
    std::string m_metric;
    // We assume each metric is of type double, even if its an integer in reality.
    std::map<double, int> m_threshold_colours;
    std::map<std::string, double> m_data;
    std::vector<std::string> m_available_years {};
    void UpdateAvailableYears();
};

#endif