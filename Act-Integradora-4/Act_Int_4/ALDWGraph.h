#ifndef ALDWGRAPH_H
#define ALDWGRAPH_H

#include <iostream> // entra y salida de la consola
#include <ostream> // sobrecarga de operadores con flujos
#include <vector> // Almacena los nodos si se usa el vector dinamico
#include <algorithm> // Este sirve para
#include <limits> // Esta libreria ayuda a acceder el valor maximo
#include <unordered_map> // acceso rapido y eficasz a los nodos y vecinos del mapa hash
#include <list> // Esta es un libreria para lo nodos de doble enlazo
#include "Heap.h" // El heap personalizado que se ha creado

using std::vector;
using std::swap;
using std::cout;
using std::endl;
using std::unordered_map;
using std::pair;
using std::list;
using std::string;
using std::ostream;

template<class T>
class ALDWGraph
{
public:
    struct Dijkstra_path_info
    { // Estructura que almacena la información de un camino encontrado por Dijkstra
        const T* from; // El nodo de origen
        const T* to; // El nodo de destino
        vector<pair<const T*, size_t>> path; // La ruta que se tomo para llegar al destino
        size_t distance; // La distancia que se tomo para llegar al destino

        Dijkstra_path_info(): from(nullptr), to(nullptr), distance(0) {} // Constructor por defecto
        Dijkstra_path_info(const T* node_, const T* to_, vector<pair<const T*, size_t>> path_, const size_t& distance_) : from(node_), to(to_), path(path_), distance(distance_) {}
        Dijkstra_path_info(const T* node_, const T* to_) : from(node_), to(to_), distance(0) {}

        const vector<pair<const T*, size_t>>& add_path_step(const pair<const T*, size_t>& step)
        { // Aqui nos permite sumar pasos en el camino de los nodos
            path.push_back(step); // Agrega el paso al camino
            return path; // regresa el vector de pasos
        }

        void set_distance(const size_t& distance_) // Metodo para actualizar la distancia total del camino
        {
            distance = distance_;
        }

        template <typename F>
        void print_path(ostream& stream, F fun) const  // Impirme con un formateador de nodos
        {
            stream << "From: " << fun(*from) << " to: " << fun(*to) << " distance: " << distance << endl;
            stream << "Path: " << fun(*from);
            for (const auto& step : path)
            {
                stream << " -> " <<fun(*step.first) << " (" << step.second << ")";
            }
            stream << '\n';
        }
        // Operadores de comparacion
        bool operator==(const Dijkstra_path_info& other) const
        {
            return distance == other.distance;
        }

        bool operator<(const Dijkstra_path_info& other) const
        {
            return distance < other.distance;
        }

        bool operator>(const Dijkstra_path_info& other) const
        {
            return distance > other.distance;
        }

        bool operator<=(const Dijkstra_path_info& other) const
        {
            return distance <= other.distance;
        }

        bool operator>=(const Dijkstra_path_info& other) const //
        {
            return distance >= other.distance;
        }

        bool operator!=(const Dijkstra_path_info& other) const
        {
            return distance != other.distance; //
        }
    };

    ALDWGraph();
    explicit ALDWGraph(const size_t& capacity); // Constructor con capacidad inicial
    explicit ALDWGraph(const vector<T>& nodes); // Crea un grafo con los nodos dados en un vector
    explicit ALDWGraph(const list<T>& nodes);
    // ALDWGraph(const ALDWGraph& otheWr);
    // ALDWGraph(ALDWGraph&& other) noexcept;
    bool add_node(const T& node); // Devuelve verdadero si se agrego el nodo
    bool add_edge(const T& from, const T& to, const size_t& weight); // Devuelve verdadero si se agrego la arista
    bool has_edge(const T& from, const T& to) const; // Devuelve verdadero si existe la arista
    bool remove_node(const T& node); // Devuelve verdadero si se elimino el nodo
    bool remove_edge(const T& from, const T& to); // Devuelve verdadero si se elimino la arista
    [[nodiscard]] size_t out_degree(const T& node) const; // Devuelve el numero de vecinos del nodo
    vector<pair<const T*, size_t>> out_degrees(bool sorted = false, bool descent = false) const;
    vector<T*> get_neighbors(const T& node); // Devuelve los vecinos del nodo en forma normal
    vector<const T*> get_neighbors(const T& node) const; // Devuelve los vecinos del nodo en forma constante
    template <typename F> // Imprime el grafo
    void print_graph(F fun) const;
    [[nodiscard]] size_t node_count () const; // El numero de nodos actuales en el grafo
    [[nodiscard]] size_t edge_count () const; // devuelve la cantidad de aristas actuales en el grafo
    bool contains_node(const T& node) const; // regresa true si el nodo existe en el grafo
    vector<const T*> get_nodes() const; // Retorna un vector de punteros constantes a todos los nodos del grafo
    void clear(); // grafo vacio

