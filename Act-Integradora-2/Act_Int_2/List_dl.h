#ifndef LIST_DL_H
#define LIST_DL_H

#include "Iterator_dll.h"
#include "Node_dll.h"

/**
 * This is a template doubly linked list based in the standard STL List.\n
 * For List_dl to work properly the following operators in the 'T' generic class must be overloaded:\n
 * == != < > <= >=\n
 * T class must handle it's own destruction correctly.\n
 * T must have:
 * - Default constructor
 * - Param constructor
 * - copy & move constructor
 * - copy & move assignment oeprator
 *
 * Missing implementations for List_dl to work similarly to std::List:
 * - Correct use of noexcept for better performance
 * - A more robust iterator
 * - Overload of some methods to work with iterators for more consistency for the user.
 *
 * List_dl has an Iterator_dll but it's primitive, missing reverse Iterator and other guidelines to be STL compliant
 *
 * @tparam T Generic class
 */
template <class T>
class List_dl
{
public:
    using it = Iterator_dll<T>;  // Define un alias "it" para Iterator_dll<T> para simplificar el uso del iterador

    List_dl();                          // Constructor por defecto crea una lista vacía
    List_dl(const List_dl& other);      // Constructor de copia crea una nueva lista copiando otra existente
    List_dl(List_dl&& other) noexcept;  // Constructor de movimiento transfiere recursos de otra lista; no lanza excepciones

    template <typename U> //Metodo para agregar un elemento al inicio de la lista usando perfect forwarding
    void add_first(U&& val);
    template <typename U>
    void add_last(U&& val); // Metodo para agregar un elemento al final de la lista usando perfect forwarding
    template <typename U>
    void insert(const size_t& pos, U&& val); //  Metodo para insertar un elemento en la posición pos utilizando perfect forwarding
    void update_data(const T& val, const T& newValue); // Actualiza la data de la primer ocurrencia de un valor dado
    void update_at(const size_t& pos, const T& newValue); //Actualiza la data en una posición específica

    List_dl& invert_list();
    List_dl& swap(const size_t& i, const size_t& j);
    List_dl& swap_k(const size_t& k);
    List_dl& merge_sort(bool desc);

    void delete_at(const size_t& pos);  // Elimina el nodo en la posición pos
    void delete_data(const T& val);     // Elimina el primer nodo que contiene el valor dado
    int delete_all(const T& val);       // Elimina todos los nodos que contienen el valor dado y retorna la cantidad de eliminaciones

    [[nodiscard]] size_t get_size() const noexcept;
    [[nodiscard]] bool is_empty() const noexcept;
    [[nodiscard]] T& get_data(const size_t& pos);
    [[nodiscard]] const T& get_data(const size_t& pos) const;
    [[nodiscard]] it find_data(const T& val) const;
    template <typename F>
    [[nodiscard]] it search_data(it start, it end, const T& k, F fun);
    // Modificar si la lista esta ordenada descendentemente
    template <typename F>
    [[nodiscard]] List_dl find_interval(const T& start, const T& end, F fun);
    [[nodiscard]] const T& get_mid() const;

    T& operator[](const size_t& pos);
    const T& operator[](const size_t& pos) const;
    List_dl& operator=(const List_dl& other);
    List_dl& operator=(List_dl&& other) noexcept;

    it begin();   // Retorna un iterador al primer elemento de la lista
    it end();     // Retorna un iterador que representa el final de la lista (nodo dummy)
    it cbegin() const; // Retorna un iterador constante al primer elemento
    it cend() const;   // Retorna un iterador constante que representa el final de la lista
    it rbegin();        // Retorna un iterador inverso al último elemento de la lista
    it crbegin() const; // Retorna un iterador constante inverso al último elemento

    ~List_dl();  // Destructor: libera todos los recursos asignados y limpia la lista


private:
    void list_is_valid() const;  // Verifica si la lista es válida (no está vacía); usado internamente
    void clear();                // Elimina todos los nodos de la lista y reinicia punteros y tamaño
    void delete_node(DLLNode<T>* node); // Elimina un nodo específico y actualiza los enlaces de la lista
    void copy_list(const List_dl& other); // Copia todos los nodos de otra lista en la actual
    void mid_node();             // Determina y establece el nodo central (mid) de la lista

