#ifndef REGISTRO_H
#define REGISTRO_H

#include "LineRead.h"
#include "DateToNum.h"
#include <fstream>

class Registro
{
public:
    enum class str_Element
    {
        MES, M_DIA, HORA, IP, MESSAGE
    };

    explicit Registro(const vector<string>& data);

    [[nodiscard]] string get_str_Data(str_Element element) const;
    void print_data(std::ofstream& outFile) const;
    [[nodiscard]] const DateToNum& date() const;
    [[nodiscard]] string get_date() const;

    bool operator<(const Registro& r) const;
    int operator()(const unsigned short int& element) const;

    [[nodiscard]] static string formatTimeString(string mes, const string& m_dia, const string& hora);
    [[nodiscard]] static string formatTimeString(const string& date);

    ~Registro();
private:
    static constexpr unsigned short int numOfStrElements = 5;
    inline static const string month[12] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    inline static LineRead lRead;
    DateToNum _dateToNum;
    string str_data[numOfStrElements];
    string date_str;
};
#endif //REGISTRO_H
