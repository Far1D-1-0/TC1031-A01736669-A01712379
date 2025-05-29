/* Laura Cintora Cendejas - A01712379
* Farid Gabriel Velasco Martínez - A01736669
 * Instrucciones de compilación:
 * g++ -std=c++20 -Wall -Wextra -pedantic -Werror main.cpp DateToNum.cpp fileReader.cpp LineRead.cpp Registro.cpp -o Act_Int_4
 * Ejecución (PowerShell):
 * ./Act_Int_4.exe
 * Ejecución (cmd/Unix/Linux):
 * ./Act_Int_4
 * Referencias:
 * https://en.cppreference.com/
 * https://google.github.io/styleguide/cppguide.html
 * https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-using-priority_queue-stl/
 * https://www.geeksforgeeks.org/binary-heap/
 * https://www.geeksforgeeks.org/graph-and-its-representations/
 * https://en.cppreference.com/w/cpp/container/vector.html
 * https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-using-priority_queue-stl/
 * https://www.programiz.com/dsa/graph-adjacency-list
 */


#include <iostream>

#include "fileReader.h"
#include "LineRead.h"
#include "Registro.h"
#include "Heap.h"
#include "ALDWGraph.h"
#include "DateToNum.h"

int main()
{
    fileReader input_file(' ', 2); // Se inicializa el archivo de entrada con un espacio como separador y dos elementos por linea
    input_file.openFile("bitacoraGrafos.txt"); // Se abre el archivo de entrada
    ALDWGraph<Registro> registros_graph; // Se inicializa el grafo
    vector<Registro> registros_i; // Se inicializa el vector de registros

    input_file.setline(0); // Se lee la primera linea del archivo
    string aux = input_file.lRead().getElement(0); // Se extrae el numero de nodos y aristas del archivo
    size_t n = std::stoull(aux); // Se convierte el string a un numero
    aux = input_file.lRead().getElement(1); // Se extrae el numero de nodos y aristas del archivo
    size_t m = std::stoull(aux); // Se convierte el string a un numero
    registros_i.reserve(m); // Se reserva el espacio necesario para los nodos

    input_file.lRead().setNumOfElements(1); // Se lee la segunda linea del archivo

    for (size_t i = 1; i < n + 1; i++) // Se leen los nodos
    {
        input_file.setline(i); // Se lee la linea del archivo
        string ip = input_file.lRead().getLine() + ":0000"; // Se agrega el puerto al ip

        vector<string> data = {"Jan", "01", "00:00:01", ip, "ndef"}; // Se crea un vector con los datos del nodo
        registros_graph.add_node(Registro(data)); // Se agrega el nodo al grafo
    }

    input_file.lRead().setNumOfElements(7); // Se lee la tercera linea del archivo

    for (size_t i = 0; i < m; i++) // Se leen las aristas
    {
        input_file.setline(input_file.numOfLines() - m + i); // parte de la linea n + 1
        // Guarda cada registro de acceso en un vector
        registros_i.emplace_back(vector<string> {input_file.lRead().getElement(0), input_file.lRead().getElement(1), input_file.lRead().getElement(2), input_file.lRead().getElement(3), input_file.lRead().getElement(6)});

        // Crea un objeto pair<Registro, size_t> auxiliar para agregar la arista al grafo
        pair<Registro, size_t> edge = {Registro(vector<string> {"Jan", "01", "00:00:01", input_file.lRead().getElement(4), "ndef"}), std::stoull(input_file.lRead().getElement(5))};
        registros_graph.add_edge(registros_i[i], edge.first, edge.second); // agrega la arista al grafo
    }

    input_file.closeFile(); // Se cierra el archivo de entrada

    std::ofstream grados_out("grados_ips.txt"); // Se crea un archivo para guardar los grados de salida
    list<const Registro*> top_degree_ips; // Lista para guardar los ips con mayor grado de salida

    auto grados_salida = registros_graph.out_degrees(true, true); // Se obtienen los grados de salida de los nodos del grafo
    size_t max_grado = grados_salida[0].second; // Se obtiene el grado de salida mas alto
    for (auto& grados : grados_salida) // Se guardan los ips con mayor grado de salida en una lista
    {
        grados_out << "IP: " << grados.first->get_ip_str() << " Grado Salida: " << grados.second << endl; // Se imprime el grado de salida
        if (grados.second == max_grado) top_degree_ips.push_back(grados.first); // Se agrega el ip a la lista si es el mas alto
    }
    grados_out.close(); // Se cierra el archivo

    std::ofstream mayores_grados_out("mayores_grados_ips.txt"); // Se crea un archivo para guardar los grados de salida
    for (int i = 0; i < 5; ++i) // Se imprimen los 5 ips con mayor grado de salida
    {
        mayores_grados_out << "IP: " << grados_salida[i].first->get_ip_str() << " Grado Salida: " << grados_salida[i].second << endl; // Se imprime el grado de salida
    }
    mayores_grados_out.close(); // Se cierra el archivo

    auto date_comp = [](const Registro& r1, const Registro& r2) { return r1.date().get_timestamp() > r2.date().get_timestamp(); };
    Heap<Registro, decltype(date_comp)> heap(registros_i);

    bool exit = false; // Variable para saber si se ha encontrado el bot master
    Registro bot_master; // Registro del bot master
    auto registros_i_ordered = heap.heap_sort(); // Se ordenan los registros por fecha
    for (auto& registro : registros_i_ordered) // Se busca el bot master
    {
        for (auto& top_degree_ip : top_degree_ips) // Se busca el ip con mayor grado de salida
        {
            if (*top_degree_ip == *registro) // Si el registro es igual al ip con mayor grado de salida
            {
                bot_master = *registro; // Se guarda el registro del bot master
                exit = true; // Se marca que se ha encontrado el bot master
                break; // Se sale del bucle
            }
        }
        if (exit) break; // Si se ha encontrado el bot master sale del bucle
    }

    cout << "Bot Master: " << bot_master.get_ip_str() << endl; // Se imprime el ip del bot master
    cout << "Fecha de primera conexion: " << bot_master.get_date() << endl; // Se imprime la fecha de primera conexion

    auto paths_bot_master = registros_graph.dijkstra(bot_master); // Se calculan las rutas del bot master

    Heap<ALDWGraph<Registro>::Dijkstra_path_info, std::less<>> heap_paths(paths_bot_master); // Se crea un heap con los paths del bot master
    auto paths_bot_master_ordered = heap_paths.heap_sort(); // Se ordenan los paths del bot master por distancia

    std::ofstream distancia_out("distancia_botmaster.txt"); // Se crea un archivo para guardar las distancias
    for (auto& path : paths_bot_master_ordered) // Se imprimen las distancias
    {
        distancia_out << "IP: " << path->to->get_ip_str() << " Distancia: " << path->distance << endl; // Se imprime la distancia
    }
    distancia_out.close(); // Se cierra el archivo

    Registro ip_harder = *(paths_bot_master_ordered[0]->to); // Se guarda el ip de mayor esfuerzo desde Bot Master
    cout << "IP de mayor esfuerzo desde Bot Master: " << ip_harder.get_ip_str() << endl; // Se imprime el ip de mayor esfuerzo desde Bot Master

    std::ofstream bot_master_attack("ataque_botmaster.txt"); // Se crea un archivo para guardar el ataque del bot master
    auto atack_path = paths_bot_master_ordered[0]->path; // Se guarda la ruta del bot master
    bot_master_attack << paths_bot_master_ordered[0]->from->get_ip_str() << endl; // Se imprime el ip del bot master
    for (auto& registro : atack_path) // Se imprimen los ips del ataque
    {
        bot_master_attack << registro.first->get_ip_str() << endl; // Se imprime el ip del registro
    }
    bot_master_attack.close(); // Se cierra el archivo

    return 0; // Retorna 0
}