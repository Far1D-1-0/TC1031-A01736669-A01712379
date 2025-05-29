#ifndef REGISTRO_H
#define REGISTRO_H

#include "LineRead.h"
#include "DateToNum.h"
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

class Registro
{
public:
    enum class str_Element
    {
        MES, M_DIA, HORA, IP, MESSAGE
    };

    Registro(); // Constructor por defecto
    explicit Registro(const vector<string>& data);
    Registro(const Registro& other);

    [[nodiscard]] string get_str_Data(str_Element element) const;
    [[nodiscard]] string get_ip_str() const;
    [[nodiscard]] size_t get_ip_int() const;
    void print_data(std::ofstream& outFile) const;
    [[nodiscard]] const DateToNum& date() const;
    [[nodiscard]] string get_date() const;

    // Sobrecarga de operadores de comparacion por IP

    bool operator<(const Registro& r) const;
    bool operator<=(const Registro& r) const;
    bool operator>=(const Registro& r) const;
    bool operator>(const Registro& r) const;
    bool operator==(const Registro& r) const;
    bool operator!=(const Registro& r) const;

    Registro& operator=(const Registro& other);

    int operator()(const unsigned short int& element) const;

    // Metodos auxiliares para convertir los campos separados en una fecha
    [[nodiscard]] static string formatTimeString(string mes, const string& m_dia, const string& hora);
    [[nodiscard]] static string formatTimeString(const string& date);

    ~Registro();
private:
    static constexpr unsigned short int numOfStrElements = 5; // Total de elementos de tipo string
    inline static const string month[12] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    inline static LineRead lRead; // Instancia de LineRead
    DateToNum _dateToNum; // Instancia que convierte la fecha a valores numericos
    string str_data[numOfStrElements]; // Arreglo que almacena los datos que son el registro
    string date_str;  // Fecha formateada en string
    string ip_str = "000.000.000.000";
    size_t ip_int = 0;
};

#endif //REGISTRO_H

// Hash para Registro, permite usarlo en unordered_map
#include <functional>

namespace std {
    template <>
    struct hash<Registro> {
        std::size_t operator()(const Registro& r) const {
            return std::hash<size_t>()(r.get_ip_int());
        }
    };
}
