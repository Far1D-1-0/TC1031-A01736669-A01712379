/* Laura Cintora Cendejas - A01712379
* Farid Gabriel Velasco Martínez - A01736669
 * Instrucciones de compilación:
 * g++ -std=c++20 -Wall -Wextra -pedantic -Werror main.cpp DateToNum.cpp fileReader.cpp LineRead.cpp Registro.cpp -o Act_Int_3
 * Ejecución (PowerShell):
 * ./Act_Int_3.exe
 * Ejecución (cmd/Unix/Linux):
 * ./Act_Int_3
 * Referencias:
 * https://www.geeksforgeeks.org/minimum-element-in-a-max-heap/
 * https://www.geeksforgeeks.org/insertion-and-deletion-in-heaps/
 * https://www.geeksforgeeks.org/heap-sort/
 * https://www.geeksforgeeks.org/priority-queue-in-cpp-stl/
 * https://www.geeksforgeeks.org/building-heap-from-array/
 * https://www.youtube.com/watch?v=pLIajuc31qk
 * https://www.youtube.com/watch?v=AE5I0xACpZs
 * https://www.geeksforgeeks.org/heap-data-structure/
 */

#include <iostream> // Para entrada y salida de la consola
#include <vector>
#include <fstream>
#include <chrono>

#include "Heap.h"
#include "Registro.h"
#include "LineRead.h"
#include "fileReader.h"
#include "DateToNum.h"

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
    DateToNum _dateToNum; // Aqui se esta declarando una instancia de DateToNum
    ofstream bitacora_ordenada("bitacora_ordenada.txt"); // Aqui el archivo guarda la bitacora ya ordenada
    ofstream ip_result("ips_con_mayor_acceso.txt"); // Se guardan las IPs con el mayor acceso
    fileReader bitacora_heap(' ', 5); // Se crean un objeto fileReader para poder leer la biacora
    bitacora_heap.openFile("bitacoraHeap.txt"); // Se abre el archivo de la bitacora
    vector<Registro> registros; // almacena los registros
    registros.reserve(bitacora_heap.numOfLines()); // Se reserva un espacio en el vector

    for (size_t i = 0; i < bitacora_heap.numOfLines(); i++) // Lee cada linea del archivo
    {
        bitacora_heap.setline(i); // Se esttablece la linea actual
        registros.emplace_back(bitacora_heap.lRead().getElements()); // Se agrega el registro al vector
    }

    bitacora_heap.closeFile(); // Se cierra el archivo de la bitacora

    Heap<Registro> registros_heap(registros, true); // Aqui se crea un heap de registros

    const auto registros_ordenados = registros_heap.heap_sort();  // Ordena los registros usando heap sort

    for (size_t i = 0; i < registros_ordenados.size(); i++) // Imprime los registros ordenados
    {
        registros_ordenados[i]->print_data(bitacora_ordenada); // Imprime cada registro en el archivo
    }

    vector<Heap<Registro>::Element_amount> ip_occurrence = registros_heap.count_simiar(); // Cuenta las ocurrencias de IPs

    Heap<Heap<Registro>::Element_amount> ip_occurrence_heap(ip_occurrence, true); // Crea un heap de ocurrencias de los IPs


    const auto ip_occurrence_ordenado = ip_occurrence_heap.heap_sort(); // Ordena las ocurrencias de IPs

    cout << "10 IPs con mayor cantidad de accesos: " << endl; // Mensaje de salida
    for (size_t i = ip_occurrence_ordenado.size() - 1; i > ip_occurrence_ordenado.size() - 11; i--)
    { // Imprime las 10 IPs con la mayor cantidad de accesos
        cout << "IP: " << ip_occurrence_ordenado[i]->element->get_ip_str() << " Accesos totales: " << ip_occurrence_ordenado[i]->amount << endl;
        ip_result << "IP: " << ip_occurrence_ordenado[i]->element->get_ip_str() << " Accesos totales: " << ip_occurrence_ordenado[i]->amount << endl;
    } cout << endl; // Imprime la IP y su cantidad de accesos

    cout << "IP con menor numero de accesos pero mayor o igual a 3: " << endl;

    Heap<Registro>::Element_amount ip_min3 = ip_occurrence_heap.find_min_if([](const Heap<Registro>::Element_amount& r)->bool{return r.amount >= 3;});
    // Imprime la IP encontrada y su cantidad
    cout << "IP: " << ip_min3.element->get_ip_str() << " Accesos totales: " << ip_min3.amount << endl;

    return 0;
}
