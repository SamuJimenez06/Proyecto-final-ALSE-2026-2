/**
 * @file GestorDomotica.h
 * @brief Administrador de la colección en memoria de dispositivos domóticos.
 */

#ifndef GESTOR_DOMOTICA_H
#define GESTOR_DOMOTICA_H

#include <vector>
#include "Dispositivo.h"

class GestorDomotica {
private:
    std::vector<Dispositivo*> dispositivos;

public:
    GestorDomotica() {}

    ~GestorDomotica() {
        for (auto d : dispositivos) {
            delete d;
        }
        dispositivos.clear();
    }

    void agregarDispositivo(Dispositivo* d) {
        dispositivos.push_back(d);
    }

    std::vector<Dispositivo*>& obtenerDispositivos() {
        return dispositivos;
    }
};

#endif