    //
    DLLNode<T>* split(DLLNode<T>* start, size_t n);
    template <typename F>
    std::pair<DLLNode<T>*, DLLNode<T>*> merge(DLLNode<T>* a, DLLNode<T>* b, F cmp);
    //https://www.geeksforgeeks.org/merge-sort/?utm_source=chatgpt.com
    //https://www.geeksforgeeks.org/iterative-merge-sort/?utm_source=chatgpt.com

    [[nodiscard]] DLLNode<T>* get_dir_at(const size_t& pos);
    [[nodiscard]] DLLNode<T>* get_dir_of(const T& val);
    // (Agregar si la lista esta ordenada recorrer desde head o tail dependiendo si val > mid element)
    [[nodiscard]] DLLNode<T>* find_mid_in(DLLNode<T>* start, const DLLNode<T>* end);
    template <typename F>
    [[nodiscard]] DLLNode<T>* binary_search(DLLNode<T>* start, DLLNode<T>* end, const T& k, F fun);
    // Modificar si la lista esta ordenada descendentemente

    bool is_sorted = false;
    bool direction = false;
    DLLNode<T>* dummy_node_ = new DLLNode<T>{};
    size_t size;
    DLLNode<T>* head;
    DLLNode<T>* tail;
    DLLNode<T>* mid = new DLLNode<T>{};
};

template <class T>
List_dl<T>::List_dl(): size(0), head(dummy_node_), tail(dummy_node_)
{
    dummy_node_->next = dummy_node_; // El siguiente del dummy apunta a sí mismo para representar una lista vacía
    dummy_node_->prev = dummy_node_; // El anterior del dummy apunta a sí mismo
}


template <class T>
List_dl<T>::List_dl(const List_dl& other): size(0), head(dummy_node_), tail(dummy_node_)
{
    dummy_node_->next = dummy_node_; // Inicializa el dummy_node_ de la lista copiada
    dummy_node_->prev = dummy_node_;

    if (other.size == 0) return;      // Si la otra lista está vacía, no se realiza copia

    copy_list(other);               // Copia cada nodo de other en la lista actual
}

template <class T>
List_dl<T>::List_dl(List_dl&& other) noexcept: size(other.size), head{other.head}, tail{other.tail}
{

    if (other.size == 0)
    {
        head = tail = dummy_node_;
    }
    else
    {
        head->prev = dummy_node_;     // Ajusta el puntero previo del head para que apunte al dummy_node_
        tail->next = dummy_node_;     // Ajusta el puntero siguiente del tail para que apunte al dummy_node_
    }

    dummy_node_->next = head; // El dummy_node_ apunta a head como inicio de la lista
    dummy_node_->prev = tail; // El dummy_node_ apunta a tail como final de la lista

    other.head = other.tail = other.dummy_node_;
    other.dummy_node_->next = other.dummy_node_;
    other.dummy_node_->prev = other.dummy_node_;
    other.size = 0;
}

template <class T>
template <typename U>
void List_dl<T>::add_first(U&& val)
{
    auto* new_node = new DLLNode<T>{std::forward<U>(val)}; // Crea un nuevo nodo con el valor dado usando perfect forwarding

    new_node->prev = dummy_node_;    // El nodo nuevo apunta hacia atrás al nodo dummy
    new_node->next = head;           // El nodo nuevo apunta hacia adelante al actual head

    head->prev = new_node;           // El antiguo head actualiza su puntero previo para apuntar al nodo nuevo

    if (head == dummy_node_)         // Si la lista estaba vacía, se actualiza tail también
    {
        tail = new_node;
    }

    dummy_node_->next = new_node;    // El puntero siguiente del dummy se actualiza para apuntar al nodo nuevo (nuevo head)
    head = new_node;                 // Actualiza head para que apunte al nodo nuevo

    size++;                        // Incrementa el tamaño de la lista
}

template <class T>
template <typename U>
void List_dl<T>::add_last(U&& val)
{
    auto* new_node = new DLLNode<T>{std::forward<U>(val)};

    new_node->prev = dummy_node_->prev;
    new_node->next = dummy_node_;
    dummy_node_->prev->next = new_node;
    dummy_node_->prev = new_node;

    if (size == 0) head = tail = new_node;
    else tail = new_node;

    size++;
}

