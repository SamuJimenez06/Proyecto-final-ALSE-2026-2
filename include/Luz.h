/**
 * @file Luz.h
 * @brief Clase derivada que modela una luz domótica con brillo regulable.
 * @author Samuel Jiménez, Brayan Villamil
 * @date 2026-11
 */

#ifndef LUZ_H
#define LUZ_H

#include "Dispositivo.h"

class Luz : public Dispositivo {
private:
    int brillo;
    int brilloGuardado;

public:
    Luz(int id, std::string nombre, double potencia)
        : Dispositivo(id, nombre, potencia), brillo(0), brilloGuardado(100) {}

    void encender() override {
        if (estado != Estado::Fallo) {
            estado = Estado::Encendido;
            brillo = brilloGuardado;
        }
    }

    void apagar() override {
        estado = Estado::Apagado;
        brillo = 0;
    }

    std::string getTipo() const override {
        return "Luz";
    }

    std::string getDetalle() const override {
        if (estado == Estado::Fallo) return "Sin respuesta";
        return "Brillo " + std::to_string(brillo) + "%";
    }

    void setBrillo(int valor) {
        if (valor >= 0 && valor <= 100) {
            brilloGuardado = valor;
            if (estado == Estado::Encendido) brillo = valor;
        }
    }

    int getBrillo() const { return brillo; }
};

#endif
