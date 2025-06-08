#ifndef HEAP_H
#define HEAP_H

#include <vector> // Librería para usar el contenedor vector
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <functional>

using std::vector;
using std::swap;
using std::cout;
using std::endl;
using std::greater;

template<class T, typename CMP_FUN = greater<T>> //  implementa un heap genérico (min-heap o max-heap)
class Heap
{
public:
    Heap(); // Constructor por defecto
    explicit Heap(const size_t& capacity); // Constructor con la capacidad inicial y el tipo de heap
    Heap(const Heap& other); // Constructor de copia
    Heap(Heap&& other) noexcept;  // Constructor de movimiento
    explicit Heap(const vector<T>& elements);  // Constructor que se construye un heap desde  un vector existente

    struct Element_amount  // Esta es un etructura auxiliar que contara los elementos repetidos
    {
        T* element;  // apunta al elemento
        size_t amount; // Esto indica en el numero de veces que se repite

        Element_amount(T* element_, const size_t amount_) : element(element_), amount(amount_) // Constructor
        {
            if (!element) throw std::invalid_argument("Heap::Element_amount no puede ser null"); // // Verifica que el elemento no sea nulo
        }
        // Sobrecarga de operadores para comparar
        bool operator<(const Element_amount& other) const // Compara la cantidad de elementos
        {
            return amount < other.amount;  // Devuelve el valor true solo si la cantidad es menor
        }

        bool operator>(const Element_amount& other) const // Compara la cantidad de elementos
        {
            return amount > other.amount; // Devuelve el valor true solo si la cantidad es mayor
        }

        bool operator==(const Element_amount& other) const  // Compara la cantidad de elementos
        {
            return amount == other.amount; // Devuelve  el valor de true si la cantidad de ambos son  iguales
        }
    };

    /**
     *
     * @param capacity capacidad del heap, @code insert()@endcode lanza un warning si se agrega un elemento
     * cuando la capacidad establecida es alcanzada pero no detiene la ejecución
     */
    void set_capacity(const size_t& capacity);

    void push(const T& element); // Inserta un nuevo elemento al heap
    T pop(); // Extrae y devuelve el elemento de la raiz
    [[nodiscard]] const T& peek() const;
    // const T& peek_max() const; //  Devuelve una referencia constante al elemento de la raiz
    // const T& peek_min() const; // Devuelve la referencia que es constante del elemento de la raiz
    template<typename F>
    const T& find_min_if(F fun) const; // Encuentra el minimo que va cumpliendo con una condicion
    [[nodiscard]] bool empty() const;  // Retorna true si el heap esta vacio
    [[nodiscard]] size_t get_size() const; // Devuelve el tamaño actual del heap
    [[nodiscard]] size_t get_capacity() const; // Devuelve la capacidad maxima del heap
    template<typename F>
    [[nodiscard]] vector<Element_amount> count_similar(F fun) const; // Cuenta los elementos que son similares
    [[nodiscard]] vector<Element_amount> count_similar() const;

    vector<T*>& heap_sort(); // Ordenamiento de Heap Sort

    const vector<T>& get_data() const; // Esto devuelve los datos del heap

    ~Heap(); // Destructor

private:
    static size_t parent(const size_t& index); // Devuelve el índice del nodo padre
    static size_t left_child(const size_t& index); // Devuelve el índice del hijo izquierdo
    static size_t right_child(const size_t& index); // Devuelve el índice del hijo derecho

    void heapify_down(const size_t& index); // Reordena hacia abajo desde el índice dado
    void heapify_down_s(const size_t& n, const size_t& index); // Esto va a reordenar hacia abajo en un heap que esta ordenado
    void heapify_up(size_t index); // Reordena hacia arriba desde el índice dado
    void build_heap(); // Aqui se construye el heap

    bool sorted = false;
    size_t capacity; // Capacidad máxima del heap
    size_t size; // Número de elementos actuales en el heap
    vector<T> data; // Vector que almacena los elementos del heap
    vector<T*> data_sorted;
    CMP_FUN comp;
};

// Constructor por defecto. Complejidad: O(1)
template<class T, typename CMP_FUN>
Heap<T, CMP_FUN>::Heap(): capacity(100), size(0) // Aqui se va iniciando la capacidad de 100, tamaño a 0
{
    data.reserve(capacity); // Se hace la reserva de la memoria en el vector
    data_sorted.reserve(capacity); // Aqui se esta resrvando la memoria para vector auxiliar de datos ordenados
}

