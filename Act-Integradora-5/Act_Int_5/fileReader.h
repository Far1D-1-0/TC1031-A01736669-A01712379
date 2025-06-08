#ifndef FILEREADER_H
#define FILEREADER_H

#include "LineRead.h"
#include <fstream>

using std::ifstream;

/**
 * @class fileReader
 * Maneja un archivo de texto.\n
 * Incluye @code fstream\endcode para leer el archivo.\n Indexa sus lineas para un acceso
 * más eficiente a una linea aleatoria en el archivo.\n
 * Incluye @code LineRead\endcode para analizar cada linea del archivo si se requiere.
 */
class fileReader {
public:
    fileReader();
    fileReader(const char& separator, const unsigned short int& numOfElements);

    /**
     * Abre el archivo de texto especificado y llama a @code indexLines@endcode para indexarlo
     * @param filename Archivo de texto a leer
     */
    void openFile(const string& filename);
    /**
     *
     * @param line Indice de la linea del archivo a leer
     * @note Usa el atributo @code positions@endcode para saltar directamente al indice de la linea deseada.
     */
    void setline(const size_t& line);
    void closeFile(); // cierra el archivo

    [[nodiscard]] size_t numOfLines() const; // Regresa el tamaño del vector positions

    const LineRead& clRead(); // Regresa una referencia constante al atributo lr de tipo LineRead
    LineRead& lRead(); // Regresa una referencia al atributo lr de tipo LineRead

    ~fileReader(); //Declaracion del desctructor de la clase
private:
    bool fileOpen = false; //Indica si el archivo esta abierto
    size_t lines = 0; //Almacena el numero de lineas del archivo
    LineRead lr; //Analza cada linea
    ifstream infile; //Abre y lee el archivo
    vector<std::streampos> positions; //almacena los objetos de tipo steampos


    /**
     * Indexa el indice de cada linea del archivo en el atributo @code std::vector<std::streampos> positions@endcode
     * Para un acceso aleatorio más eficiente.
     */
    void indexLines();
};



#endif //FILEREADER_H
