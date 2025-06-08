#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <vector>
#include "HashNode.h"

using std::ostream;

/**
 *
 * @tparam K Tipo de dato de la llave
 * @tparam T Tipo de dato del elemento a guardar
 * @tparam Hash Define una función hash personalizada
 *
 * La clase HashTable es una implementación de una tabla hash con open adddresing (Hashing cerrado) usando quadratic
 * probing para manejar colisiones.
 */
template<class K, class T, typename Hash>
class HashTable
{
public:
    /**
     * Constructor por defecto
     */
    HashTable();
    /**
     * Capacidad máxima de la Tabla
     */
    explicit HashTable(const size_t& capacity_);
    /**
     * Constructor de copia
     * @param other HashTable ya instllanciada
     */
    HashTable(const HashTable& other);

    /**
     * Inserta o actualiza una entrada si la llave ya existe.
     * Usa quadratic probing para manejar colisiones
     * @param key Llave a insertar
     * @param value Data de la entrada
     * @return Operación exitosa
     */
    bool insert(const K& key, const T& value);
    /**
     * Limpia el nodo y lo marca como HashNode::Status::DELETED
     * @param key Llave a borrar
     * @return Operacion exitosa
     */
    bool remove(const K& key);
    /**
     * Verifica si @code key@endcode existe en la tabla
     * @param key Llave a verificar
     * @return Llave existe
     */
    [[nodiscard]] bool contains(const K& key) const;

    /**
     * Busca una llave y regresa el elemento en dicha entrada, regrsa @code nullptr@endcode si la llave no se encuentra
     * @param key Llave a buscar
     * @return Puntero al elemento buscado
     */
    [[nodiscard]] T* find(const K& key);
    [[nodiscard]] const T* find(const K& key) const;
    /**
     * ~
     * @param os Elemento @code ostream@endcode donde escribir la información de la tabla
     */
    template <typename F1, typename F2>
    void print(std::ostream& os, F1 key_str, F2 data_str) const;
    /**
     * Vacía la tabla
     */
    void clear();

    /**
     * ~
     * @return Tamaño actual de la tabla
     */
    [[nodiscard]] size_t size() const;
    /**
     * ~
     * @return True si la tabla no tiene ningun elemento
     */
    [[nodiscard]] bool is_empty() const;

    /**
     *
     * @return Capacidad de la tabla
     */
    [[nodiscard]] size_t get_capacity() const;
    /**
     *
     * @return Factor de carga
     */
    [[nodiscard]] double get_alfa() const;
    /**
     *
     * @return Numero total de colisiones registradas
     */
    [[nodiscard]] size_t get_collisions() const;
    /**
     *
     * @param key Llave
     * @return Indice en la tabla (vector) donde se encuentra @code key@endcode
     */
    size_t get_index(const K& key) const;

    /**
     * Operador de asignación de copia
     * @param other Otro elemento HashTable
     * @return Referencia a la si misma
     */
    HashTable& operator=(const HashTable& other);

    ~HashTable();
private:
    std::vector<HashNode<K, T>> table;
    size_t current_size = 0;
    size_t capacity = 101;
    double alfa = 0.0; // (size/capacity)
    size_t collisions = 0;
    Hash hasher;
    /**
     * Usa @code base_hash()@endcode + quadratic probing para poder calcular el indice
     * @param key Llave para aplicar hashing
     * @param i Número de intento de reubicación
     * @return Indice hash calculado
     */
    size_t hash(const K& key, size_t i) const;
    /**
     * Usa @code Hash<>@endcode para calcular el índice
     * @param key Llave para aplicar hashing
     * @return
     */
    size_t base_hash(const K& key) const;
    /**
     * Duplica el tamaño de la tabla y reinserta elementos activos
     */
    void rehash();
    /**
     * Verifíca que un numero sea primo
     * @param n Numero a evaluar
     * @return True si el numero es primo
     */
    [[nodiscard]] static bool is_prime(size_t n);
    /**
     *
     * @param n Nunmero a evaluar
     * @return Siguiente numero primo más cercano
     */
    [[nodiscard]] static size_t next_prime(size_t n);
};

template <class K, class T, typename Hash>
HashTable<K, T, Hash>::HashTable(): table(101) {}


