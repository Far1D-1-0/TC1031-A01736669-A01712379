#include "LineRead.h"

LineRead::LineRead()
= default; // Constructor por defecto, no inicia los valores de forma manual

LineRead::LineRead(const char& separator, const unsigned short int& numOfElements)
{
    setSeparator(separator); // Establece el separador para dividir las lineas por partes
    setNumOfElements(numOfElements); // Va definiendo la cantidad de elementos que se deben de extraer de una linea
}

void LineRead::setSeparator(const char& separator)
{
    this->separator = separator; //Establece separador
}

void LineRead::setNumOfElements(const unsigned short int& numOfElements)
{
    this->numOfElements = numOfElements;
    elements.reserve(numOfElements); // Reserva espacio de memoria para el vector elements de acuerdo
                                     // a la cantidad de elementos que contendrá la string
}

void LineRead::setLine(const string& line)
{
    if (line.empty()) {
        throw std::invalid_argument("line is empty"); // Si la linea esta vacia, da un mensaje
    }
    this->line = line; // Guarda la linea en la variable clase
    readLine(this->line);  //Hace un llamado a readline para procesar su linea y poder extraer sus partes
}

string LineRead::getLine() const
{
    return line; //Devuelve la linea alamcenada en string
}

string LineRead::getElement(const unsigned short int& element) const
{
    if (element >= numOfElements)
    {
        throw std::invalid_argument("element is out of range");
    }
    return elements[element]; //Regresa el elemento en la posicion element que indique el vector
}

const vector<string>& LineRead::getElements() const
{
    return this->elements; //Regresa el vector con los elemnetos que fueron extraidos
}

LineRead::~LineRead() //Destructor por defecto
= default;

void LineRead::readLine(string line)
{
    elements.clear(); // Cada ves que readLine es llamado, se limpia 'elements' para guardar los elementos de la nueva linea
    if (line.find(separator) ==  string::npos) //Verifica si el separado se encuentra presente en la linea
    {
        throw std::runtime_error("Line does not contain separator");
    }

    for (int i = 0; i < numOfElements; i++) {
        if (i == numOfElements - 1) { // Si se encuentra en la ultima iteracion, lo que resta de la linea
                                      // se toma como el último elemento a obtener
            elements.push_back(line);
            break;
        }

        size_t sepPos = line.find(separator);
        if (sepPos == std::string::npos) {
            throw std::runtime_error("Not enough elements in line");
        }

        elements.push_back(line.substr(0, sepPos));
        line = line.substr(sepPos + 1);
    }
}
