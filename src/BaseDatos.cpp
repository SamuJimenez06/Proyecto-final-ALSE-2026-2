/**
 * @file BaseDatos.cpp
 * @brief Implementación del acceso a SQLite: usuarios, dispositivos y registro de eventos.
 */

#include "BaseDatos.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QCryptographicHash>
#include <iostream>

/// Convierte una contraseña en su hash SHA-256 (así no se guarda en texto plano).
static std::string hashClave(const std::string& clave) {
    QByteArray hash = QCryptographicHash::hash(QByteArray::fromStdString(clave),
                                               QCryptographicHash::Sha256);
    return hash.toHex().toStdString();
}

BaseDatos::BaseDatos() : db(nullptr) {}

BaseDatos::~BaseDatos() {
    if (db) sqlite3_close(db);
}

bool BaseDatos::abrirConexion() {
    QString ruta = QCoreApplication::applicationDirPath() + "/domotica.db";
    return sqlite3_open(ruta.toUtf8().constData(), &db) == SQLITE_OK;
}

void BaseDatos::inicializarTablas() {
    const char* sqlDisp = "CREATE TABLE IF NOT EXISTS Dispositivos ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                          "nombre TEXT UNIQUE, tipo TEXT, potencia_watts REAL, estado TEXT);";
    sqlite3_exec(db, sqlDisp, 0, 0, 0);

    // Tabla de eventos
    const char* sqlEvt = "CREATE TABLE IF NOT EXISTS Eventos ("
                         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "dispositivo TEXT, accion TEXT, fecha TEXT);";
    sqlite3_exec(db, sqlEvt, 0, 0, 0);

    // Tabla de usuarios (la contraseña se guarda como hash, nunca en texto plano)
    const char* sqlUsr = "CREATE TABLE IF NOT EXISTS Usuarios ("
                         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "usuario TEXT UNIQUE NOT NULL, clave_hash TEXT NOT NULL);";
    sqlite3_exec(db, sqlUsr, 0, 0, 0);

    // Usuario por defecto: admin / 1234 (solo se crea si no existe)
    sqlite3_stmt* stmt;
    const char* sqlAdmin = "INSERT OR IGNORE INTO Usuarios (usuario, clave_hash) VALUES (?, ?);";
    if (sqlite3_prepare_v2(db, sqlAdmin, -1, &stmt, nullptr) == SQLITE_OK) {
        std::string hashAdmin = hashClave("1234");
        sqlite3_bind_text(stmt, 1, "admin", -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, hashAdmin.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

bool BaseDatos::validarUsuario(const std::string& usuario, const std::string& clave) {
    sqlite3_stmt* stmt;
    const char* query = "SELECT 1 FROM Usuarios WHERE usuario = ? AND clave_hash = ?;";
    bool valido = false;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        std::string hash = hashClave(clave);
        sqlite3_bind_text(stmt, 1, usuario.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, hash.c_str(), -1, SQLITE_TRANSIENT);
        valido = (sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
    }
    return valido;
}

void BaseDatos::guardarDispositivo(const std::string& nombre, const std::string& tipo, double potencia) {
    sqlite3_stmt* stmt;
    std::string query = "INSERT INTO Dispositivos (nombre, tipo, potencia_watts, estado) VALUES (?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, tipo.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 3, potencia);
        sqlite3_bind_text(stmt, 4, "Apagado", -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

void BaseDatos::registrarEvento(const std::string& dispositivo, const std::string& accion) {
    sqlite3_stmt* stmt;
    std::string query = "INSERT INTO Eventos (dispositivo, accion, fecha) VALUES (?, ?, ?);";

    // La fecha se guarda en una variable que vive hasta el final de la función
    std::string fechaActual = QDateTime::currentDateTime()
                                  .toString("yyyy-MM-dd HH:mm:ss")
                                  .toStdString();

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        // SQLITE_TRANSIENT hace que SQLite copie el texto
        sqlite3_bind_text(stmt, 1, dispositivo.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, accion.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, fechaActual.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

std::vector<RegistroDispositivo> BaseDatos::cargarDispositivos() {
    std::vector<RegistroDispositivo> lista;
    sqlite3_stmt* stmt;
    const char* query = "SELECT nombre, tipo, potencia_watts, estado FROM Dispositivos ORDER BY id;";

    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            RegistroDispositivo r;
            const unsigned char* nom = sqlite3_column_text(stmt, 0);
            const unsigned char* tip = sqlite3_column_text(stmt, 1);
            const unsigned char* est = sqlite3_column_text(stmt, 3);
            r.nombre   = nom ? reinterpret_cast<const char*>(nom) : "";
            r.tipo     = tip ? reinterpret_cast<const char*>(tip) : "";
            r.potencia = sqlite3_column_double(stmt, 2);
            r.estado   = est ? reinterpret_cast<const char*>(est) : "Apagado";
            lista.push_back(r);
        }
        sqlite3_finalize(stmt);
    }
    return lista;
}

void BaseDatos::actualizarEstado(const std::string& nombre, const std::string& estado) {
    sqlite3_stmt* stmt;
    const char* query = "UPDATE Dispositivos SET estado = ? WHERE nombre = ?;";
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, estado.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, nombre.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}

bool BaseDatos::existeDispositivo(const std::string& nombre) {
    sqlite3_stmt* stmt;
    const char* query = "SELECT 1 FROM Dispositivos WHERE lower(nombre) = lower(?);";
    bool existe = false;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, SQLITE_TRANSIENT);
        existe = (sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
    }
    return existe;
}
