/* Laura Cintora Cendejas - A01712379
* Farid Gabriel Velasco Martínez - A01736669
 * Instrucciones de compilación:
 * g++ -std=c++20 -Wall -Wextra -pedantic -Werror main.cpp DateToNum.cpp fileReader.cpp LineRead.cpp Registro.cpp -o Act_Int_2
 * Ejecución (PowerShell):
 * Test 1: Get-Content TestCases/test01.txt | ./Act_Int_2.exe
 * Test 2: Get-Content TestCases/test02.txt | ./Act_Int_2.exe
 * Test 3: Get-Content TestCases/test03.txt | ./Act_Int_2.exe
 * Test 4: Get-Content TestCases/test04.txt | ./Act_Int_2.exe
 * Ejecución (cmd/Unix/Linux):
 * Test 1: ./Act_Int_2 < TestCases/test01.txt
 * Test 2: ./Act_Int_2 < TestCases/test02.txt
 * Test 3: ./Act_Int_2 < TestCases/test03.txt
 * Test 4: ./Act_Int_2 < TestCases/test04.txt
 * Referencias:
 * https://www.geeksforgeeks.org/cpp-binary-search/?ref=header_outind
 * https://www.geeksforgeeks.org/iterative-merge-sort/
 * https://www.geeksforgeeks.org/merge-sort/
 * https://www.geeksforgeeks.org/introduction-and-insertion-in-a-doubly-linked-list/?ref=header_outind
 * https://www.geeksforgeeks.org/doubly-linked-list/?ref=header_outind
 * https://www.naukri.com/code360/library/the-merge-sort-for-doubly-linked-list
 * Replit y actividades de clase
 */

#include <iostream>
#include <fstream>

#include "DateToNum.h"
#include "fileReader.h"
#include "Iterator_dll.h"
#include "LineRead.h"
#include "List_dl.h"
#include "Registro.h"

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;

int main()
{
    ofstream bitacora_ordenada("bitacora_ordenada.txt");
    ofstream resultado_busqueda("resultado_busqueda");
    fileReader file = {' ', 5};
    List_dl<Registro> registros;
    LineRead lr(' ', 5);

    if (!bitacora_ordenada || !resultado_busqueda)
    {
        cout << "Error: No se pudo crear o abrir el archivo." << endl;
        return 1;
    }

    try
    {
        file.openFile("bitacoraData.txt");
    } catch (const std::exception& e)
    {
        cout << e.what() << endl;
        return 2;
    }

    try
    {
        for (size_t i = 0; i < file.numOfLines(); i++)
        {
            file.setline(i);
            registros.add_last(Registro{file.lRead().getElements()});
        }
    } catch (const std::exception& e)
    {
        cout << e.what() << endl;
        return 3;
    }

    registros.merge_sort(true);

    for (Iterator_dll<Registro> it = registros.begin(); it != registros.end(); ++it)
    {
        it->print_data(bitacora_ordenada);
    }

    string fecha_inicio, fecha_fin;

    cout << "fecha inicio:";
    std::getline(cin >> std::ws, fecha_inicio);
    lr.setLine(fecha_inicio + " 1 def");
    Registro reg_inicio{lr.getElements()};

    cout << "fecha fin:";
    std::getline(cin >> std::ws, fecha_fin);
    lr.setLine(fecha_fin + " 1 def");
    Registro reg_fin(lr.getElements());
    cout << endl;
    cout << endl;

    List_dl<Registro> interval = registros.find_interval(reg_inicio, reg_fin, [](const Registro& r){return r;});

    if (interval.is_empty())
    {
        cout << "Resultado: 0 Registros" << endl;
        return 0;
    }

    string message = "Resultado: " + std::to_string(interval.get_size());
    message += (interval.get_size() == 1) ? " Registro" : " Registros";
    cout << message << endl;

    for (Iterator_dll<Registro> it = interval.begin(); it != interval.end(); ++it)
    {
        it->print_data(resultado_busqueda);
        cout << it->get_date() << endl;
    }

    return 0;
}