    vector<Dijkstra_path_info> dijkstra(const T& start); // retorna los caminos más cortos a todos los nodos

    ~ALDWGraph(); // Libera cualquier recurso asociado al grafo

private:
    bool use_vector = false; // Esta aqui indicando si se guardan los nodos en un vector o puede que en una lista
    size_t reserved_capacity = 100; // Es la capacidad que tiene al iniciar para los vectores de nodos
    size_t nodes_amount = 0; // cantidad de grafos en el nodo
    size_t edges_amount = 0; // cantidad de conexiones en el grafo
    vector<T> nodes_vector; // guarda los nodos se se utiliza un vector
    list<T> node_list; // contendeor si se utilizan las listas
    unordered_map<T, T*> node_ptr_map; // mapa que accede a los punteros de nodos por medio de su valor
    unordered_map<T, vector<pair<T*, size_t>>> adj_list; // cada nodo va a guardar ub vector de pares
};
// Complejidad 0(1)
template <class T> // Constructor por defecto
ALDWGraph<T>::ALDWGraph() = default; // Se crea un grafo sin nodos ni aristas
// Complejidad 0(1)
template <class T> // Va a obtener una capacidad nicial
ALDWGraph<T>::ALDWGraph( const size_t& capacity): reserved_capacity(capacity)
{
    use_vector = true; // Se usa el vector para guardar los nodos
    nodes_vector.reserve(reserved_capacity); // Se reserva la capacidad que se inicio
}
// Complejidad 0(n)
template <class T>
ALDWGraph<T>::ALDWGraph(const vector<T>& nodes)
{
    reserved_capacity = nodes.size(); // Se conserva lo exacto que se usara
    use_vector = true;// Se indica que se va a utiizar el contendor del vector especifico
    nodes_vector.reserve(reserved_capacity); // se reserva la memoria en el vector
    nodes_vector = nodes; // se hace una copia de los nodos del vector interior
    nodes_amount = nodes.size(); // se guardan los nodos que exsiten
    for (auto& node : nodes_vector) // se hace el recorrido de los nodos guardados
    {
        node_ptr_map[node] = &node; // Se guarda la direccion de cada uno de los nodos
    }
}
//  Complejidad 0 (n)
template <class T>
ALDWGraph<T>::ALDWGraph(const list<T>& nodes)
{
    node_list = nodes;
    nodes_amount = nodes.size();
    for (auto& node : node_list)
    {
        node_ptr_map[node] = &node; // Se almaceban las direcciones desde la list
    }
}
// Complejidad 0(1)
template <class T>
bool ALDWGraph<T>::add_node(const T& node)
{
    if (node_ptr_map.count(node)) return false; // Si el nodo ya existe

    if (use_vector)
    {
        if (nodes_vector.size() >= reserved_capacity) return false; // si no hay espacio
        nodes_vector.push_back(node); // añade al vector
        node_ptr_map[node] = &nodes_vector.back(); // guarda la direccion del nodo que se añadio
    } else
    {
        node_list.push_back(node); // se sunma al final de la lista
        node_ptr_map[node] = &node_list.back(); // almacena la direccion en el nodo que se añadio
    }

    ++nodes_amount; // va aumentando el contador de nodos
    return true; //nodo añadido de forma correcta
}
// Complejidad 0(1)
template <class T>
bool ALDWGraph<T>::add_edge(const T& from, const T& to, const size_t& weight)
{
    if (!node_ptr_map.count(from) || !node_ptr_map.count(to)) return false;
    // aqui se verifica si ambos nodos estan presentes en el grafo
    T* to_ptr = node_ptr_map[to]; // Se adquiere el puntero al nodo destino
    auto& neighbors = adj_list[from]; // Puede acceder a la lsita de los vecinos del nodo inicio

    for (auto& neighbor : neighbors) // revisa su la arista ya esta presente
    { // k es la cantidad de vecinos que hay en el nodo origen
        if (neighbor.first == to_ptr) return false; // Aqui la arista ya esta presente
    }

    neighbors.push_back({to_ptr, weight}); // Suma la arista a la lista de vecinos
    ++edges_amount; // Aumenta la cantidad de aristas
    return true; // Regresa verdadero cuando la arista se agrego de forma correcta
}

