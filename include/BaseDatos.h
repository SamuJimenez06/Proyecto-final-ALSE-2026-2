/**
 * @file BaseDatos.h
 * @brief Acceso a la base de datos SQLite: usuarios, dispositivos y registro de eventos.
 */

#ifndef BASE_DATOS_H
#define BASE_DATOS_H

#include <sqlite3.h>
#include <string>
#include <vector>

/**
 * @brief Datos de un dispositivo tal como se guardan en la base de datos.
 */
struct RegistroDispositivo {
    std::string nombre;
    std::string tipo;
    double potencia;
    std::string estado;
};

/**
 * @brief Maneja la conexión con SQLite y todas las consultas del sistema.
 */
class BaseDatos {
private:
    sqlite3* db;   ///< Conexión a la base de datos

public:
    BaseDatos();
    ~BaseDatos();

    bool abrirConexion();

    /// Crea las tablas Dispositivos, Eventos y Usuarios si no existen, y el usuario admin.
    void inicializarTablas();

    /// Devuelve la conexión para hacer consultas directas (ej. historial).
    sqlite3* getConexion() const { return db; }

    // ----- Usuarios -----
    /// Verifica usuario y contraseña contra la tabla Usuarios (login).
    bool validarUsuario(const std::string& usuario, const std::string& clave);

    // ----- Dispositivos -----
    /// Guarda un dispositivo nuevo.
    void guardarDispositivo(const std::string& nombre, const std::string& tipo, double potencia);

    /// Lee todos los dispositivos guardados (se usa al abrir el programa).
    std::vector<RegistroDispositivo> cargarDispositivos();

    /// Guarda el nuevo estado (Encendido/Apagado/Fallo) de un dispositivo.
    void actualizarEstado(const std::string& nombre, const std::string& estado);

    /// Indica si ya existe un dispositivo con ese nombre (evita repetidos).
    bool existeDispositivo(const std::string& nombre);

    // ----- Eventos -----
    /// Registra una acción en la tabla Eventos con fecha y hora.
    void registrarEvento(const std::string& dispositivo, const std::string& accion);
};

#endif
