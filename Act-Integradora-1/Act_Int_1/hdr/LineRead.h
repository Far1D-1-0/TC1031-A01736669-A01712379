#ifndef LINEREAD_H
#define LINEREAD_H

#include <iostream>
#include <vector>

using std::vector;
using std::string;

/**
 * LineRead es una clase para manejar strings.
 * Se define el caracter separador de la string para poder
 * separar los elementos significativos en ella.
 * Se debe especificar el número de elementos significativos.\n
 * @note El formato de la string ingresada debe contener solo separadores entre los elementos, no a los extremos.\n
 * Ejemplo: sss:ss:sss\n En este caso el separador es ':' y hay 3 elementos significativos en la string.
 */
class LineRead {
public:
    LineRead();
    LineRead(const char& separator, const unsigned short int& numOfElements);

    void setSeparator(const char& separator);
    void setNumOfElements(const unsigned short int& numOfElements);
    /**
     * @param line string a leer.
     * @brief Llama a @code readLine(string line) \endcode
     *
     */
    void setLine(const string& line);

    /**
     * @return La string ingresada en  @code setLine(const string& line)\endcode
     * */
    [[nodiscard]] string getLine() const;
    /**
     *
     * @param element Indice del elemento deseado
     * @return string en el indice @code element\endcode en @code vector<string> elements\endcode
     */
    [[nodiscard]] string getElement(const unsigned short int& element) const;
    /**
     *
     * @return referencia constante a @code vector<string> elements\endcode
     */
    [[nodiscard]] const vector<string>& getElements() const;
    ~LineRead();
private:
    char separator = ' ';
    unsigned short int numOfElements = 1;
    string line;
    vector<string> elements;

    /**
     * Separa la string en varias strings individuales (elementos significativos).\n
     * El numero de strings que se obtienen de la string principal esta dado por @code numOfElements\endcode
     * @param line string a separar
     *
     */
    void readLine(string line);
};



#endif //LINEREAD_H
