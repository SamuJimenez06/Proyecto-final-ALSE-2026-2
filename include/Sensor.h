/**
 * @file Sensor.h
 * @brief Clase derivada que modela un sensor domótico.
 */

#ifndef SENSOR_H
#define SENSOR_H

#include "Dispositivo.h"

class Sensor : public Dispositivo {
public:
    Sensor(int id, std::string nombre, double potencia) : Dispositivo(id, nombre, potencia) {}

    void encender() override {
        if (estado != Estado::Fallo) {
            estado = Estado::Encendido;
        }
    }

    void apagar() override {
        estado = Estado::Apagado;
    }

    std::string getTipo() const override {
        return "Sensor";
    }

    double leerValor() {
        return 24.5;
    }
};

#endif