template <class T>  // Complejidad 0(k)
bool ALDWGraph<T>::has_edge(const T& from, const T& to) const
{ // se hace la verificacion si hay algun nodo que exista o ambos y ademas si cuenta con la listya adyacente
    if (!node_ptr_map.count(from) || !node_ptr_map.count(to) || !adj_list.count(from)) return false; // Ambos nodos
    T* to_ptr = node_ptr_map.at(to); // Logra tener el puntero del nodo que sera destino

    const auto& neighbors = adj_list.at(from); // Se obtiene la lista de vecinos del nodo form
    for (const auto& neighbor : neighbors)  // recorre todos los vecinos que indican al nodo to
    {
        if (neighbor.first == to_ptr) return true; // si esta la arista, entonces es verdadero
    }
    return false;
}
// Complejidad  0(1)
template <class T> // Se realiza la verificacion del nodo en caso de que exista en el grafo
size_t ALDWGraph<T>::out_degree(const T& node) const
{ // Si no existe entonce dara un valor de 0
    if (!node_ptr_map.count(node)) return 0; // Si no existe, regresa 0

    auto it = adj_list.find(node); // Busca el nodo en la lista de adyacencia
    if (it == adj_list.end()) return 0; // Si no existe, regresa 0

    return it->second.size(); // Devuelve la cantidad de vecinos que tiene el nodo
}

template <class T>
vector<pair<const T*, size_t>> ALDWGraph<T>::out_degrees(const bool sorted, const bool descent) const
{
    using Pair = pair<const T*, size_t>;
    vector<Pair> result;
    result.reserve(nodes_amount);
    if (use_vector)
    {
        for (auto& node : nodes_vector)
        {
            size_t degree = out_degree(node);
            result.emplace_back(&node, degree);
        }
    } else
    {
        for (auto& node : node_list)
        {
            size_t degree = out_degree(node);
            result.emplace_back(&node, degree);
        }
    }

    if (sorted)
    {
        if (descent)
        {
            auto comp = [](const Pair& a, const Pair& b) { return a.second < b.second; };
            Heap<Pair, decltype(comp)> heap(result);
            auto aux = heap.heap_sort();
            result.clear();
            for (auto& element : aux)
            {
                result.emplace_back(*element);
            }
        }
        else
        {
            auto comp = [](const Pair& a, const Pair& b) { return a.second > b.second; };

            Heap<Pair, decltype(comp)> heap(result);
            auto aux = heap.heap_sort();
            result.clear();
            for (auto& element : aux)
            {
                result.emplace_back(*element);
            }
        }
    }

    return result;
}

// Complejidad O(k)
template <class T> // este metodo va a regresar con los punteros a los vecinos que son del nodo especifico
vector<T*> ALDWGraph<T>::get_neighbors(const T& node)
{
    if (!node_ptr_map.count(node) || !adj_list.count(node)) return {}; // aqui esta evaluando si el nodo existe dentro del grafo
    // y ademas si tiene la lista adyacente
    vector<T*> neighbors; // aqui el vector almacena los vecinos
    const auto& neighbors_list = adj_list.at(node); // Aqui se obtine la lista de vecinos desde el mapa
    for (const auto& neighbor : neighbors_list)
    { // se recorre cada par y despues e extrea del puntero hacia el nodo que es vecino
        neighbors.push_back(neighbor.first); // aqui el vector se le suma al puntero del vector que sale
    }
    return neighbors;
}
// Comlejidad 0 (k)
template <class T>
vector<const T*> ALDWGraph<T>::get_neighbors(const T& node) const
{
    if (!node_ptr_map.count(node) || !adj_list.count(node)) return {}; // Si el nodo no existe, se va a regresar un vector vacio
    //Busqueda de unordered_map
    vector<const T*> neighbors; // aqui el vector almacena los vecinos
    const auto& neighbors_list = adj_list.at(node); // se logra conseguir la lista de vecino del nod
    for (const auto& neighbor : neighbors_list) // se hace el recorrido de la lista adyacencia
    { // k es la cantidad de vecinos de un nodo
        neighbors.push_back(neighbor.first); // va guardando el puntero en cada vecino
    }
    return neighbors; // regresa el vector con los vecinos
}
// Complejidad 0(n+m)
template <class T>
template <typename F>
void ALDWGraph<T>::print_graph(F fun) const
{
    if (use_vector) // los vectores sirven para guardar los nodos
    {
        for (const auto& node : nodes_vector)
        { // Aqui esta imprimiendo el nodo actual de la funcion que se proporciona
            std::cout << fun(node) << " -> ";
            auto it = adj_list.find(node); // Busca el nodo en la lista de adyacencia
            if (it != adj_list.end())
            { // Imprimer los vecinos y ademas los recorre con su peso
                for (const auto& neighbor : it->second)
                {
                    std::cout << fun(*neighbor.first) << " (" << neighbor.second << "); ";
                } //imprime el vecino con su peso
            } // un salto de linea para pasar al futuro nodo
            std::cout << '\n';
        }
    }
    else
    {
        for (const auto& node : node_list)
        { // Se realiza el mismo proceso pero solo que con listas enlazadas a comparacion de un vector
            std::cout << fun(node) << " -> ";
            auto it = adj_list.find(node); // Busca el nodo en la lista de adyacencia (hash map)
            if (it != adj_list.end()) //solo si el nodo tiene vecinos registrados
            {
                for (const auto& neighbor : it->second) // Se va iterando de cada vecino del nodo
                {
                    std::cout << fun(*neighbor.first) << " (" << neighbor.second << "); ";
                } // Aqui imprime la IP del vecino y el peso que es de la arista
            }
            std::cout << '\n'; // otro salto para la vizualizacion de cada nodo
        }
    }
}

