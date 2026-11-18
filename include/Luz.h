/**
 * @file Luz.h
 * @brief Clase derivada que modela una luz domótica.
 */

#ifndef LUZ_H
#define LUZ_H

#include "Dispositivo.h"

class Luz : public Dispositivo {
public:
    Luz(int id, std::string nombre, double potencia) : Dispositivo(id, nombre, potencia) {}

    void encender() override {
        if (estado != Estado::Fallo) {
            estado = Estado::Encendido;
        }
    }

    void apagar() override {
        estado = Estado::Apagado;
    }

    std::string getTipo() const override {
        return "Luz";
    }
};

#endif
