/**
 * @file Sensor.h
 * @brief Clase derivada que modela un sensor domótico (temperatura, humo, movimiento...).
 * @author Samuel Jiménez, Brayan Villamil
 * @date 2026-11
 */

#ifndef SENSOR_H
#define SENSOR_H

#include "Dispositivo.h"
#include <random>
#include <sstream>
#include <iomanip>

/**
 * @class Sensor
 * @brief Sensor que toma una lectura al activarse.
 *
 * Como el proyecto no está conectado a hardware real, la lectura se simula
 * con un valor aleatorio dentro de un rango típico.
 */
class Sensor : public Dispositivo {
private:
    double ultimaLectura;   ///< Último valor medido por el sensor.
    bool tieneLectura;      ///< Indica si ya se tomó al menos una lectura.

    /// Genera una lectura simulada entre 20.0 y 26.0.
    static double medir() {
        static std::mt19937 generador(std::random_device{}());
        std::uniform_real_distribution<double> rango(20.0, 26.0);
        return rango(generador);
    }

public:
    /**
     * @brief Crea un sensor inactivo.
     * @param id Identificador.
     * @param nombre Nombre del sensor.
     * @param potencia Potencia en vatios.
     */
    Sensor(int id, std::string nombre, double potencia)
        : Dispositivo(id, nombre, potencia), ultimaLectura(0.0), tieneLectura(false) {}

    /// Activa el sensor y toma una primera lectura (si no está en fallo).
    void encender() override {
        if (estado != Estado::Fallo) {
            estado = Estado::Encendido;
            leerValor();
        }
    }

    /// Desactiva el sensor; deja de reportar lecturas.
    void apagar() override {
        estado = Estado::Apagado;
    }

    /// @return "Sensor".
    std::string getTipo() const override {
        return "Sensor";
    }

    /// @return La última lectura, "Inactivo" o el aviso de falla.
    std::string getDetalle() const override {
        if (estado == Estado::Fallo) return "Sin señal";
        if (estado != Estado::Encendido || !tieneLectura) return "Inactivo";
        std::ostringstream texto;
        texto << "Lectura " << std::fixed << std::setprecision(1) << ultimaLectura;
        return texto.str();
    }

    /**
     * @brief Toma una nueva lectura. Solo funciona si el sensor está encendido.
     * @return Valor medido, o la última lectura si el sensor no está activo.
     */
    double leerValor() {
        if (estado == Estado::Encendido) {
            ultimaLectura = medir();
            tieneLectura = true;
        }
        return ultimaLectura;
    }
};

#endif