// Complejidad O(1)
template <class T> // Destructor del grafo
ALDWGraph<T>::~ALDWGraph() = default; // Se elimina el grafo

// Complejidad O(1)
template <class T> // regresa el numero de totales de nodos que estan guardados en el grafo
size_t ALDWGraph<T>::node_count() const {
    return nodes_amount;
}

// Complejidad O(1)
template <class T> // Regresa la cantidad que hay de aristas en el grafo
size_t ALDWGraph<T>::edge_count() const {
    return edges_amount;
}

// Complejidad O(1)
template <class T> // Regresa verdadero si el nodo existe en el grafo
bool ALDWGraph<T>::contains_node(const T& node) const { // realiza la verificacion por medio de estas funciones
    return node_ptr_map.contains(node);
}

// Complejidad O(n)
template <class T>
vector<const T*> ALDWGraph<T>::get_nodes() const
{
    vector<const T*> resultado; // El vector esta para almacenar los punteros constantes de los nodos
    if (use_vector){ // Si se esta usando un vector como un contenedor principal de los nodos
        for (const auto& nodo : nodes_vector){
            resultado.push_back(&nodo); // Se le agrega la direccion a  cada uno de los nodo
        }
    } else
        { // Lista se implementa como un contenedor principal de nodos
        for (const auto& nodo : node_list){
            resultado.push_back(&nodo); // Agrega la direccion de cada nodo
        }
    }
    return resultado; // Devuelve el vector con los punteros a los nodos
}
//complejidad 0 (n +m)
template <class T>
bool ALDWGraph<T>::remove_node(const T& node)
{
    if (!node_ptr_map.count(node)) return false; // Si el nodo no existe, no se elimina

    // Eliminar todas las aristas entrantes hacia este nodo
    for (auto& [key, vec] : adj_list)
    {
        for (auto it = vec.begin(); it != vec.end(); ) {
            if (*it->first == node) { // // Si el nodo actual tiene una arista hacia el nodo que se quiere eliminar
                it = vec.erase(it); } // Se elimina la arista
            else{
                ++it;} // Sigue iterando si no coincide
        }
    }

    // Restar las aristas salientes desde este nodo
    edges_amount -= adj_list[node].size();

    // Eliminar del mapa de adyacencia y de punteraos
    adj_list.erase(node);
    node_ptr_map.erase(node);

    // Eliminar del contenedor correspondiente
    if (use_vector)
    {
        nodes_vector.erase(std::remove(nodes_vector.begin(), nodes_vector.end(), node), nodes_vector.end());
    }
    else
    {
        node_list.remove(node);
    }

    // Disminuir contador de nodos y retornar true
    --nodes_amount;
    return true;
}
// Complejidad O(k)
template <class T>
bool ALDWGraph<T>::remove_edge(const T& from, const T& to)
{ // Se hace la verificacion si ambos nodos existen en el grafo
    if (!node_ptr_map.count(from) || !node_ptr_map.count(to)) return false;

    T* to_ptr = node_ptr_map[to]; // se logra tener el puntero al nodo destino
    auto& vec = adj_list[from];  // esto se refiere a la lista de vecinos del nodo from
    size_t original_size = vec.size(); // Va alamacenando el tamaño inicial para saber si se eliminoalguna arista

    vec.erase(std::remove_if(vec.begin(), vec.end(), [&to_ptr](const pair<T*, size_t>& p) {
        return p.first == to_ptr; // Elimina la arista si existe utilizando estas funciones
    }), vec.end()); // Se compara el puntero con el  nodo destino

    if (vec.size() < original_size) // Si el tamaño del vector cambia o vario esto significa que se borro una arista
    {
        --edges_amount; // Se reduce el contador de aristas
        return true; // Se elimino de forma correcta
    }

    return false; // No se encuentra la arista
}


