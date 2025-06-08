#ifndef IP_H
#define IP_H

#include <iostream>
#include <string>
#include <vector>
#include <ostream>
#include <iomanip>
#include "LineRead.h"

using std::string;
using std::vector;
using std::ostream;
using std::cout;
using std::endl;

class IP { // Declaracion de la clase IP que representa una dirección IP
public:
    IP(); // Constructor por defectoq ue inciia los valores de la dirección IP a valores especificos
    IP(const IP& other); // Constructor de copia que crea una instancia de IP duplicando los valores de otro IP
    IP(IP&& other) noexcept ; // Constructor de movimiento
    explicit IP(const string&  ip); // Constructor que recibe una dirección IP en string
    IP(const string& ip, const size_t& ic, const size_t& oc, const double& ior); // Constructor que recibe dirección IP y sus conexiones ic y oc
    void set_ic(size_t ic); // tecnica para establecer el valor de las conexiones de ic
    void set_oc(size_t oc); // establece la direccion de IP en el modelo numerico
    void set_ip(const string& ip); // Esta tecnica es para establecer la direccion de la IP en el modelo numerico
    void set_in_out_ratio(double ior);
    [[nodiscard]] const string& get_ip_s() const; // Esta tecnica es para obtener la direccion de la IP en el formato de la cadena
    [[nodiscard]] const size_t& get_ip_i() const; // Obtiene la dirección IP en modelo numerico
    [[nodiscard]] const size_t& get_ic() const; // Se obtiene la conexion que entra de ic
    [[nodiscard]] const size_t& get_oc() const; // // se obtiene las conexiones que salen de oc
    [[nodiscard]] const double& get_in_out_ratio() const;
    void getIPSummary(ostream& os) const; // Esta tecnica es para imprimir la direccion de la IP y sus conexiones  de salida

    // Operadores de comparación entre los objetos de IP
    bool operator==(const IP& other) const; // Comparación de igualdad
    bool operator<(const IP& other) const; // Comparación menor que
    bool operator>(const IP& other) const; // Comparación mayor que
    bool operator<=(const IP& other) const; // Comparación menor o igual que
    bool operator>=(const IP& other) const; // Comparación mayor o igual que
    bool operator!=(const IP& other) const; // Comparación desigual
    IP& operator=(const IP& other); // Operador que es asignado por copia
    IP& operator=(IP&& other) noexcept; // Operador de asignación por movimiento

    ~IP(); // Destructor de la clase IP que tiene el rol de limpiar los recursos que se usaron del objeto
private:
    string ip_s = "000.000.000.000"; // Direccion IP en formato de cadena
    size_t ip_i = 0; // Aquie es la direccion de IP en formato numrico
    size_t ic = 0; // // Contador de conexiones entrantes
    size_t oc = 0; // Contador de conexiones salientes
    double in_out_ratio = 0;

    static inline char sep = '.'; // Aqui es el caracter que es de  separador para las direcciones IP
    static inline unsigned short int n = 4; // Numero de octetos que dsse debe tener la dirección IP

};

#include <functional>

namespace std {
    template <>
    struct hash<IP> {
        std::size_t operator()(const IP& i) const {
            return std::hash<size_t>()(i.get_ip_i());
        }
    };
}


#endif //IP_H
