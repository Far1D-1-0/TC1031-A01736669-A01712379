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

    const LineRead& lRead(); // Regresa una referencia constante al atributo lr de tipo LineRead

    ~fileReader();
private:
    LineRead lr;
    ifstream infile;
    vector<std::streampos> positions;
    bool fileOpen = false;
    size_t lines = 0;

    /**
     * Indexa el indice de cada linea del archivo en el atributo @code std::vector<std::streampos> positions@endcode
     * Para un acceso aleatorio más eficiente.
     */
    void indexLines();
};



#endif //FILEREADER_H
