/**
 * @file BaseDatos.h
 * @brief Acceso a la base de datos SQLite: usuarios, dispositivos y registro de eventos.
 * @author Samuel Jiménez, Brayan Villamil
 * @date 2026-11
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
 * @class BaseDatos
 * @brief Maneja la conexión con SQLite y todas las consultas del sistema.
 *
 * Tablas que administra:
 * - **Usuarios:** usuario y contraseña cifrada con SHA-256.
 * - **Dispositivos:** nombre, tipo, potencia y último estado.
 * - **Eventos:** historial de acciones con fecha y hora.
 *
 * Todas las consultas usan sentencias preparadas (sqlite3_prepare_v2 + bind)
 * para evitar inyección SQL.
 */
class BaseDatos {
private:
    sqlite3* db;   ///< Conexión a la base de datos

public:
    /// Crea el objeto sin conexión abierta.
    BaseDatos();

    /// Cierra la conexión si está abierta.
    ~BaseDatos();

    /**
     * @brief Abre (o crea) el archivo domotica.db junto al ejecutable.
     * @return true si la conexión se abrió correctamente.
     */
    bool abrirConexion();

    /// Crea las tablas Dispositivos, Eventos y Usuarios si no existen, y el usuario admin.
    void inicializarTablas();

    /// Devuelve la conexión para hacer consultas directas (ej. historial).
    sqlite3* getConexion() const { return db; }

    // ----- Usuarios -----
    /**
     * @brief Verifica usuario y contraseña contra la tabla Usuarios (login).
     * @param usuario Nombre de usuario.
     * @param clave Contraseña en texto plano (se compara su hash).
     * @return true si las credenciales son correctas.
     */
    bool validarUsuario(const std::string& usuario, const std::string& clave);

    // ----- Dispositivos -----
    /**
     * @brief Guarda un dispositivo nuevo con estado inicial "Apagado".
     * @param nombre Nombre del dispositivo.
     * @param tipo "Luz", "Persiana" o "Sensor".
     * @param potencia Potencia en vatios.
     */
    void guardarDispositivo(const std::string& nombre, const std::string& tipo, double potencia);

    /// Lee todos los dispositivos guardados (se usa al abrir el programa).
    std::vector<RegistroDispositivo> cargarDispositivos();

    /**
     * @brief Guarda el nuevo estado de un dispositivo.
     * @param nombre Nombre del dispositivo.
     * @param estado "Encendido", "Apagado" o "Fallo".
     */
    void actualizarEstado(const std::string& nombre, const std::string& estado);

    /**
     * @brief Indica si ya existe un dispositivo con ese nombre (sin distinguir mayúsculas).
     * @param nombre Nombre a buscar.
     * @return true si el nombre ya está registrado.
     */
    bool existeDispositivo(const std::string& nombre);

    // ----- Eventos -----
    /**
     * @brief Registra una acción en la tabla Eventos con la fecha y hora actuales.
     * @param dispositivo Nombre del dispositivo.
     * @param accion Descripción de la acción (ej. "Encendido", "Fallo Simulado").
     */
    void registrarEvento(const std::string& dispositivo, const std::string& accion);
};

#endif
