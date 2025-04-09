#include "DateToNum.h" // Es el encabezado que define a la clase DateToNum

DateToNum::DateToNum() //Constructor por defecto
= default; //Crea la instacia vacia de una clase

DateToNum::DateToNum(const string& date) // Constructor que recibira la fecha en string
{
    set_date(date); // Procesa y alamcena la fecha
}

void DateToNum::set_date(const string& date) // Metodo para indicar la fecha y hacer conversion de dato numerico
{
    time_string = date; // Guarda la fecha en string
    date_to_int_elements(); // Convertir y almacernar la fecha
}

void DateToNum::date_to_int_elements() // Fecha convertida en valor numerico de forma singular en string
{
    std::istringstream ss(time_string); // Flujo de entrada para analizar la fecha en "time_string"
    ss >> std::get_time(&timeStruct, format.c_str()); // Conversion de string a una estructura de tiempo
    timeStruct.tm_isdst = 0; //
    const time_t time_stamp = std::mktime(&timeStruct); // Convierte "tm" en un timestamp
    const int t_seg = 3600*timeStruct.tm_hour +  60*timeStruct.tm_min + timeStruct.tm_sec; //Realiza el calculo de los segundos totales de un dia
    //Almacen os valores del array en "int_data"
    int_data[static_cast<int>(int_Element::TIMESTAMP)] = static_cast<int>(time_stamp); // Guarda el timestamp
    int_data[static_cast<int>(int_Element::MES)] = timeStruct.tm_mon; // Guarda el mes
    int_data[static_cast<int>(int_Element::M_DIA)] = timeStruct.tm_mday; //Guarda el dia de un mes especifico
    int_data[static_cast<int>(int_Element::DIA)] = timeStruct.tm_yday; //Almacena el dia del año
    int_data[static_cast<int>(int_Element::HORA)] = timeStruct.tm_hour; // Almacena la hora
    int_data[static_cast<int>(int_Element::MIN)] = timeStruct.tm_min; // Alamcena los minutos
    int_data[static_cast<int>(int_Element::SEG)] = timeStruct.tm_sec; //Almacena los segundo
    int_data[static_cast<int>(int_Element::T_SEG)] = t_seg; // Guarda los segundos que ocurrieron en un dia especifico

}

string DateToNum::get_date() const // Regresa la fecha en formato string
{
    return time_string; //Retorna formato origal en string (fecha)
}

int DateToNum::get_month() const // Devuelve el mes de la fecha
{
    return int_data[static_cast<int>(int_Element::MES)]; //Regresa el mes almacenado
}

int DateToNum::get_m_day() const //Devuelve el dia del mes
{
    return int_data[static_cast<int>(int_Element::M_DIA)];  // Regresa el dia del mes alamacenado
}

int DateToNum::get_y_day() const //Devuelve el dia del año
{
    return int_data[static_cast<int>(int_Element::DIA)]; // Regresa el dia del año alamacenado
}

int DateToNum::get_hour() const // Devuelve la hora
{
    return int_data[static_cast<int>(int_Element::HORA)];  //Devuelve la hora guardada
}

int DateToNum::get_minute() const //Regresa los minutos
{
    return int_data[static_cast<int>(int_Element::MIN)]; // Retorna los minutos almacenados
}

int DateToNum::get_second() const //Regresa los segundos
{
    return int_data[static_cast<int>(int_Element::SEG)]; //Retorna los segundo almacenados
}

int DateToNum::get_t_seconds() const //Regresa los segundo totales que pasan en un dia
{
    return int_data[static_cast<int>(int_Element::T_SEG)];
}

int DateToNum::get_timestamp() const //Devuelve el timestamp
{
    return int_data[static_cast<int>(int_Element::TIMESTAMP)];
}

int DateToNum::get_int_data(const unsigned short int& element) const  // Devuelve el valor determinado de int_data
{
    return int_data[element]; //Regresa el valor numerico que corresponde
}

int DateToNum::days_to_timestamp_diff(const int& days)
{
    constexpr int day_1_timestamp = 1735711200;
    constexpr int year = 2025;
    std::tm timeStruct{};
    timeStruct.tm_year = year - 1900;
    timeStruct.tm_mday = days + 1;
    timeStruct.tm_mon = 0;
    timeStruct.tm_isdst = 0;

    const std::time_t time_stamp = std::mktime(&timeStruct);

    return static_cast<int>(time_stamp) - day_1_timestamp;

}

DateToNum::~DateToNum() // Destrutor de la clase
= default;


