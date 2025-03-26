/* Laura Cintora Cendejas - A01712379
 * Farid Gabriel Velasco Martínez - A01736669
 * Instrucciones de compilación:
 * g++ -Wall -Wextra -pedantic -Werror -c main.cpp -o main.o
 * g++ -Wall -Wextra -pedantic -Werror -c src\DateToNum.cpp -o DateToNum.o
 * g++ -Wall -Wextra -pedantic -Werror -c src\fileReader.cpp -o fileReader.o
 * g++ -Wall -Wextra -pedantic -Werror -c src\LineRead.cpp -o LineRead.o
 * g++ -Wall -Wextra -pedantic -Werror -c src\Registro.cpp -o Registro.o
 * g++ main.o DateToNum.o fileReader.o LineRead.o Registro.o -o Act_Int_1.exe
 * Ejecución (PowerShell):
 * Test 1: Get-Content TestCases/test01.txt | ./Act_Int_1.exe
 * Test 2: Get-Content TestCases/test02.txt | ./Act_Int_1.exe
 * Test 3: Get-Content TestCases/test03.txt | ./Act_Int_1.exe
 * Test 4: Get-Content TestCases/test04.txt | ./Act_Int_1.exe
 * Ejecución (cmd/Unix/Linux):
 * Test 1: ./Act_Int_1.exe < TestCases/test01.txt
 * Test 2: ./Act_Int_1.exe < TestCases/test02.txt
 * Test 3: ./Act_Int_1.exe < TestCases/test03.txt
 * Test 4: ./Act_Int_1.exe < TestCases/test04.txt
 * Referencias:
 * https://www.geeksforgeeks.org/date-and-time-parsing-in-cpp/
 * https://www.geeksforgeeks.org/timsort/
 * https://www.geeksforgeeks.org/find-a-pair-with-the-given-difference/
 * https://www.geeksforgeeks.org/counting-sort/
 * https://www.geeksforgeeks.org/quick-sort-algorithm/
 * https://www.geeksforgeeks.org/binary-search/
 * https://www.epochconverter.com/
 * https://www.geeksforgeeks.org/operator-overloading-cpp/
 * https://www.geeksforgeeks.org/static-member-function-in-cpp/
 * https://www.geeksforgeeks.org/cpp-17-inline-variables/
 * Replit y Presentaciones de clase
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

#include "hdr/fileReader.h"
#include "hdr/LineRead.h"
#include "hdr/DateToNum.h"
#include "hdr/Registro.h"
#include "hdr/Sort.h"
#include "hdr/Search.h"

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;
using std::stoi;

int main()
{
    DateToNum dateToNum; // Objeto DateToNum para convertir la diferencia de días para la busqueda de pares a diferencia de TimeStamps
    string aux; // Variable auxiliar para manejar strings de entrada o generadas en el código
    size_t comp = 0; // contador de comparaciones
    size_t swap = 0; // contador de swaps
    size_t operaciones = 0; // Variable auxiliar para guardar el input de la cantidad de operaciones de consulta a realizar
    int D = 0; // Variable para la cantidad de días de diferencia (Llave para la busqueda por pares)
    size_t o_e = 0; // contador operaciones exitosas
    size_t o_ne = 0; // contador operaciones no exitosas
    ofstream bitacora_ordenada("OutFiles/bitacora_ordenada.txt"); // Inicialización del archivo de registros ordenados
    ofstream resultados_busqueda("OutFiles/resultados_busqueda.txt"); // Inizialización del archivo de los resultados de búsqueda
    const string filename = "InFiles/bitacoraData.txt"; // Path del archivo txt de datos a leer
    fileReader file(' ', 5); // Declaración de un objeto fileReader para leer el archivo de datos
    file.openFile(filename); // Apertura e indexación del archivo de datos
    vector<Registro> registros; // Declaración del vector de objetos 'Registro' para almacenar los datos del archivo de texto
    registros.reserve(file.numOfLines()); // Reserva espacio para todos los registros

    for (size_t i = 0; i < file.numOfLines(); i++)
    { // Recorre cada linea del archivo bitacoraData
        file.setline(i);
        registros.emplace_back(file.lRead().getElements()); // Crea un objeto Registro al final del vector con los Datos leidos de file
    }
    file.closeFile(); // Cierra el archivo de datos

    // Ordena el vector de objetos Registro. Two Step Sort utiliza Counting Sort y Quick Sort para optimizar
    // las comparaciones, swaps y el tiempo de ordenamiento del vector (Ordena primero por Dia y luego por
    // la cantidad total de segundos de la hora de cada registro)
    Sort<Registro>::two_step_sort(registros, 364, static_cast<int>(DateToNum::int_Element::DIA),
                                  static_cast<int>(DateToNum::int_Element::T_SEG), comp, swap);

    for (size_t i = 0; i < registros.size(); i++) // Recorre el vector registros
    {
        registros[i].print_data(bitacora_ordenada); // Imprime en el archivo bitacora_ordenada el vector ordenado
        (i == registros.size() - 1) ? bitacora_ordenada << "" : bitacora_ordenada << endl;
    }

    cin >> operaciones; // Recibe la cantidad de operaciones a realizar (Primer input)
    cin.ignore(); // Ignora "\n" dejado por cin para leer la siguiente linea
    for (size_t i = 0; i < operaciones; i++) // Itera las veces necesarias en operaciones
    {
        getline(cin, aux); // Lee la fecha completa introducida (Segundo input)
        dateToNum.set_date(Registro::formatTimeString(aux)); // Usa la función estática de Registro 'formatTimeString()'
        // para introducir la fecha con el formato correcto en 'set_date()' del objeto DateToNum

        // Realiza la búsqueda por busqueda binaria de la fecha introducida en aux, en el vector de registros ordenado.
        // La llave de búsqueda es el TimeStamp de las fechas en cuestion.
        int binary_result = Search<Registro>::binary_custom(registros, 0, static_cast<int>(registros.size()) - 1,
                                                            dateToNum.get_timestamp(),
                                                            static_cast<int>(DateToNum::int_Element::TIMESTAMP));
        if (binary_result == -1)
        { // Si la fecha no existe (return -1) imprime en el archivo resultados_busqueda el mensaje de error
            resultados_busqueda << "Fecha no encontrada: " << aux;
            o_ne += 1; // Contabiliza una busqueda no exitosa
        } else
        {
            resultados_busqueda << "[" << binary_result << "] "; // Imprime el índice de la fecha encontrada en el formato [i]
            registros[binary_result].print_data(resultados_busqueda); // Imprime los datos de la fecha encontrada
            o_e += 1; // Contabiliza una busqueda exitosa
        }
        resultados_busqueda << endl;
    }
    aux = "";
    cin >> D; // Recibe los días de diferencia entre un par de registros a encontrar
    int timestamp_diff = DateToNum::days_to_timestamp_diff(D); // Convierte la diferencia de días a diferencia de TimeStamp
    // para contar la extension completa de los D días. (Para incluir hasta el último segundo de una fecha)

    // Busca el primner par de registros con D dias de diferencia por el parametro TimeStamp y regresa un vector con los índices
    vector<int> find_by_interval_result = Search<Registro>::find_by_interval(registros, timestamp_diff, static_cast<int>(DateToNum::int_Element::TIMESTAMP));

    resultados_busqueda << "Busqueda del primer par de registros con " << D << " dias de diferencia: " << endl;
    for (unsigned short int i = 0; i < 2; i++) // Recorre el par
    {
        if (find_by_interval_result[i] == -1) // Si el primer elemento es -1, la busqueda no fue exitosa
        {
            resultados_busqueda << "No existe dicho par de registros" << endl; // Imprime mensaje de error
            aux = "No existe dicho par de registros"; // Guarda el mensaje en una cadena para imprimirlo despues en la terminal
            break;
        }
        // Genera la string con el resultado para imprimirla despues en la terminal
        aux += "[" + std::to_string(find_by_interval_result[i]) + "] " + registros[find_by_interval_result[i]].get_date() + "\n";

        // Imprime los resultados en el archvo de resultados_busqueda
        resultados_busqueda << "[" << find_by_interval_result[i] << "] ";
        registros[find_by_interval_result[i]].print_data(resultados_busqueda);
        (i == 1) ? resultados_busqueda << "" : resultados_busqueda << endl;
    }

    cout << "Two Step Sort -> Counting Sort + Quick Sort" << endl; // Tipo de algoritmo de ordenamiento empleado
    cout << "Comparaciones: " << comp << endl; // Comparaciones totales
    cout << "Swap: " << swap << endl << endl; // Swaps totales
    cout << "Numero total de operaciones de busquedas por fecha solicitadas: " << operaciones << endl;
    cout << "Numero total de busquedas exitosas: " << o_e << endl;
    cout << "Numero total de busquedas no exitosas: " << o_ne << endl << endl;
    cout << "Busqueda del primer par de registros con " << D << " dias de diferencia: " << endl;
    cout << aux << endl; // Imprime el resultado de la bsuqueda de pares

    return 0; // Fin del programa
}