template <class T>
void ALDWGraph<T>::clear() { // Limpia lo siguiente del grafo
    nodes_vector.clear(); // Se limpia el vector
    node_list.clear(); // Se limpia la lista
    node_ptr_map.clear(); // Se limpia el mapa
    adj_list.clear(); // Se limpia la lista de adyacencia
    nodes_amount = 0; // Se limpia el contador de nodos y aristas
    edges_amount = 0; // Se limpia el contador de nodos y aristas
}

// complejidad O(V + ElogV)
template <class T>
vector<typename ALDWGraph<T>::Dijkstra_path_info> ALDWGraph<T>::dijkstra(const T& start)
{
    using Pair = pair<size_t, T*>; // Se define un par
    vector<Dijkstra_path_info> result; // esto da el resulñtado de los caminos que se hicieron pero cortos

    if (!node_ptr_map.count(start)) return result; // si el nodo que se creo no existe, eso quiere decir que se regresa un vacio

    unordered_map<T*, size_t> distances; // Se guarda la distancia que es corta de cada nodo
    unordered_map<T*, T*> parent; // se almacena el nodo previo que hubo el mas corto
    unordered_map<T*, bool> visited; // va tomando en cuenta que nodos ya fueron procesados

    for (const auto& node : node_ptr_map)
    { // se inicia la distancia de u valor maximo pero sin padre
        distances[node.second] = std::numeric_limits<size_t>::max();
        parent[node.second] = nullptr;
    }
    T* start_ptr = node_ptr_map[start]; // se almacena el puntero del nodo de inicio
    distances[start_ptr] = 0; // la distancia como tal es cero
    // la funcion lambda se usa para la comparacion de la min - heap
    auto comp = [](const Pair& p1, const Pair& p2) { return p1.first < p2.first; };
    Heap<Pair, decltype(comp)> pq(nodes_amount); // Aqui se crea un heap que va a tener la capacidad guardada

    pq.push({0, start_ptr}); // se inserta el nodo de inicio con una distancia de 0

    while (!pq.empty()) // aqui empieza el algoritmo
    {
        auto [dist, node] = pq.pop(); // se extrae el nodo con la menor distancia
        if (visited[node]) continue; // si el nodo ya fue procesado, se continua con el siguiente
        visited[node] = true; // se marca como procesado

        for (auto& [neighbor, weight] : adj_list[*node]) // se empieza a iterar vecinos
        {
            size_t new_dist = dist + weight; // se calcula la distancia del nodo actual al vecino
            if (new_dist < distances[neighbor]) // if si es la mejor ruta en este caso
            {
                distances[neighbor] = new_dist; // se actualiza la distancia
                parent[neighbor] = node; // se actualiza el nodo antes del caminos
                pq.push({new_dist, neighbor}); // se crea la fila de prioridad
            }
        }
    }

    for (const auto& [node, dist] : distances) // se construyeron los caminos desde un nodo inicio a uno final
    {
        if (dist == std::numeric_limits<size_t>::max() || dist == 0) continue;  // si la distancia es maximo, no se guarda

        vector<pair<const T*, size_t>> path; // esta es la lista de pasos que hubo en el camino
        T* current = node;
        T* prev = parent[current];

        while (prev)
        { // retrocede el nodo al original, el cual va construyendo aqui su camino
            size_t weight = 0;
            for (const auto& [n, w] : adj_list[*prev]) // se busca su peso de la arista
            {
                if (n == current)
                {
                    weight = w;
                    break;
                }
            }
            path.insert(path.begin(), {current, weight}); // se inserta al inicio del vector
            current = prev;
            prev = parent[current];
        }

        result.emplace_back(start_ptr, node, path, dist); // se guarda el camino
    }

    return result; // se regresan los caminos que fueron cortos desde que comienza el incio
}

#endif //ALDWGRAPH_H