// Constructor con capacidad y tipo de heap. Complegidad O(1)
template<class T, typename CMP_FUN>
Heap<T, CMP_FUN>::Heap(const size_t& capacity): capacity(capacity), size(0) // Inicia la capacidad en tamaño a 0
{
    data.reserve(this->capacity); // Reserva espacio en el vector de datos
    data_sorted.reserve(this->capacity); // Se reserva un espacio en el vector de los datos ordenados
}

// Constructor de copia. Complejidad O(1)
template<class T, typename CMP_FUN>
Heap<T, CMP_FUN>::Heap(const Heap& other)
    : sorted(other.sorted), capacity(other.capacity), size(other.size), data(other.data),
      data_sorted(other.data_sorted) {} // Aqui hay una copia de todos los atributos que hay del otro heap

// Constructor de movimiento. Complejidad O(1)
template<class T, typename CMP_FUN>
Heap<T, CMP_FUN>::Heap(Heap&& other) noexcept
        : sorted(other.sorted), capacity(other.capacity),
          size(other.size), data(std::move(other.data)), data_sorted(std::move(other.data_sorted))
{
    other.capacity = 0; // El otro heap queda en estado vacío para evitar doble liberación
    other.size = 0;
}

// Cosntructor que recibe un vector para construir el heap. Complejidad O(n)
template<class T, typename CMP_FUN>
Heap<T, CMP_FUN>::Heap(const vector<T>& elements)
    : capacity(elements.size()), size(elements.size()), data(elements)
{
    if (!elements.empty()) build_heap(); // Si no se encuentra vacio se construye el heap
    data_sorted.reserve(elements.size()); // Se reserva el espacio para los datos ordenados
}

// Este es el metodo que establece la capacidad que mas alto en un heap. Complejidad O(1)
template<class T, typename CMP_FUN>
void Heap<T, CMP_FUN>::set_capacity(const size_t& capacity)
{
    this->capacity = capacity; // Aqui se actualiza la capacidad
    data.reserve(this->capacity); // Aqui se reserva el espacio que hay necesario para el vector de los datos
    data_sorted.reserve(this->capacity); // Aqui se reserva el espacio en el vector auxiliar
}

template<class T, typename CMP_FUN> // Complejidad O(log n)
void Heap<T, CMP_FUN>::push(const T& element) // Se inserta un nuevo elemento en el heap
{
    sorted = false;
    data_sorted.clear();
    if (size >= capacity) // Verificamos si alcazamos el maximo del heap
    {
        cout << "Capacidad maxima alcanzada, reasignacion del vector en memoria" << endl; /// Mensaje de advertencia
        capacity += 100; // Aumenta la capacidad en 100
        data.reserve(capacity); // Se reserva un nuevo espacio
        data_sorted.reserve(capacity);
    }

    data.emplace_back(element); // Se agrega el nuevo elemento al terminar el vector
    size++; // Aumento del tamaño en el heap nuevo
    heapify_up(size - 1); // Aqui se esta haciendo un reajuste en el heap
    // Para poder conservar la propiedad de heap
}

template<class T, typename CMP_FUN> // Aqui se extrae el elemento de la raiz. Complejidad O(log n)
T Heap<T, CMP_FUN>::pop()
{
    sorted = false;
    data_sorted.clear();
    if (size == 0) throw std::underflow_error("Heap esta vacia");  // Envia un mensaje de error si esta vacio
    T top = data[0];  // Aqui se guarda el elemento de la raiz ya  que se devuelve
    swap(data[0], data.back());  // El ultimo elemnto se Intercambia con la raiz
    data.pop_back(); // Aqui se elimina en el ultimo elemento
    size--; // Decrementa tamaño
    if (size > 0) heapify_down(0);
    return top; // Aqui esta regresando el elemnto extraido
}

template <class T, typename CMP_FUN>
const T& Heap<T, CMP_FUN>::peek() const
{
    if (empty()) throw std::out_of_range("Heap::peek::Heap vacio"); // Aqui verifica que no este vacio
    return data[0]; // Aqui regresa el elemento de la raiz
}