template <class K, class T, typename Hash>
HashTable<K, T, Hash>::HashTable(const size_t& capacity_)
{
    capacity = next_prime(capacity_);
    table.resize(capacity);
}

template <class K, class T, typename Hash>
HashTable<K, T, Hash>::HashTable(const HashTable& other): table(other.table), current_size(other.current_size),
                                                          capacity(other.capacity), alfa(other.alfa), collisions(other.collisions),
                                                          hasher(other.hasher) {}
/**
 * Se le inserta una nueva entrada de la tabla en hash o actualiza su valor solo si la llave es de ya existe
 * Se le utiliza un sondeo de cuadratico para poder manejar las colisiones y ademas se actualiza las metricas
 *
 * @param key Llave a insertar
 * @param value Valor asociado a la llave
 * @return True si la inserción es correcta y False si no hay espacio
 */

template <class K, class T, typename Hash>
bool HashTable<K, T, Hash>::insert(const K& key, const T& value)
{
    // Aqui se verifica si la llave ya exite de ser asi se hace una ctualizacion de su valor
    T* existing = find(key);
    if (existing != nullptr) {
        *existing = value;
        return true;
    }
    // Si se alcanza a superar la capacidad entonces se realiza rehash para expandir la tabla
    if (alfa >= 0.75) rehash();

    size_t i = 0;  // Inicializa el intento de sondeo y calcula el indice que es inicial
    size_t index = hash(key, i);
    size_t original_index = index;

    while (table[index].get_status() == HashNode<K, T>::Status::OCCUPIED) {  // Realiza el sondeo  que es cuadratico que es  hasta encontrar una posicion que este disponible
        ++collisions; // Se registra la colision
        ++i;
        index = hash(key, i);
        if (index == original_index) return false;  // Aqui si se regresa lo que es el indice actual por ello esto significa que no hay espacio
    }

    // Inserta un nuevo nodo
    table[index].set_key(key);
    table[index].set_data(value);
    table[index].set_status(HashNode<K, T>::Status::OCCUPIED);
    ++current_size;
    alfa = static_cast<double>(current_size) / capacity; // Aqui se actualiza en el factor de la carga

    if (i > 0) {  // Si hubo colisiones por ello se registra en esta posicion  de como parte del overflow
        size_t first_index = base_hash(key);
        table[first_index].add_overflow(index);
    }

    return true;
}

/**
 * Elimina una entrada de la tabla hash si la llave existe
 * Cambia el estado del nodo a eliminado
 * Por ello se reduce el tamaño de ese moento
 * Aqui se actualiza en el factor de la carga y tambien se limpia de la lista del overflow solo si aplica
 *
 * @param key Llave que es del elemento que esta a eliminar
 * @return True solo si la eliminacion  se hizo bien y sino es False solo si la llave no se encontro
 */
template <class K, class T, typename Hash>
bool HashTable<K, T, Hash>::remove(const K& key)
{
    size_t base_index = base_hash(key); // Obtiene el índice base aplicando la función hash

    if (table[base_index].get_status() == HashNode<K, T>::Status::OCCUPIED &&
        table[base_index].get_key() == key) // Verifica si la clave esta de manera lineal en la posicion base
    {
        table[base_index].clear_data();  // Limpia el nodo y actualiza las metricas
        --current_size;
        alfa = static_cast<double>(current_size) / capacity;
        return true;
    }
    // Aqui se recorre lo que es la lista del overflow si no se encuentra en el indice de la base
    for (size_t i = 0; i < table[base_index].get_overflow_size(); ++i)
    {
        size_t overflow_index = table[base_index].get_overflow_at(i);
        if (table[overflow_index].get_status() == HashNode<K, T>::Status::OCCUPIED &&
            table[overflow_index].get_key() == key) // Aqui se hace la verificacion solo si la clave se encuentra en la posicion del overflow
        {  // Elimina el dato y ademas se actualiza con  la lista del overflow y sus metricas
            table[overflow_index].clear_data();
            table[base_index].remove_overflow_at(overflow_index);
            --current_size;
            alfa = static_cast<double>(current_size) / capacity;
            return true;
        }
    }

    return false; // Si no se encuentra la clave en ningún lado, retorna false
}

