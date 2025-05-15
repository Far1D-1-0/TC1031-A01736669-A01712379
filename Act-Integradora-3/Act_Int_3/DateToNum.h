#ifndef DATETONUM_H
#define DATETONUM_H

#include <iostream> // Entrada y salida estandar
#include <vector> // Uso del contenedor vector
#include <chrono> // Manipulacion de tiempo y fechas
#include <ctime>   // Definición de estructuras y funciones de tiempo
#include <iomanip>  // Para manipulación de formato en entrada/salida
#include <sstream>  // Para usar stringstream y manipular cadenas

using std::string;
using std::size_t;
using std::vector;

class DateToNum // Declara clase
{ // Clase para convertir una fecha en string a distintos formatos numericos
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
    [[nodiscard]] string get_date() const; // Devuelve la fecha original en string
    [[nodiscard]] int get_month() const; // Devuelve el mes (0-11)
    [[nodiscard]] int get_m_day() const;  // Devuelve el dia del mes
    [[nodiscard]] int get_y_day() const; // Devuelve el dia del año
    [[nodiscard]] int get_hour() const;  // Devuelve la hora
    [[nodiscard]] int get_minute() const; // Devuelve los minutos
    [[nodiscard]] int get_second() const; // Devuelve los segundos
    [[nodiscard]] int get_t_seconds() const; // Devuelve los segundos totales del dia
    [[nodiscard]] int get_timestamp() const; // Devuelve el timestamp en formato UNIX
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
