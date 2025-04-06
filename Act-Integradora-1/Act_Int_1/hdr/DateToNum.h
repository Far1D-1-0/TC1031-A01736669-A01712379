#ifndef DATETONUM_H
#define DATETONUM_H

#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

using std::string;
using std::size_t;
using std::vector;

class DateToNum
{
public:
    enum class int_Element
    {
        TIMESTAMP, MES, M_DIA, DIA, HORA, MIN, SEG, T_SEG
    };

    DateToNum();
    explicit DateToNum(const string& date);

    void set_date(const string& date);

    //getters
    [[nodiscard]] string get_date() const;
    [[nodiscard]] int get_month() const;
    [[nodiscard]] int get_m_day() const;
    [[nodiscard]] int get_y_day() const;
    [[nodiscard]] int get_hour() const;
    [[nodiscard]] int get_minute() const;
    [[nodiscard]] int get_second() const;
    [[nodiscard]] int get_t_seconds() const;
    [[nodiscard]] int get_timestamp() const;
    [[nodiscard]] int get_int_data(const unsigned short int& element) const;
    [[nodiscard]] static int days_to_timestamp_diff(const int& days);

    ~DateToNum();
private:
    std::tm timeStruct{};
    string time_string;
    string format = "%Y-%m-%d %H:%M:%S";
    int int_data[8]{};

    void date_to_int_elements();
};

#endif //DATETONUM_H
