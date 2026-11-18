/**
 * @file Persiana.h
 * @brief Clase derivada que modela una persiana domótica.
 */

#ifndef PERSIANA_H
#define PERSIANA_H

#include "Dispositivo.h"

class Persiana : public Dispositivo {
public:
    Persiana(int id, std::string nombre, double potencia) : Dispositivo(id, nombre, potencia) {}

    void encender() override {
        if (estado != Estado::Fallo) {
            estado = Estado::Encendido;
        }
    }

    void apagar() override {
        estado = Estado::Apagado;
    }

    std::string getTipo() const override {
        return "Persiana";
    }
};

#endif