/**
 * Verifica si una llave existe en la tabla hash
 * De forma interna se utiliza la funcion de find() y ademas se retorna el true solo si se encuentra un resultado correcto
 *
 * @param key es la llave a buscar
 * @return True solo si la llave se encuentra presente sino el False si no se esta
 */

template <class K, class T, typename Hash>
bool HashTable<K, T, Hash>::contains(const K& key) const {
    return find(key) != nullptr;
}

/**
 * Aqui se busca lo que es una llave en la tabla hash
 * Solo si la llave existe entonces se  retorna hacia el  apuntador lo que es el dato que es asociado y  que permite la modificacion
 * Si no se encuentra por lo tanto se retorna nullptr
 *
 * Aqui nos permite la modificacion de el valor del dato que es encontrado
 *
 * @param key Llave que se desea buscar en la tabla
 * @return Aqui solo si el puntero al dato esta correspondiendo solo si se encuentra sino el nullptr  no existe
 */

template <class K, class T, typename Hash>
T* HashTable<K, T, Hash>::find(const K& key)
{ // Aqui se calcula lo que es el indice de la base a partir de la función hash
    size_t index = base_hash(key);
    if (table[index].get_status() == HashNode<K, T>::Status::OCCUPIED && table[index].get_key() == key)
    { // Aqui si se verifica solo si la clave esta de manera directa en el indice de la base
        return &table[index].data;
    }

    for (size_t i = 0; i < table[index].get_overflow_size(); ++i) // Devuelve lo que es un puntero que es modificable con base al dato
    { // En este se recorre la lista de desbordamiento para la busqueda de la clave
        size_t overflow_index = table[index].get_overflow_at(i);
        if (table[overflow_index].get_status() == HashNode<K, T>::Status::OCCUPIED &&
            table[overflow_index].get_key() == key)  // Aqui es si la clave coincide y el nodo se ecuentra ocupado por ello se retorna el dato
        {
            return &table[overflow_index].data; // Aqui se devuelve el puntero lo que es el dato encontrado
        }
    }

    return nullptr; // Si no se encuentra la clave entonces se retorna nullptr
}

/**
 * Busca una llave en la tabla hash
 * Si la llave existe entonces se retorna un apuntador al dato
 * Si no se encuentrar para ello va a retornar nullptr.
 *
 * Esta version no permite modificar el valor que ha sido encontrado
 *
 * @param key Llave que se desea buscar en la tabla
 * @return Aqui el untero es constante con base al dato correspondiente solo si se encuentra
 */

template <class K, class T, typename Hash>
const T* HashTable<K, T, Hash>::find(const K& key) const
{
    size_t index = base_hash(key); // Aqui se calcula el indice de la base usando la funcion

    if (table[index].get_status() == HashNode<K, T>::Status::OCCUPIED &&
        table[index].get_key() == key)  // Aqui se verifica solo si la clave esta de forma directa en la base del indice
    {
        return &table[index].get_data(); // Aqui se devuelve el puntero que es constante al dato
    }

    for (size_t i = 0; i < table[index].get_overflow_size(); ++i)
    { // Si no se encuentra por lo tanto se revisa en  la lista de overflow del indice
        size_t overflow_index = table[index].get_overflow_at(i);
        if (table[overflow_index].get_status() == HashNode<K, T>::Status::OCCUPIED &&
            table[overflow_index].get_key() == key)
        { // Aqui solo si la clave se encuentra en alguna posicion de overflow ademas se retorna
            return &table[overflow_index].get_data();  // Devuelve el puntero  que esconstante
        }
    }

    return nullptr; // Si la clave no existe en ninguna parte entonces  se retorna nullptr
}