template <class T>
template <typename U>
void List_dl<T>::insert(const size_t& pos, U&& val)
{
    if (pos == 0)
    {
        add_first(std::forward<U>(val));
        return;
    }
    if (pos == size)
    {
        add_last(std::forward<U>(val));
        return;
    }

    auto* node = get_dir_at(pos);
    auto* new_node = new DLLNode<T>{std::forward<U>(val)};

    new_node->prev = node->prev;
    new_node->next = node;
    node->prev->next = new_node;
    node->prev = new_node;

    size++;
}

template <class T>
void List_dl<T>::update_data(const T& val, const T& newValue)
{
    auto* node = get_dir_of(val);
    if (node == dummy_node_) throw std::out_of_range("Element to update not found");
    node->data = newValue;
}

template <class T>
void List_dl<T>::update_at(const size_t& pos, const T& newValue)
{
    get_dir_at(pos)->data = newValue;
}

template <class T>
List_dl<T>& List_dl<T>::invert_list()
{
    if (head == dummy_node_ || size < 2) return *this;

    auto* current = head;

    while (current != dummy_node_)
    {
        std::swap(current->next, current->prev);
        current = current->prev;
    }
    std::swap(head, tail);
    dummy_node_->next = head;
    dummy_node_->prev = tail;

    return *this;
}

template <class T>
List_dl<T>& List_dl<T>::swap(const size_t& i, const size_t& j)
{
    if (i >= size || j >= size) throw std::out_of_range("Index out of range");
    if (i == j) return *this;
    auto* node1 = get_dir_at(i);
    auto* node2 = get_dir_at(j);
    std::swap(node1->data, node2->data);
    return *this;
}

template <class T>
List_dl<T>& List_dl<T>::swap_k(const size_t& k)
{
    auto* node1 = get_dir_at(k);
    auto* node2 = get_dir_at(size - 1 - k);
    std::swap(node1->data, node2->data);
    return *this;
}

template <class T>
List_dl<T>& List_dl<T>::merge_sort(bool desc)
{
    list_is_valid(); // Verifica que la lista no esté vacía
    is_sorted = true; // Marca la lista como ordenada
    direction = desc; // Define la dirección de ordenamiento: descendente si desc es true
    if (size < 2) return *this;

    auto asc = [](const T& a, const T& b){return a >= b;}; // Lambda para comparación en orden ascendente (ajustada para merge)
    auto des = [](const T& a, const T& b){return a <= b;}; // Lambda para comparación en orden descendente
    auto cmp = (desc) ? (asc) : (des); // Selecciona la función de comparación según el parámetro desc

    // Paso de tamaño inicial
    for (size_t step = 1; step < size; step *= 2)
    {
        DLLNode<T>* curr = head; // Inicializa curr en head
        DLLNode<T>* new_head = nullptr; // Nueva cabeza que se definirá tras fusionar
        DLLNode<T>* new_tail = nullptr; // Nueva cola que se definirá tras fusionar

        while (curr != dummy_node_)
        {
            DLLNode<T>* left = curr;
            DLLNode<T>* right = split(left, step);
            DLLNode<T>* next = split(right, step);

            auto [merged_head, merged_tail] = merge(left, right, cmp);

            if (!new_head) new_head = merged_head;
            else
            {
                new_tail->next = merged_head;  // Conecta la cola actual con el comienzo de la sublista fusionada
                merged_head->prev = new_tail; // Actualiza el puntero previo de merged_head
            }

            new_tail = merged_tail;
            curr = next; // Avanza curr a la siguiente sublista
        }

        // Reconectar con dummy_node_
        head = new_head; // Actualiza head a new_head
        tail = new_tail; // Actualiza tail a new_tail

        dummy_node_->next = head;
        dummy_node_->prev = tail;
        head->prev = dummy_node_;
        tail->next = dummy_node_;
    }

    return *this;
}

// template <class T>
// void List_dl<T>::print_list() const
// {
//     std::cout << "List: ";
//     auto* current = head;
//     while (current != dummy_node_)
//     {
//         std::cout << current->data << " ";
//         current = current->next;
//     } std::cout << std::endl;
// }

template <class T>
void List_dl<T>::delete_at(const size_t& pos)
{
    auto* node = get_dir_at(pos);
    delete_node(node);
}

template <class T>
void List_dl<T>::delete_data(const T& val)
{
    auto* node = get_dir_of(val); // Obtiene el nodo que contiene el valor val
    if (node == dummy_node_) throw std::out_of_range("Element to delete not found");

    delete_node(node);
}