template<class T, typename CMP_FUN> // Encuentra el minimo solo si cumple la funcion. Complejidad O(n)
template <typename F>
const T& Heap<T, CMP_FUN>::find_min_if(F fun) const
{
    if (empty()) throw std::out_of_range("Heap vacío"); // Aqui verifica que no este vacio

    const T* min_ptr = nullptr; // Puntero minimo el cual cumple la condicion

    // Solo buscamos entre las hojas: desde size / 2 hasta size - 1
    for (size_t i = size / 2; i < size; ++i)
    {
        if (fun(data[i])) { // Si cumple con la condicion
            if (!min_ptr || data[i] < *min_ptr) {
                min_ptr = &data[i]; // // Asignado como un nuevo minimo
            }
        }
    }

    // Buscamos en los padres si no se encontró en las hojas
    if (!min_ptr)
    {
        for (size_t i = 0; i < size / 2; ++i)
        {
            if (fun(data[i])) {
                if (!min_ptr || data[i] < *min_ptr) {
                    min_ptr = &data[i];
                }
            }
        }
    }

    if (!min_ptr)
        throw std::invalid_argument("Ningún elemento cumple la condición"); // Envia mensaje si no cumple con la condcion

    return *min_ptr; // Devolvera la referencia al minimo si cumple con la condicion
}

template<class T, typename CMP_FUN> // Devuelve si el heap está vacio. Complejidad O(1)
bool Heap<T, CMP_FUN>::empty() const
{
    return size == 0; // Aqui es verdadero si el tamaño es 0
}

template<class T, typename CMP_FUN>
size_t Heap<T, CMP_FUN>::get_size() const // Devuelve el tamaño actual del heap. Complejidad O(1)
{
    return size; // Regresao retorna la cantidad de elementos que hay actualizados
}

template<class T, typename CMP_FUN>
size_t Heap<T, CMP_FUN>::get_capacity() const // Devuelve la capacidad maxima del heap. Complejidad O(1)
{
    return capacity; // Aqui se retorna la capacidad maxima que esta establecida
}

template<class T, typename CMP_FUN> // Cuenta cuantos elementos hay y son similares en el heap ordenado. Complejidad O(n)
template<typename F>
vector<typename Heap<T, CMP_FUN>::Element_amount> Heap<T, CMP_FUN>::count_similar(F fun) const
{ // count similar elementos similares
    vector<Element_amount> resultado; // aqui se estan guardando los elemneto repetidos

    if (empty()) throw std::underflow_error("El heap esta vacio"); // revisa que no este vacio
    if (!sorted) throw std::logic_error("Debe llamarse heap_sort() antes de count_similar()");
    if (empty()) return resultado;

    T* actual = data_sorted[0]; // Dirije y apunta al primer elemento
    size_t contador = 1; // Se incia en el contador en 1

    for (size_t i = 1; i < data_sorted.size(); i++) // Se recorre todos los elementos ordenados
    {
        if (fun(*data_sorted[i], *actual)) // Aqui si el elemento es igual al anterior
        {
            contador ++; // Esta incrementando el contador
        }
        else
        {
            resultado.emplace_back(Element_amount(actual, contador)); // Se le agrega un  elemento con el numero de repetidos
            actual = data_sorted[i]; // Aqui se estra actualizando hacia el nuevo
            contador = 1; // se vuelve a iniciar  el contador
        }
    } resultado.emplace_back(Element_amount(actual, contador));
    return resultado; // Regresa el vector de los elementos y cantidades que sean repetidas
}

template <class T, typename CMP_FUN>
vector<typename Heap<T, CMP_FUN>::Element_amount> Heap<T, CMP_FUN>::count_similar() const
{
    return count_similar(std::equal_to<T>());
}

template<class T, typename CMP_FUN> // Destructor por defecto
Heap<T, CMP_FUN>::~Heap()
= default;

template<class T, typename CMP_FUN> // devuelve el indice al nodo del padre. Complejidad O(1)
size_t Heap<T, CMP_FUN>::parent(const size_t& index)
{
    if (index == 0) return 0; // La raiz en este caso no tiene padre y regresa un  0
    return (index - 1) / 2; // Se calcula el indice padre en el heap binario
}

template<class T, typename CMP_FUN>
size_t Heap<T, CMP_FUN>::left_child(const size_t& index) // devuelve el indice del hijo izquierdo para un nodo. Complejidad O(1)
{
    return index * 2 + 1; // Esta es la formula para el hijo izquierdo
}

template<class T, typename CMP_FUN> // Regresa el indice del hijo derecho. Complejidad O(1)
size_t Heap<T, CMP_FUN>::right_child(const size_t& index)
{
    return index * 2 + 2; // Calculo para el hijo derecho
}

