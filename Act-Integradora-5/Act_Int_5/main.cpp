/* Laura Cintora Cendejas - A01712379
* Farid Gabriel Velasco Martínez - A01736669
 * Instrucciones de compilación:
 * g++ -std=c++20 -Wall -Wextra -pedantic -Werror main.cpp fileReader.cpp LineRead.cpp IP.cpp -o Act_Int_5
 * Ejecución (PowerShell):
 * Test 1: Get-Content TestCases/test01.txt | ./Act_Int_5
 * Test 2: Get-Content TestCases/test02.txt | ./Act_Int_5
 * Test 3: Get-Content TestCases/test03.txt | ./Act_Int_5
 * Test 4: Get-Content TestCases/test04.txt | ./Act_Int_5
 * Ejecución (cmd/Unix/Linux):
 * Test 1: ./Act_Int_5 < TestCases/test01.txt
 * Test 2: ./Act_Int_5 < TestCases/test02.txt
 * Test 3: ./Act_Int_5 < TestCases/test03.txt
 * Test 4: ./Act_Int_5 < TestCases/test04.txt
 * Referencias:
 * https://www.geeksforgeeks.org/open-addressing-collision-handling-technique-in-hashing/
 * https://en.cppreference.com/w/cpp.html
 * https://en.cppreference.com/w/cpp/algorithm.html
 * https://en.cppreference.com/w/cpp/language/enum.html
 * https://en.cppreference.com/w/cpp/container/vector.html
 * https://www.geeksforgeeks.org/open-addressing-collision-handling-technique-in-hashing/
 * https://www.geeksforgeeks.org/quadratic-probing-in-hashing/
 * https://www.geeksforgeeks.org/load-factor-and-rehashing/
 * https://www.geeksforgeeks.org/friend-class-function-cpp/
 */

#include <iostream>
#include "fileReader.h"
#include "LineRead.h"
#include "Heap.h"
#include "ALDWGraph.h"
#include "HashTable.h"
#include "IP.h"

int main()
{
    fileReader bitacoraGrafos(' ', 2); // Aqui se crea lo del lector de archivos y con un separador y numero inicial de elemtos por liena siempre igual 2
    bitacoraGrafos.openFile("bitacoraGrafos.txt"); // Aqui se abre el archivo que tiene la bitacora y con sus nodos y ademas aristas del grafo
    ALDWGraph<IP> IP_Graph; // Se crea un grafo dirigido y ponderado que almacenará nodos de tipo IP
    bitacoraGrafos.setline(0); // Aqui se esta posicionando lo de la linea 0 pero en el archivo
    string aux = bitacoraGrafos.lRead().getElement(0);  // Aqui se obtiene lo que es la cantyidad  de nodos que empieza del  archivo
    size_t n = std::stoull(aux); // Se obtien aqui el numero de aristas que es desde el archivo
    aux = bitacoraGrafos.lRead().getElement(1);   // Ademas se configura el lector para esperar 1 solo dato
    size_t m = std::stoull(aux); // Se suman los nodos al grafo
    vector<string> IP_str;
    IP_str.reserve(n);

    bitacoraGrafos.lRead().setNumOfElements(1);

    for (size_t i = 1; i < n + 1; i++)
    {
        bitacoraGrafos.setline(i);  // Si la posicoon tiene correspondecia al nodo de la linea
        string ip = bitacoraGrafos.lRead().getLine(); // Empiza a  leer lo de la IP completa en string
        IP_str.push_back(ip);

        IP_Graph.add_node(IP(ip)); // Aqui se esta creando el nodo de la IP y ademas se le agrega al grafo
    }

    bitacoraGrafos.lRead().setNumOfElements(7); // En esta linea se configura el lector para hacer una lectura de 7 elementos

    for (size_t i = 0; i < m; i++) // Se le agregan las aristas al grafo
    {
        bitacoraGrafos.setline(bitacoraGrafos.numOfLines() - m + i); // Aqui se hace la posicion de  la linea con base a la arista de agohora
        LineRead lr(':', 2);
        lr.setLine(bitacoraGrafos.lRead().getElement(3));
        IP ip_from(lr.getElement(0));  // Aqui se estan creando las IPs del origen y destino
        lr.setLine(bitacoraGrafos.lRead().getElement(4));
        IP ip_to(lr.getElement(0));
        size_t weight = stoull(bitacoraGrafos.lRead().getElement(5)); // Se hace la obtencion del el peso
        IP_Graph.add_edge(ip_from, ip_to, weight); // Se le agrega a la arista lo del grafo
    }

    bitacoraGrafos.closeFile();  // Se cierra lo del archivo ya que este terminado con base al proceso

    HashTable<string, IP, std::hash<string>> IP_hashTable(n);
    for (size_t i  = 0; i < n; i++)
    {
        const string& temp_str = IP_str[i];
        size_t ic = IP_Graph.in_degree(IP(temp_str));
        size_t oc = IP_Graph.out_degree(IP(temp_str));
        double ior = static_cast<double>(oc) / static_cast<double>(ic);
        IP temp(temp_str, ic, oc, ior);

        IP_hashTable.insert(temp_str, temp);
    }
    /// Se le solicita al usuario una IP a buscar
    cout << "Colisiones totales en Tabla Hash: " << IP_hashTable.get_collisions() << endl;
    string search_ip;
    cout << "IP:"; // imprime el ip
    std::getline(std::cin,  search_ip);
    cout << " " +  search_ip << endl;

    IP* found = IP_hashTable.find(search_ip); // Aqui se busca la IP  que esta ingresada en la tabla hash
    found->getIPSummary(cout);
    cout << "Lista de direcciones accesadas: " << endl;
    auto neighbors = IP_Graph.get_neighbors(*found); // Aqui sVecinos del nodo buscado
    // Aqui se crea un heap con los vecinos que son ordenados por la I
    auto comp = [](const IP* a,  const IP* b) { return *a < *b; };
    // Se empieza a definir un comparador para  poder ordenar las IPs

    // Aqui se ordena todos los vecinos que esta usando heapsort
    Heap<IP*, decltype(comp)> heap(neighbors);
    // Imprime a  los vecinos que son  ordenados
    auto neighbors_ordered = heap.heap_sort();
    for (auto neighbor : neighbors_ordered)
    {
        cout << (*neighbor)->get_ip_s() << endl;
    }

    return 0; // Fin del programa
}
