/**
 * @file Sensor.h
 * @brief Clase derivada que modela un sensor domótico.
 * @author Samuel Jiménez, Brayan Villamil
 * @date 2026-11
 */

#ifndef SENSOR_H
#define SENSOR_H

#include "Dispositivo.h"
#include <random>
#include <sstream>
#include <iomanip>

class Sensor : public Dispositivo {
private:
    double ultimaLectura;
    bool tieneLectura;

    static double medir() {
        static std::mt19937 generador(std::random_device{}());
        std::uniform_real_distribution<double> rango(20.0, 26.0);
        return rango(generador);
    }

public:
    Sensor(int id, std::string nombre, double potencia)
        : Dispositivo(id, nombre, potencia), ultimaLectura(0.0), tieneLectura(false) {}

    void encender() override {
        if (estado != Estado::Fallo) {
            estado = Estado::Encendido;
            leerValor();
        }
    }

    void apagar() override {
        estado = Estado::Apagado;
    }

    std::string getTipo() const override {
        return "Sensor";
    }

    std::string getDetalle() const override {
        if (estado == Estado::Fallo) return "Sin señal";
        if (estado != Estado::Encendido || !tieneLectura) return "Inactivo";
        std::ostringstream texto;
        texto << "Lectura " << std::fixed << std::setprecision(1) << ultimaLectura;
        return texto.str();
    }

    double leerValor() {
        if (estado == Estado::Encendido) {
            ultimaLectura = medir();
            tieneLectura = true;
        }
        return ultimaLectura;
    }
};

#endif