template <class T>
int List_dl<T>::delete_all(const T& val)
{
    int count = 0;
    auto* current = head;

    while (current != dummy_node_)
    {
        auto* next = current->next;
        if (current->data == val)
        {
            delete_node(current);
            count++;
        }

        current = next;
    }

    return count;
}

template <class T>
size_t List_dl<T>::get_size() const noexcept
{
    return size;
}

template <class T>
bool List_dl<T>::is_empty() const noexcept
{
    if (size == 0) return true;
    else return false;
}

template <class T>
T& List_dl<T>::get_data(const size_t& pos)
{
    return get_dir_at(pos)->data;
}

template <class T>
const T& List_dl<T>::get_data(const size_t& pos) const
{
    return get_dir_at(pos)->data;
}

template <class T>
typename List_dl<T>::it List_dl<T>::find_data(const T& val) const
{
    return it{get_dir_of(val)};
}

template <class T>
template <typename F>
typename List_dl<T>::it List_dl<T>::search_data(it start, it end, const T& k, F fun)
{
    list_is_valid();

    // if (!is_sorted)
    // {
    //     return find_data(k);
    // }

    if (end.current == dummy_node_)
    {
        --end;
    }

    while (start != end)
    {
        auto* mid = find_mid_in(start.current, end.current);
        auto mid_val = fun(mid->data);
        auto k_val = fun(k);

        if (mid_val == k_val) return it{mid};
        if (mid_val > k_val) start = it{mid->next};
        else end = it{mid->prev};
    }

    if (fun(*start) == fun(k))
    {
        return start;
    }

    return this->end();
}

template <class T>
template <typename F>
List_dl<T> List_dl<T>::find_interval(const T& start, const T& end, F fun)
{
    auto* s_node = binary_search(head, tail, start, fun); // Busca el nodo inicial usando búsqueda binaria
    auto* e_node = binary_search(head, tail, end, fun); // Busca el nodo final usando búsqueda binaria

    if (fun(start) >= fun(end) || s_node == dummy_node_ || e_node == dummy_node_)
    {
        return List_dl{};  // Si el intervalo es inválido o no se encuentran nodos, retorna una lista vacía
    }

    List_dl list; // Crea una nueva lista para almacenar el intervalo

    while (e_node != s_node->next) // Recorre desde e_node hasta el nodo siguiente a s_node

    {
        list.add_last(e_node->data);
        e_node = e_node->next;       // Avanza al siguiente nodo
    }

    return list;
}

template <class T>
const T& List_dl<T>::get_mid() const
// Metodo get_mid: retorna la data del nodo central de la lista
{
    list_is_valid();

    mid_node();

    return mid->data;
}

template <class T>
void List_dl<T>::mid_node()
{
    list_is_valid(); // Asegura que la lista no está vacía

    if (size == 1)
    {
        mid = head; // Si la lista tiene un solo elemento, el nodo central es head
        return;
    }

    auto* slow = head;
    auto* fast = head;

    while (fast != dummy_node_ && fast->next != dummy_node_)
    {
        slow = slow->next; // Puntero lento inicia en head
        fast = fast->next->next; // Puntero rápido inicia en hea
    }

    mid = slow;  // Al finalizar, slow apunta al nodo central
}

template <class T>
DLLNode<T>* List_dl<T>::split(DLLNode<T>* start, size_t n)
{
    while (--n && start != dummy_node_ && start->next != dummy_node_)
        start = start->next; // Avanza n-1 nodos o hasta llegar al final

    if (start == dummy_node_ || start->next == dummy_node_)
        return dummy_node_; // Si no hay suficientes nodos, retorna dummy_node_

    DLLNode<T>* second = start->next;

    // Desconectamos la primera sublista
    start->next = dummy_node_;
    dummy_node_->prev = start;

    // Desconectamos la segunda sublista
    second->prev = dummy_node_;

    return second;
}

