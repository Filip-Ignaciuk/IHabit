#ifndef HABIT_TRACKER_HPP
#define HABIT_TRACKER_HPP

#include <chrono>
#include <map>
#include <vector>
class HabitTracker{
    public:
    HabitTracker() = default;

    HabitTracker(std::string title, 
        std::string description, 
        std::string metric, 
        std::map<double, std::string> threshold_colours);

    // Constructor for if you have data to fill out initially.
    HabitTracker(std::string title,
        std::string description,
        std::string metric,
        std::map<double, std::string> threshold_colours,
        std::map<std::chrono::year_month_day, double> data);

    [[nodiscard]] const std::string& GetTitle() const;
    [[nodiscard]] const std::string& GetDescription() const;
    [[nodiscard]] const std::string& GetMetric() const;
    [[nodiscard]] const std::map<double, std::string>& GetThresholdColours() const;
    [[nodiscard]] const std::map<std::chrono::year_month_day, double>& GetData() const;
    // Returns the colour threshold associated with that value.
    // If value is not exactly the same value it will find
    // The threshold colour that is the largest value
    // That is less than the value provided.
    [[nodiscard]] std::string GetColour(double value) const;
    [[nodiscard]] const std::vector<std::string>& GetAvailableYears() const;

    void SetTitle(const std::string &title);
    void SetDescription(const std::string &description);
    void SetMetric(const std::string &metric);
    void AddThresholdColour(std::pair<double, std::string> threshold_colour);
    void AddDay(std::chrono::year_month_day date, double value);

    bool operator<(const HabitTracker& other) const;
    
    private:
    std::string title_;
    std::string description_;
    std::string metric_;
    // We assume each metric is of type double, even if its an integer in reality.
    std::map<double, std::string> threshold_colours_;
    std::map<std::chrono::year_month_day, double> data_;
    std::vector<std::string> available_years_;
    void UpdateAvailableYears();
};

#endif