// Se reorganiza el heap para mantener la propiedad desde un indice. Complejidad O(log n)
template<class T, typename CMP_FUN>
void Heap<T, CMP_FUN>::heapify_down(const size_t& index)
{
    size_t current = index; // Aqui el indice esta reparando

    while (true)
    {
        size_t left = left_child(current);  //  hijo izquierdo del indice
        size_t right = right_child(current);//  hijo derecho del indice
        size_t selected = current; // El nodo seleccionado actualizado

        // Dependiendo si es Max o Min Heap se evaluara la conidcion
        if (left < size && comp(data[left], data[selected])) selected = left;
        if (right < size && comp(data[right], data[selected])) selected = right;

        // Se encontro un hijo que puede que  rompa la propiedad del heap
        if (selected != current) {
            swap(data[current], data[selected]); // Se hace un intercambio con el hijo mayor (o menor)
            current = selected; // el puntero  baja de la posicion
        } else break; // Si no hay interrupcion entonces se termina
    }
}

template<class T, typename CMP_FUN>
void Heap<T, CMP_FUN>::heapify_down_s(const size_t& n, const size_t& index) // para vector de punteros ordenados. Complejidad O(n)
{
    size_t current = index;

    while (true)
    {
        size_t left = left_child(current);
        size_t right = right_child(current);
        size_t selected = current;

        if (left < n && comp(*data_sorted[left], *data_sorted[selected])) selected = left;
        if (right < n && comp(*data_sorted[right], *data_sorted[selected])) selected = right;

        if (selected != current)
        {
            swap(data_sorted[current], data_sorted[selected]);
            current = selected;
        } else break;
    }
}

template<class T, typename CMP_FUN> // Se reorganiza el heap hacia arriba. Complejidad O(log n)
void Heap<T, CMP_FUN>::heapify_up(size_t index)
{
    while (index > 0) // Se obtiene el indice del padre
    {
        size_t par = parent(index);

        if (comp(data[index], data[par])) { // Compara el hijo con padre con base a la funcion de comparacion
            swap(data[index], data[par]);// si se rompe, se hace el intercambio
            index = par; // Sube al padre y comienza a repetir
        } else break; // Si la propiedad salio bien entonces se acaba
        }
}

// Construye el heap desde datos arbitrarios. Complejidad O(n)
template<class T, typename CMP_FUN>
void Heap<T, CMP_FUN>::build_heap()
{
    size_t i = size / 2 - 1; // Inicia desde el ultimo nodo padre

    while (true)
    {
        heapify_down(i); // Se usa el heapify_down para el nodo actual
        if (i == 0) break; // Si llego a la raiz entonces se termina
        i--;
    }
}

template<class T, typename CMP_FUN> // Complejidad O(n log n)
vector<T*>& Heap<T, CMP_FUN>::heap_sort()
{
    if (empty()) throw std::underflow_error("El heap esta vacio");
    if (sorted) return data_sorted; // Si ya esta ordenado regresa el vector de punteros ordenados
    sorted = true; // Se mar ca elheap como ordenado
    data_sorted.clear(); // Se va a hacer una limpia en el vector auxiliar

    for (size_t i = 0; i < size; i++) // Se llena el vector con los punteros y elementos del heap
    {
        data_sorted.emplace_back(&data[i]);
    }

    size_t j = size / 2 - 1; // Se empieza por el ultimo  nodo padre

    while (true)
    {
        heapify_down_s(size, j); // Se usa el  heapify_down para el vector de punteros
        if (j == 0) break; // aqui finaliza cuando se llega a la raiz
        j--;
    }

    // Se ordenan los punterosal usar el heap sort
    for (size_t i = size - 1; i > 0; --i)
    {
        swap(data_sorted[0], data_sorted[i]); // Se hace el intercambio de  punteros del primero y ultimo
        heapify_down_s(i, 0); // Aplica el heapify_down para el primer elemento que tiene un tamaño que esta reducido
    }

    return data_sorted; // Se retorna el vector con los punteros para los datos que estan ordenados
}

template<class T, typename CMP_FUN> // Se devuelve la referencia constante del vector. Complejidad O(1)
const vector<T>& Heap<T, CMP_FUN>::get_data() const
{
    return data; // Aqui se retorna el vector de los datos
}


#endif //HEAP_H