template <class T>
template <typename F>
std::pair<DLLNode<T>*, DLLNode<T>*> List_dl<T>::merge(DLLNode<T>* a, DLLNode<T>* b, F cmp)
{
    DLLNode<T> dummy; // Nodo dummy local para facilitar la fusión
    DLLNode<T>* tail = &dummy; // Puntero tail inicia en el dummy

    while (a != dummy_node_ && b != dummy_node_) // Mientras ambas sublistas tengan elementos
    {
        if (cmp(a->data, b->data)) // Si a debe preceder a b según cmp
        {
            tail->next = a; // Une el nodo de a a la lista fusionada
            a->prev = tail; // Actualiza el puntero previo de a
            a = a->next; // Avanza en la sublista a
        }
        else
        {
            tail->next = b; // Une el nodo de b a la lista fusionada
            b->prev = tail; // Actualiza el puntero previo de b
            b = b->next; // Avanza en la sublista b
        }
        tail = tail->next; // Avanza el puntero tail
    }

    DLLNode<T>* rest = (a != dummy_node_) ? a : b; // Determina la sublista restante

    while (rest != dummy_node_)
    {
        tail->next = rest;
        rest->prev = tail;
        tail = rest;
        rest = rest->next;
    }

    // cerrar la lista
    tail->next = dummy_node_;
    dummy_node_->prev = tail;

    DLLNode<T>* result_head = dummy.next;
    result_head->prev = dummy_node_;

    return { result_head, tail };
}

template <class T>
DLLNode<T>* List_dl<T>::find_mid_in(DLLNode<T>* start, const DLLNode<T>* end)
{
    if (start == end)
    {
        return start;
    }

    auto* slow = start;
    auto* fast = start;

    while (fast != end && fast->next != end)
    {
        slow = slow->next; // Puntero lento inicia en start
        fast = fast->next->next; // Puntero rápido inicia en start
    }

    return slow;
}

template <class T>
template <typename F>
DLLNode<T>* List_dl<T>::binary_search(DLLNode<T>* start, DLLNode<T>* end, const T& k, F fun)
{
    list_is_valid();
    if (!is_sorted)
    {
        throw std::runtime_error("List is not sorted");
    }

    while (start != end)
    {
        auto* mid = find_mid_in(start, end); // Encuentra el nodo medio en el intervalo
        auto mid_val = fun(mid->data);   // Aplica la función fun al dato del nodo medio
        auto k_val = fun(k); // Aplica fun al valor de búsqueda k


        if (mid_val == k_val) return mid; // Si coinciden, retorna el nodo medio
        if (mid_val > k_val) start = mid->next; // Si el dato medio es mayor, busca en el subintervalo derecho
        else end = mid->prev; // De lo contrario, busca en el subintervalo izquierdo

    }

    if (fun(start->data) == fun(k))
    {
        return start;
    }

    return dummy_node_;
}

template <class T>
T& List_dl<T>::operator[](const size_t& pos)
{
    return get_data(pos); // Usa get_data para obtener el dato en pos
}

template <class T>
const T& List_dl<T>::operator[](const size_t& pos) const
{
    return get_data(pos); // Usa get_data para obtener el dato en pos (versión const)
}

template <class T>
List_dl<T>& List_dl<T>::operator=(const List_dl& other)
{
    if (this != &other)
    {
        clear();
        copy_list(other);
    }

    return *this;
}

template <class T>
List_dl<T>& List_dl<T>::operator=(List_dl&& other) noexcept
{
    if (this != &other)
    {
        clear(); // Limpia la lista actual
        head = other.head; // Transfiere el puntero head de other
        tail = other.tail; // Transfiere el puntero tail de other
        size = other.size; // Transfiere el tamaño de other

        if (head != other.dummy_node_)
        {
            dummy_node_->next = head; // Actualiza el puntero next del dummy al nuevo head
            dummy_node_->prev = tail; // Actualiza el puntero prev del dummy a la nueva cola
            head->prev = dummy_node_; // Asegura que el head tenga su prev apuntando al dummy
            tail->next = dummy_node_; // Asegura que el tail tenga su next apuntando al dummy
        }
        else // lista vacía
        {
            dummy_node_->next = dummy_node_;
            dummy_node_->prev = dummy_node_;
            head = tail = dummy_node_;
        }
        // Reinicia la otra lista a un estado vacío
        other.head = other.tail = other.dummy_node_;
        other.dummy_node_->next = other.dummy_node_;
        other.dummy_node_->prev = other.dummy_node_;
        other.size = 0;
    }

    return *this;
}

template <class T>
typename List_dl<T>::it List_dl<T>::begin()
{
    return Iterator_dll<T>(head); // Crea y retorna un iterador a partir del nodo head
}

