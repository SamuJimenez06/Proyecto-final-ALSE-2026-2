/**
 * @file Persiana.h
 * @brief Clase derivada que modela una persiana motorizada.
 * @author Samuel Jiménez, Brayan Villamil
 * @date 2026-11
 */

#ifndef PERSIANA_H
#define PERSIANA_H

#include "Dispositivo.h"

class Persiana : public Dispositivo {
private:
    int apertura;

public:
    Persiana(int id, std::string nombre, double potencia)
        : Dispositivo(id, nombre, potencia), apertura(0) {}

    void encender() override {
        if (estado != Estado::Fallo) {
            estado = Estado::Encendido;
            apertura = 100;
        }
    }

    void apagar() override {
        estado = Estado::Apagado;
        apertura = 0;
    }

    std::string getTipo() const override {
        return "Persiana";
    }

    std::string getDetalle() const override {
        if (estado == Estado::Fallo) return "Motor bloqueado";
        if (apertura == 0) return "Cerrada";
        return "Abierta " + std::to_string(apertura) + "%";
    }

    void setApertura(int porcentaje) {
        if (estado == Estado::Fallo) return;
        if (porcentaje >= 0 && porcentaje <= 100) {
            apertura = porcentaje;
            estado = (porcentaje > 0) ? Estado::Encendido : Estado::Apagado;
        }
    }

    int getApertura() const { return apertura; }
};

#endif