template <class K, class T, typename Hash>
template <typename F1, typename F2>
void HashTable<K, T, Hash>::print(std::ostream& os, F1 key_str, F2 data_str) const
{
    os << "Tabla Hash (capacidad: " << capacity << ", tamaño actual: " << current_size << ")\n";
    os << "--------------------------------------------------------------\n";

    for (size_t i = 0; i < table.size(); ++i)
    {
        os << "[" << i << "] ";

        switch (table[i].get_status()) {
        case HashNode<K, T>::Status::EMPTY:
            os << "EMPTY";
            break;

        case HashNode<K, T>::Status::OCCUPIED:
            os << "OCCUPIED | Key: " << key_str(table[i].get_key())
               << " | Value: " << data_str(table[i].get_data());
            break;

        case HashNode<K, T>::Status::DELETED:
            os << "DELETED";
            break;
        }

        if (table[i].get_overflow_size() > 0)
        {
            os << " | Overflow: ";
            for (size_t j = 0; j < table[i].get_overflow_size(); ++j)
            {
                os << table[i].get_overflow_at(j);
                if (j + 1 < table[i].get_overflow_size())
                    os << ", ";
            }
        }

        os << '\n';
    }

    os << "--------------------------------------------------------------\n";
}

/**
 * Limpia  la tabla hash eliminando todos los elementos
 * Reinicia el vector de los nodos en el tamaño actual y asi el numero de colisiones y el factor de carga
 */

template <class K, class T, typename Hash>
void HashTable<K, T, Hash>::clear()
{
    table = std::vector<HashNode<K, T>>(capacity); // Reinicia la tabla con nodos vacíos
    current_size = 0; // Reinicia el contador de elementos ocupados
    alfa = 0.0; // Reinicia el factor de carga
    collisions = 0; // Reinicia el contador de colisiones
}

/**
 * Regresa la cantidad de los  elementos que  actualmente estan  almacenados en la tabla hash
 *
 * @return Regresa el numero de los elementos con estado ocupado
 */
template <class K, class T, typename Hash>
size_t HashTable<K, T, Hash>::size() const
{
    return current_size;
}

/**
 * Aqui se hace la verificacion solo si la tabla se ecuentra vacia
 *
 * @return aqui es de que true si no hay elementos ocupados en la tabla, sino sera false en caso contrari
 */

template <class K, class T, typename Hash>
bool HashTable<K, T, Hash>::is_empty() const
{
    return current_size == 0;
}

/**
 * En eeste obtiene la capacidad actual de la tabla hash
 *
 * @return El numero total de los espacios que son disponibles en la tabla
 */

template <class K, class T, typename Hash>
size_t HashTable<K, T, Hash>::get_capacity() const
{
    return capacity;
}

/**
 * Aqui se calcula y se retorna el factor de carga de la tabla
 * El factor de carga se define como el tamaño actual y la capacidad total
 *
 * @return resegresa el valor decimal del factor de la carga
 */

template <class K, class T, typename Hash>
double HashTable<K, T, Hash>::get_alfa() const
{
    return alfa;
}

/**
 * En este se retorna el numero total de las colisiones que han estado ocurriendo durante las inserciones
 *
 * @return por lo tanto regresa el total de las colisiones que estan registradas
 */

template <class K, class T, typename Hash>
size_t HashTable<K, T, Hash>::get_collisions() const
{
    return collisions;
}

template <class K, class T, typename Hash>
size_t HashTable<K, T, Hash>::get_index(const K& key) const
{
    size_t index = base_hash(key); // Se calcula el indice base usando la funcin hash principal
    if (table[index].get_status() == HashNode<K, T>::Status::OCCUPIED &&  table[index].get_key() == key)
    { // // Verifica si en ese índice está la clave buscada y su estado es valido
        return index; // Si la clave se encuentra de forma directa en el indice  de la bas entonces se retorna
    }
    for (size_t i = 0; i < table[index].get_overflow_size(); ++i)
    { // Si no se encuentra en el indice base entonces se revisa la lista de desbordamientos
        size_t overflow_index = table[index].get_overflow_at(i);  //  En este se obtiene el inice del nodo que esta en la lista del overflow
        const auto& ovr = table[overflow_index];  // Aqui se accede al nodo en la posicion de overflow
        if (ovr.get_status() ==  HashNode<K, T>::Status::OCCUPIED && ovr.get_key() == key)
        { // Aqui se realiza y se hace la vcerificacion de que si la clave coincide y el nodo está ocupado
            return overflow_index; // Se devuelve el indice que se encontro de la clase de overflow
        }
    }
    // Si no se encuentra la clave ni en el indice base ni en overflow, se manda una excepcion
    throw std::out_of_range("Out of range");
}

