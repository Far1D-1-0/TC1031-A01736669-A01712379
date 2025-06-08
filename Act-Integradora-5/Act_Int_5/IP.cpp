#include "IP.h"

#include <utility>

IP::IP() = default; // Constructor por defecto que inicia los atributos a los valores especificos

IP::IP(const IP& other) // este es el constructor de la copia que creara la instancia de la Ip copiando los valores del otro objeto de IP
{
    ip_s = other.ip_s; // realiza una copia del valor de la direccion de IP en un formato de cadena
    ip_i = other.ip_i; // replica el valor de la direccion de IP en modelo numerico
    ic =  other.ic; // replica el valor de la conexion de la entrada
    oc = other.oc; // replica el valor de la conexion output
    in_out_ratio  = other.in_out_ratio;
}

IP::IP(IP&& other) noexcept // constructor del movimiento que hace la trasnferencia de valores a otro objeto
{
    ip_s = std::move(other.ip_s); // realiza movimientos con base el valor de la direccion de la IP dentro del formato de la cadena
    ip_i = other.ip_i; // Duplica el valor de la direccion IP pero con modelo numerico
    ic = other.ic; // Replica el valor de las conexiones de ic que son input
    oc = other.oc; // Replica el valor de las conexiones output de oc
    in_out_ratio  = other.in_out_ratio;
}

IP::IP(const string& ip) // Constructor que recibe la dirección IP en string
{
    set_ip(ip); // Llama a la funcion set_ip para determinar la direcccion de la IP en el modelo numerico
}

IP::IP(const string& ip, const size_t& ic, const size_t& oc, const double& ior): ic(ic), oc(oc), in_out_ratio(ior) // recibe toda direccion IP, conexion de entrada y salida
{
    set_ip(ip); //Si se establece en la direccion de IP en set_ip
}

void IP::set_ic(size_t ic) // Funcion que establece el valor de las conexiones de input de ic
{
    this->ic = ic; // Se le agrega un valor a la ic que es la variable que pertence a ic
}

void IP::set_oc(size_t oc) // Esta funcion establece el valor de las conexiones de output
{
    this->oc = oc; // Se le agrega un valor de oc a la variable que pertenece a la oc
}

void IP::set_ip(const string& ip) // Esta funcion se le establece que la direccion Ip esta en modelo numerico y de la cadena
{
    ip_s = ip; // Se le asigna un valor a la direccion de IO para la forma de cadena en este caso ip_s
    LineRead lr(sep, n);   // Se le crea un objeto con LineRead para poder hacer divisiones con la direccion de IP ensus elemento
    lr.setLine(ip_s); // se crea lineas con la direccion de IP
    size_t iaux = 1000000000;
    for (const auto& e : lr.getElements()) // Esta linea va iterando de cada componenete que se obtienen de la IP
    {
        ip_i += stoull(e) * iaux;
        iaux = iaux / 1000;

    }
}

void IP::set_in_out_ratio(double ior)
{
    this->in_out_ratio = ior;
}

const string& IP::get_ip_s() const // funcion que va a obtener la direccion de la IP en el modelo numerico
{
    return ip_s; // regresa la direccion de la IP en forma string
}

const size_t& IP::get_ip_i() const // Funcion que se obtiene la direccion IP pero en el formato numerico
{
    return ip_i; // se devuelve la direccion de la IP en forma de numero
}

const size_t& IP::get_ic() const  // funcion que se obtiene las conexiones de input
{
    return ic; // regresa las conexiones de output
}

const size_t& IP::get_oc() const // funcion que obtiene las conexiones de output
{
    return oc; // regresa las conexiones de output
}

const double& IP::get_in_out_ratio() const
{
    return in_out_ratio;
}

void IP::getIPSummary(ostream& os) const// Esta funcion imprime la direccion de la IP junto con sus conexiones
{
    os << "Total de direcciones accesadas desde la IP: " << oc << endl;
    os << "Total de direcciones que intentaron acceder a la IP: " << ic << endl;
    os << "Relacion entre el numero de direcciones accesadas y que intentaron acceder a la IP: " <<  std::fixed << std::setprecision(2) << in_out_ratio <<endl;
}

bool IP::operator==(const IP& other) const // operador de comparacion si las IP son iguales
{
    return ip_i == other.ip_i; // realiza compraciones de la IP numericas
}

bool IP::operator<(const IP& other) const  // operador de menor que
{
    return ip_i < other.ip_i; // realiza compraciones de la IP numericas
}

bool IP::operator>(const IP& other) const // operador de mayor que
{
    return ip_i > other.ip_i; // realiza compraciones de la IP numericas
}

bool IP::operator<=(const IP& other) const // operador de comparacion menor o igual que
{
    return ip_i <= other.ip_i;// realiza compraciones de la IP numericas
}

bool IP::operator>=(const IP& other) const // operador de comparacion mayor o igual que
{
    return ip_i >= other.ip_i;// realiza compraciones de la IP numericas
}

bool IP::operator!=(const IP& other) const // operador de comparacion desigual
{
    return ip_i != other.ip_i; // realiza compraciones de la IP numericas
}

IP& IP::operator=(const IP& other) // operador que asigna por duplicacion
{
    if (this != &other) // esto nos indica que se asegure que no realice copias sobre si mismo
    {
        ip_s = other.ip_s; // copia la direccion de la IP en string
        ip_i = other.ip_i; // duplica la direccion de la IP en numerico
        ic = other.ic; // Copia las conexiones de input
        oc = other.oc; // copia las direcciones de output
        in_out_ratio = other.in_out_ratio;
    }
    return *this; // va a regresar el objeto de ahora
}

IP& IP::operator=(IP&& other) noexcept// el operador es de asigancion en movimiento
{
    if (this != &other) // se asegura en que no exista ningun movimiento sobre el
    {
        ip_s = std::move(other.ip_s); // hace que la direccion se mueva de IP a string
        ip_i = other.ip_i; // duplica la direccion de IP en numerico
        ic = other.ic; // duplica la conexicon de input
        oc = other.oc; // duplica la conexion en output
        in_out_ratio = other.in_out_ratio;
        other.ip_s = "000.000.000.000";
        other.ip_i = 0;
        other.ic = 0;
        other.oc = 0;
        other.in_out_ratio = 0;
    }
    return *this; // regresa el objeto al de ahora
}

IP::~IP() // destructor
= default;  // destructor por defecto ya que elimina el objeto sin hacer nada por agregar
