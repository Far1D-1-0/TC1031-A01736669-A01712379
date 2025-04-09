#ifndef NODE_DLL_H
#define NODE_DLL_H

#include <iostream>

template <class T>
class DLLNode
{
public:
    DLLNode();
    explicit DLLNode(const T& data); // Declaración del constructor que recibe una referencia constante al dato (evita conversiones implícitas)
    explicit DLLNode(T&& data) noexcept; // Declaración del constructor que recibe un dato por movimiento (rvalue), garantizado que no lanza excepciones

    // Declaración de clases amigas para que List_dl y Iterator_dll puedan acceder a los miembros privados de DLLNode
    template<typename U> friend class List_dl;
    template<typename U> friend class Iterator_dll;

private:
    T data;                    // Miembro que almacena el dato del nodo (tipo T)
    DLLNode<T>* next;          // Puntero al siguiente nodo en la lista doblemente enlazada
    DLLNode<T>* prev;          // Puntero al nodo anterior en la lista doblemente enlazada
};
// Inicializa 'data' con su constructor por defecto y establece 'next' y 'prev' a nullptr
template <class T>
DLLNode<T>::DLLNode(): data{}, next{nullptr}, prev{nullptr} {}
// Copia 'data' al miembro 'data' del nodo y establece los punteros 'next' y 'prev' a nullptr, indicando que no está enlazado
template <class T>
DLLNode<T>::DLLNode(const T& data): data{data}, next{nullptr}, prev{nullptr} {}
// Implementación del constructor que inicializa el nodo mediante movimiento del dato (transferencia de recursos)
template <class T>
DLLNode<T>::DLLNode(T&& data) noexcept: data{std::move(data)}, next{nullptr}, prev{nullptr} {}
// Utiliza std::move para transferir el dato sin copiarlo, y establece 'next' y 'prev' en nullptr
#endif //NODE_DLL_H
