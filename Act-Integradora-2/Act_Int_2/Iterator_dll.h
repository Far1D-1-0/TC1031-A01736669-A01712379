#ifndef ITERATOR_DLL_H
#define ITERATOR_DLL_H

#include "Node_dll.h"

template <class T> // clase generica y trabaja con cualquier dato
class Iterator_dll //Iterador que opera nodos, reccorre datos doble
{
public:
    explicit Iterator_dll(DLLNode<T>* node); //explicit no permite conversiones implícitas
    //Constructor inicializa el iterador en el nodo determinado

    Iterator_dll(const Iterator_dll& other); // Crea un nuevo iterados como copia de otro
    Iterator_dll(Iterator_dll&& other) noexcept; // constructor no lanzará excepciones (noexcept)
    //Ayuda a tranderir los recursos de otro iterador a la nueva instancia
    T& operator*() const; // referencia al dato almacenado en el nodo actual,
    T* operator->() const; // Acceso
    Iterator_dll& operator++();
    Iterator_dll& operator--();
    bool operator==(const Iterator_dll& other);
    bool operator!=(const Iterator_dll& other);
    bool operator==(const Iterator_dll& other) const;
    bool operator!=(const Iterator_dll& other) const;
    Iterator_dll& operator=(const Iterator_dll& other);
    Iterator_dll& operator=(Iterator_dll&& other) noexcept;

    template <typename U> friend class List_dl;

private:
    DLLNode<T>* current;
};

template <class T>
Iterator_dll<T>::Iterator_dll(DLLNode<T>* node): current{node} {}

template <class T>
Iterator_dll<T>::Iterator_dll(const Iterator_dll& other)
{
    if (this != &other) // Verifica que no se realice autoasignación
    {
        current = other.current; // Copia el puntero current del iterador other
    }
}

template <class T>
Iterator_dll<T>::Iterator_dll(Iterator_dll&& other) noexcept
{
    if (this != &other)
    {
        current = other.current; // Transfiere el puntero current del iterador other
        other.current = nullptr; // Deja a other sin recurso, asignándole nullptr
    }
}

template <class T>
T& Iterator_dll<T>::operator*() const
{
    if (!current) throw std::runtime_error("Trying to dereference a null pointer");
    return current->data; // Retorna una referencia al dato almacenado en el nodo actual
}

template <class T>
T* Iterator_dll<T>::operator->() const
{
    return &(current->data); // Retorna la dirección del dato en el nodo actual para acceso a miembros

}

template <class T>
Iterator_dll<T>& Iterator_dll<T>::operator++()
{
    current = current->next; // Avanza current al siguiente nodo
    return *this; // Retorna el iterador modificado
}

template <class T>
Iterator_dll<T>& Iterator_dll<T>::operator--()
{
    current = current->prev;   // Retrocede current al nodo anterior
    return *this;
}

template <class T>
bool Iterator_dll<T>::operator==(const Iterator_dll& other)
{
    return current == other.current; // Retorna true si ambos iteradores apuntan al mismo nodo
}

template <class T>
bool Iterator_dll<T>::operator!=(const Iterator_dll& other)
{
    return current != other.current; // Retorna true si ambos iteradores apuntan a nodos diferentes
}

template <class T>
bool Iterator_dll<T>::operator==(const Iterator_dll& other) const
{
    return current == other.current; // Retorna true si ambos iteradores apuntan al mismo nodo
}

template <class T>
bool Iterator_dll<T>::operator!=(const Iterator_dll& other) const
{
    return current != other.current; // Retorna true si ambos iteradores apuntan a nodos diferentes
}

template <class T>
Iterator_dll<T>& Iterator_dll<T>::operator=(const Iterator_dll& other)
{
    if (this != &other) // Verifica que no sea autoasignación
    {
        current = other.current; // Copia el puntero current del iterador other
    }
    return *this; // Retorna el iterador actualizado
}

template <class T>
Iterator_dll<T>& Iterator_dll<T>::operator=(Iterator_dll&& other) noexcept
{
    if (this != &other) // Verifica que no sea autoasignación

    {
        current = other.current; // Transfiere el puntero current de other
        other.current = nullptr; // Pone a other.current en nullptr para liberar el recurso
    }

    return *this; // Retorna el iterador actualizado
}

#endif //ITERATOR_DLL_H
