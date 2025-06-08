#ifndef HASHNODE_H
#define HASHNODE_H

#include <vector> //
#include <algorithm>
#include <iterator>
#include <functional>

template <class K, class T, class Hash = std::hash<K>> // Esta plantilla de la clase revibe cualquier tipo de clave K
class HashTable;
// Esto es la definicion de la clase HashNode
// Representa lo que es un nodo dentro de la tabla hash
template <class K, class T>
// La plantilla de la clase es para los nodos de la tabla hash
// Ademas aceptan las claves de tipo K y los valores de tipo T
class HashNode
{
public: // Constructores
    // Aqui es la enumeracion que es interna para poder representar el estado de un nodo que es de la tabla hashNode
    enum class Status { EMPTY, OCCUPIED, DELETED };  // Aqui es eleEstatus del nodo que es vacio, ocupado o eliminado
    HashNode(); // Awui son los constructores y metodos de la clase HashNode

    // Constructor que es por defecto asimismo inicia el nodo del estado vacio
    // Este es la tecnica de configuracion
    void set_key(const K& key); // Esto es la clave del nodo
    void set_data(const T& data); // se crea el valor de datos del nodo y ademas lo marca como ocupado
    void set_status(const Status& status); // Se crea y ademas se establece todso aquello del estado del nodo
    void add_overflow(const size_t& index); // Se suma un indice del  desbordamiento

    // Metodos de acceso es decir getter
    [[nodiscard]] const K& get_key() const; // Aqui se devuelve lo de la clave del metod0
    [[nodiscard]] const T& get_data() const; // Se regresan los datos del nodo
    [[nodiscard]]Status get_status() const; // Devuelve el estado actual del nodo
    [[nodiscard]] size_t get_overflow_size() const; // Regresa el numero de elementos que esta en la lista que es de desbordes
    [[nodiscard]] size_t get_overflow_at(const size_t& index) const; // Regresa el indice que es de un desbordamiento determinado

    // Mtodos de modificación de la lista de desbordes
    void remove_overflow_at(size_t index); // Elimina el indice de desbordamiento
    void clear_data(); // Limpia todos los datos del nodo y ademas lo marca como eliminado

    // Destructor
    ~HashNode(); // Destructor que es por defecto

    // Aqui es la declaracion de la clase HashTable ademas es como amiga que para permitir el acceso a los elemnetos privados de HashNode
    template <typename, typename, typename>
    friend class HashTable;

private:
    K key{}; // Aqui la clave del nodo
    T data{}; // Valor que esta asociado a la clave
    Status status; // Estado en el nodo que es por defecto
    std::vector<size_t> overflow; // Lista de los indices qyue son de desbordes
};

//  aqui es la implementacin de  todos los metodos de la clase HashNode

template <class K, class T> // aqui inicia el constructor que es por defecto
// Aqui inicia el estado que es del nodo como vacio
HashNode<K, T>::HashNode(): status(Status::EMPTY)
{}

template <class K, class T> // Se crea y establece lo que es la clave del nodo
void HashNode<K, T>::set_key(const K& key)
{
    this->key = key; // Se le asigna el valor de la clave
}

template <class K, class T> // Se le crea los datos del nodo ademas marca ocupado
void HashNode<K, T>::set_data(const T& data)
{
    this->data = data; //  Se le indica que es el  valor de los datos
    this->status = Status::OCCUPIED; // Aqui se marca lo que es el estado como ocupado
}

template <class K, class T> // Se le crea el estado del nodo
void HashNode<K, T>::set_status(const Status& status)
{
    this->status = status; // Se le indica lo que es el estado proporcionado al nodo
}

template <class K, class T>  // Indica el indice de desbordamiento para la lista de desbordes
void HashNode<K, T>::add_overflow(const size_t& index)
{
    overflow.push_back(index); // se agrega un indice de desbordamiento a la lista
}

template <class K, class T> // Devuelve la clave del nodo
const K& HashNode<K, T>::get_key() const
{
    return key; // // Devuelve la clave lo que esta almacenado
}

template <class K, class T> // Devuelve los datos del nodo
const T& HashNode<K, T>::get_data() const
{
    return data; // regresa los datos almacenados
}
// regresa el estado del nodo
template <class K, class T>
typename HashNode<K, T>::Status HashNode<K, T>::get_status() const
{
    return status; // regresa el estado actual del nodo
}

template <class K, class T> // Devuelve la dimension de la lista de desbordes
size_t HashNode<K, T>::get_overflow_size() const
{
    return overflow.size(); // Esto hace que se regrese  la dimension de la lista que hay del  desborde
}

template <class K, class T>
size_t HashNode<K, T>::get_overflow_at(const size_t& index) const
 // Refresa el indice de desbordamiento en la posicion que es especificada
{
    return overflow[index];
}
// Borra el indice dteermanida de la lista de desbordes
template <class K, class T>
void HashNode<K, T>::remove_overflow_at(size_t index)
{
    // Busca lo que es el indice en la lista de desbordes y lo borra
    if (const auto it = std::ranges::find(overflow, index); it != overflow.end())
    {
        overflow.erase(it); // Borra el indice que esta en  la lista
    }
}

template <class K, class T> // Hace una limpia de los datos del nodo y marca el estado como eliminado
void HashNode<K, T>::clear_data()
{
    data = T(); //formula los datos a su valor por defecto
    status = Status::DELETED; // Marca el estado como eliminado
}
// Destructor que es porr defecto de la clase HashNode
template <class K, class T>
HashNode<K, T>::~HashNode() = default;

#endif //HASHNODE_H
