/**
 * @file GestorDomotica.h
 * @brief Administrador de la colección en memoria de dispositivos domóticos.
 * @author Samuel Jiménez, Brayan Villamil
 * @date 2026-11
 */

#ifndef GESTOR_DOMOTICA_H
#define GESTOR_DOMOTICA_H

#include <vector>
#include "Dispositivo.h"

/**
 * @class GestorDomotica
 * @brief Guarda y administra todos los dispositivos del hogar.
 *
 * Almacena punteros a la clase base Dispositivo, lo que permite manejar
 * luces, persianas y sensores en una sola lista (polimorfismo).
 * El gestor es dueño de los objetos y los libera al destruirse.
 */
class GestorDomotica {
private:
    std::vector<Dispositivo*> dispositivos;   ///< Lista de dispositivos registrados.

public:
    /// Crea un gestor sin dispositivos.
    GestorDomotica() {}

    /// Libera la memoria de todos los dispositivos registrados.
    ~GestorDomotica() {
        for (auto d : dispositivos) {
            delete d;
        }
        dispositivos.clear();
    }

    /**
     * @brief Agrega un dispositivo a la lista. El gestor pasa a ser su dueño.
     * @param d Puntero a un dispositivo creado con new (Luz, Persiana o Sensor).
     */
    void agregarDispositivo(Dispositivo* d) {
        dispositivos.push_back(d);
    }

    /**
     * @brief Acceso a la lista de dispositivos.
     * @return Referencia al vector de dispositivos.
     */
    std::vector<Dispositivo*>& obtenerDispositivos() {
        return dispositivos;
    }
};

#endif
