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

struct RegistroDispositivo {
    std::string nombre;
    std::string tipo;
    double potencia;
    std::string estado;
};

class BaseDatos {
private:
    sqlite3* db;

public:
    BaseDatos();
    ~BaseDatos();

    bool abrirConexion();
    void inicializarTablas();
    sqlite3* getConexion() const { return db; }

    bool validarUsuario(const std::string& usuario, const std::string& clave);
    void guardarDispositivo(const std::string& nombre, const std::string& tipo, double potencia);
    std::vector<RegistroDispositivo> cargarDispositivos();
    void actualizarEstado(const std::string& nombre, const std::string& estado);
    bool existeDispositivo(const std::string& nombre);
    void registrarEvento(const std::string& dispositivo, const std::string& accion);
};

#endif
