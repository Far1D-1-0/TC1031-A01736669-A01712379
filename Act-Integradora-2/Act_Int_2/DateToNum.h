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

class DateToNum // Declara clase
{
public:
    enum class int_Element // Conjunto de constantes
    {
        TIMESTAMP, MES, M_DIA, DIA, HORA, MIN, SEG, T_SEG
    };

    DateToNum(); // Constructor por defecto
    explicit DateToNum(const string& date); // Constructor que recibe una cadena

    void set_date(const string& date); //Funcion que permte modificar la fecha
    //almacenada dentro de la instancia esto acambio regrsa una cadena con nueva fecha

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
    [[nodiscard]] int get_int_data(const unsigned short int& element) const; // epecifica el elemento que se quiere consultar
    [[nodiscard]] static int days_to_timestamp_diff(const int& days); // Declaracion de metedo estatico (dias a numero de dias)

    ~DateToNum(); //Destructor de la clase
private:
    std::tm timeStruct{}; // Almacena los datos desglosados
    string time_string;
    string format = "%Y-%m-%d %H:%M:%S"; //Define el formato
    int int_data[8]{}; // Almacena distintos elementos numericos

    void date_to_int_elements();//Funcion privada de convertir la fecha
};

#endif //DATETONUM_H
