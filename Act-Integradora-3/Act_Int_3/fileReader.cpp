#include "fileReader.h"

fileReader::fileReader()
= default; //Constructor por defecto el cual no no inicia valores de forma manual

fileReader::fileReader(const char& separator, const unsigned short int& numOfElements)
{
    lr.setSeparator(separator); // Separador para lineas del archivo
    lr.setNumOfElements(numOfElements); //Numero de elementos que son separados por cada linea
}

void fileReader::openFile(const string& filename)
{
    infile.open(filename, std::ios::in); //Abre el archivo en modo lectura

    if (!infile.is_open()) {
        throw std::runtime_error("Could not open file " + filename); //Da un comentario en caso de error
    }

    fileOpen = true; // Indica que el archivo esta abierto

    indexLines(); // Llama para indexar las lineas del archivo
}

void fileReader::setline(const size_t& line)
{
    if (!fileOpen) {
        throw std::runtime_error("File is not open"); // Si no se abre el archivo, da un aviso
    }

    if (line >= positions.size()) {
        throw std::invalid_argument("Invalid line index"); // Si el indice es incorrecto da un mensaje de error
    }

    infile.clear(); //Limpia los flags de error esto antes de poscionarse en la linea determinada
    infile.seekg(positions[line]); // posiciona el iterador de infile en la linea requerida
    string s;
    getline(infile, s); // lee la linea del archivo
    try
    {
        lr.setLine(s); // Guarda la linea, para mas adelante su procesamiento
    } catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

}

void fileReader::closeFile()
{
    infile.close(); //Cierra el archivo
    fileOpen = false; //Marca que el archivo ya no esta abierto
}

size_t fileReader::numOfLines() const
{
    return lines; // Retorna el numero de lineas que son indexadas
}

const LineRead& fileReader::lRead() //Devuelve la intacnia y ademas guarda la ultima linea que esta leida
{
    return lr;
}

fileReader::~fileReader()
= default; // DEstructor por defecto

void fileReader::indexLines()
{
    positions.clear(); // Cada vez que se carga un nuevo archivo limpia positions para indexarlo
    while (true)
    {
        std::streampos currentPosition = infile.tellg(); // Obtiene std::streampos de cada linea

        string line;
        if (!std::getline(infile, line)) { //Intenta leer una linea
            if (infile.eof()) break; // si llega al final del archivo sale del bucle
            throw std::runtime_error("Error reading from file");  // Da un mensaje en caso de error
        }

        if (!line.empty()) //
        {
            positions.push_back(currentPosition); // Guarda la posicion del inicio de la linea
        }
    }
    infile.clear(); //Limpia el flag de un error en infile

    lines = positions.size(); //Por ultimo guarda la cantidad total de lineas indexadas
}