template <class K, class T, typename Hash> // oprador que es de asignacion por copia
HashTable<K, T, Hash>& HashTable<K, T, Hash>::operator=(const HashTable& other) // copia todos los valores que son de la otra tabala a la ctual
{ // other es la otra instancia del que se copiaran los datos
    if (this == &other) return *this; // Evita autoasignación

    table = other.table; // Copia todos los atributos que son importante
    current_size = other.current_size;
    capacity = other.capacity;
    collisions = other.collisions;
    alfa = other.alfa;
    hasher = other.hasher;

    return *this; // hace referencua a la instancia actual en la que se hace la copia
}

template <class K, class T, typename Hash>
HashTable<K, T, Hash>::~HashTable()  // se maneja de forma automatica el compilador
= default; // aqui es el desctrutor por defecto ademas se libera cualquier recurso utilizado por la tabla hash

template <class K, class T, typename Hash>
size_t HashTable<K, T, Hash>::hash(const K& key, size_t i) const
{ // se calcula aqui el indice de hash en donde se le aplica el sondeo que es cuadratico
    return (base_hash(key) + i*i) % capacity;
    // esto es para resolver las colisiones
    // adeamas el i es el numero que se incrementa sin embargo es el primer intento
} // aqui indica el indice calcula para la busqueda la llave tabla

template <class K, class T, typename Hash>
size_t HashTable<K, T, Hash>::base_hash(const K& key) const
{ // esta calculado el valo de hash base para que exista una llave que sea utilizada por lafuncion
    return hasher(key) % capacity; // key es la que s ele aplica la fubcion hash
} // hay un indice de la base del rango y su capacidad de la tabla

template <class K, class T, typename Hash>
void HashTable<K, T, Hash>::rehash()
{ // aqui se aumenta la capidad de lo que es el docble y se ajusta al siuinete nuemro que es primo
    capacity = next_prime(capacity * 2); // Aqui dupluca la capacidad y ademas lo ajusta a lo siguiente numero que es primo
    std::vector<HashNode<K, T>> old_table = table;
    /**
     * La ifnalidad es el poder reducir las colisiones y ademas el mejorar lo que es la distribucion de los datos
     * Ademas se crea una nueva tabla y  ademas se reinserta todos los elementos que sean veridicos desde la tabla anterior
     *
     * Por ultimo este metodo se activa de forma automatica solo si  la capacidad actual ya no es suficiente
     * ademas se hace insercion de nuevos componentes
     */
    // Crear nueva tabla vacíia con una nueva capacidad
    table = std::vector<HashNode<K, T>>(capacity);
    current_size = 0;
    collisions = 0;
    alfa = 0.0;

    for (const auto& node : old_table)
    { // reiniciar todos los elemtos que sean validos de la tabla anterior
        if (node.get_status() == HashNode<K, T>::Status::OCCUPIED)
        {
            insert(node.get_key(), node.get_data());
        }
    }
}

/**
 * Aqui se hace la verificacion si el numero es primo
 * Se esta utilizando como apoyo para poder calcular la siguiente capacidad si es optima
 * de la tabla hash y ademas evitando los factores que aumentan las colisiones
 *
 * @param n es el numero que se evaluara
 * @return True si el numero es el primo entonces False es el caso contrario
 */

template <class K, class T, typename Hash>
bool HashTable<K, T, Hash>::is_prime(size_t n)
{
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    // Aqui realiza la verificacion de los numero si son primos
    for (size_t i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;

    return true;
}

/**
 * Obtiene el siguiente numero primo que es mayor o igual a n
 * Se utiliza para definir la nueva capacidad de la tabla al hacer  el rehash,
 * Se esta asegurando una mejor distribucion y una disminucion de cantidad de colisiones
 *
 * @param n numero inicial desde donde comienza la busqueda
 * @return El siguiente numero es primo solo si >= n.
 */

template <class K, class T, typename Hash>
size_t HashTable<K, T, Hash>::next_prime(size_t n)
{
    while (!is_prime(n)) ++n;
    return n;
}
#endif //HASHTABLE_H