template <class T>
typename List_dl<T>::it List_dl<T>::end()
{
    return Iterator_dll<T>(dummy_node_); // Crea y retorna un iterador a partir del nodo dummy
}

template <class T>
typename List_dl<T>::it List_dl<T>::cbegin() const
{
    return Iterator_dll<T>(head); // Retorna un iterador constante a partir del nodo head
}

template <class T>
typename List_dl<T>::it List_dl<T>::cend() const
{
    return Iterator_dll<T>(dummy_node_); // Retorna un iterador constante a partir del nodo dummy
}

template <class T>
typename List_dl<T>::it List_dl<T>::rbegin()
{
    return Iterator_dll<T>(tail); // Retorna un iterador inverso creado a partir del nodo tail
}

template <class T>
typename List_dl<T>::it List_dl<T>::crbegin() const
{
    return Iterator_dll<T>(tail); // Retorna un iterador inverso constante creado a partir del nodo tail
}

template <class T>
List_dl<T>::~List_dl()
{
    clear();
    delete dummy_node_;
}

template <class T>
void List_dl<T>::list_is_valid() const
{
    if (size == 0)
    {
        throw std::runtime_error("List is empty");
    }
}

// Metodo clear: elimina todos los nodos y reinicia head, tail y size
template <class T>
void List_dl<T>::clear()
{
    auto* current = head;         // Comienza en head

    while (current != dummy_node_) // Itera hasta alcanzar el nodo dummy
    {
        auto* next = current->next; // Guarda el siguiente nodo
        delete current;           // Elimina el nodo actual
        current = next;           // Avanza al siguiente nodo
    }

    dummy_node_->next = dummy_node_->prev = dummy_node_; // Reinicia los punteros del dummy
    head = tail = dummy_node_;     // Establece head y tail al nodo dummy
    size = 0;                     // Reinicia el tamaño a 0
}


//Metodo delete_node: elimina un nodo específico y actualiza los enlaces en la lista
template <class T>
void List_dl<T>::delete_node(DLLNode<T>* node)
{
    node->prev->next = node->next; // Enlaza el nodo previo con el siguiente del nodo a eliminar
    node->next->prev = node->prev; // Enlaza el nodo siguiente con el previo del nodo a eliminar

    if (node == head) head = node->next; // Si el nodo eliminado es head, actualiza head
    if (node == tail) tail = node->prev; // Si el nodo eliminado es tail, actualiza tail

    delete node;                 // Libera la memoria del nodo
    size--;                      // Decrementa el tamaño de la lista
}

// Metodo copy_list: copia todos los nodos de otra lista en la lista actual
template <class T>
void List_dl<T>::copy_list(const List_dl& other)
{
    auto* current = other.head;   // Comienza en el head de la otra lista

    while (current != other.dummy_node_) // Itera hasta llegar al nodo dummy de la otra lista
    {
        add_last(current->data);  // Agrega la data de cada nodo al final de la lista actual
        current = current->next;  // Avanza al siguiente nodo
    }
}

// Metodo get_dir_at: retorna el puntero al nodo en la posición pos
template <class T>
DLLNode<T>* List_dl<T>::get_dir_at(const size_t& pos)
{
    if (pos >= size) throw std::out_of_range("Argument is out of range"); // Lanza error si pos es inválido
    DLLNode<T>* current;          // Declara un puntero para recorrer la lista

    if (pos > size / 2)           // Si pos se encuentra en la segunda mitad de la lista
    {
        current = tail;         // Comienza desde tail
        for (size_t i = pos; i < size - 1; i++)
        {
            current = current->prev; // Avanza hacia atrás hasta alcanzar la posición deseada
        }
    }
    else                        // Si pos se encuentra en la primera mitad de la lista
    {
        current = head;         // Comienza desde head
        for (size_t i = 0; i < pos; i++)
        {
            current = current->next; // Avanza hacia adelante hasta alcanzar la posición pos
        }
    }
    return current;             // Retorna el nodo en la posición pos
}

template <class T>
DLLNode<T>* List_dl<T>::get_dir_of(const T& val)
{
    auto* current = head; // Comienza en head
    while (current != dummy_node_) // Recorre la lista hasta el nodo dummy
    {
        if (current->data == val) return current;
        current = current->next; // Avanza al siguiente nodo
    }
    return dummy_node_; // Si no se encuentra, retorna el nodo dummy
}
#endif //LIST_DL_H
