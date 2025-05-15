#include "Registro.h"

Registro::Registro()
= default;

Registro::Registro(const vector<string>& data)
{
    for (unsigned short int i = 0; i < numOfStrElements; i++)
    {
        str_data[i] = data[i]; // Guarda cada campo del vector en estas variables
    }

    for (const string&  element : str_data)
    {
        date_str += element + " "; // Concatena los valores
    }
    date_str = date_str.substr(0, date_str.size() - 1); //Elimina el espacio extra final

    _dateToNum.set_date(formatTimeString(str_data[static_cast<int>(str_Element::MES)], str_data[static_cast<int>(str_Element::M_DIA)],
                     str_data[static_cast<int>(str_Element::HORA)])); //Extrae el mes, el dia del mes y la hora completa
    lRead.setSeparator(':'); lRead.setNumOfElements(2);
    lRead.setLine(str_data[static_cast<int>(str_Element::IP)]);
    string aux = lRead.getElement(0);
    ip_str = aux;
    lRead.setSeparator('.'); lRead.setNumOfElements(4);
    lRead.setLine(aux);
    aux.clear();
    for (const string&  element : lRead.getElements())
    {
        aux += element;
    }

    try
    {
        ip_int = std::stoull(aux);
    } catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

bool Registro::operator<(const Registro& r) const
{
    return ip_int < r.ip_int; // Compara solo IP sin puerto
}

bool Registro::operator<=(const Registro& r) const // Menor o igual por IP
{
    return ip_int <= r.ip_int;
}

bool Registro::operator>=(const Registro& r) const // Mayor o igual por IP
{
    return ip_int >= r.ip_int;
}

bool Registro::operator>(const Registro& r) const // Mayor que IP
{
    return ip_int > r.ip_int;
}

bool Registro::operator==(const Registro& r) const // Igualdad por IP
{
    return ip_int == r.ip_int;
}

inline bool Registro::operator !=(const Registro& r) const
{
    return ip_int != r.ip_int;
}

// AQUI PA ABAJO TENGO QUE REVISAR == se mantienen igual

string Registro::get_str_Data(str_Element element) const
{
    return str_data[static_cast<int>(element)]; //Regresa el dato que corresponde al elemento indicado
}

string Registro::get_ip_str() const
{
    return ip_str;
}

size_t Registro::get_ip_int() const
{
    return ip_int;
}

void Registro::print_data(std::ofstream& outFile) const
{
    outFile << date_str << std::endl;; //Imprimir datos en un archivo
}

const DateToNum& Registro::date() const
{
    return _dateToNum; //Obtneer la fecha como objeto
}

string Registro::get_date() const //Obtener la fecha en formato string
{
    return date_str;
}

int Registro::operator()(const unsigned short int& element) const //Sobrecarga del operador
{
    return date().get_int_data(element);
}

Registro::~Registro()
= default;

string Registro::formatTimeString(string mes, const string& m_dia, const string& hora)
{
    int monthsNum = 0; // Inicia la variable para guardar el numero inidicado al mes

    for (unsigned short int i = 0; i < 12; i++) //Busca el mes en el array
    {
        if (month[i] == mes) //Si encuentra una coincidencia entre el mes y el array
        {
            monthsNum = i + 1; // el indice en numero mes conversion
            if (monthsNum < 10)
            {
                mes = "0" + std::to_string(monthsNum); //El numero se conveirte string y se agrega el 0
            }
            else
            {
                mes = std::to_string(monthsNum); //convierte el numero de string sin agregar un 0
            }
            break; // sale del bcle
        }
    }

    return "2025-" + mes +  "-" + m_dia + " " + hora; //construye y devuerlve la fecha en el formato indicicado
}

string Registro::formatTimeString(const string& date)
{
    lRead.setSeparator(' '); lRead.setNumOfElements(3); //Inidca que tiene tres partes la fecha
    lRead.setLine(date); //guarda la fecha recibida y la divide en partes

    string mes = lRead.getElement(0);
    const string m_dia = lRead.getElement(1); //obtiene el dia del mes
    const string hora = lRead.getElement(2); // obtiene la hora en el formato especifico

    int monthsNum = 0;  //Inicia la variable para alamcenar el numero del mes

    for (unsigned short int i = 0; i < 12; i++)
    {
        if (month[i] == mes) //
        {
            monthsNum = i + 1; // Convierte el índice en número de mes
            if (monthsNum < 10) // Si el numeero es menor de 10 se le agregaun 0
            {
                mes = "0" + std::to_string(monthsNum); //Convierte el numero en string y agregar un 0
            }
            else
            {
                mes = std::to_string(monthsNum); //Convierte el numero en string sin agregar un 0
            }
            break; //Sale dle bucle
        }
    }

    return "2025-" + mes +  "-" + m_dia + " " + hora;

}
