/**
 * @file Persiana.h
 * @brief Clase derivada que modela una persiana motorizada.
 * @author Samuel Jiménez, Brayan Villamil
 * @date 2026-11
 */

#ifndef PERSIANA_H
#define PERSIANA_H

#include "Dispositivo.h"

/**
 * @class Persiana
 * @brief Persiana motorizada: "encender" la abre y "apagar" la cierra.
 */
class Persiana : public Dispositivo {
private:
    int apertura;   ///< Porcentaje de apertura (0 = cerrada, 100 = abierta).

public:
    /**
     * @brief Crea una persiana cerrada.
     * @param id Identificador.
     * @param nombre Nombre de la persiana.
     * @param potencia Potencia del motor en vatios.
     */
    Persiana(int id, std::string nombre, double potencia)
        : Dispositivo(id, nombre, potencia), apertura(0) {}

    /// Abre la persiana por completo (si no está en fallo).
    void encender() override {
        if (estado != Estado::Fallo) {
            estado = Estado::Encendido;
            apertura = 100;
        }
    }

    /// Cierra la persiana por completo.
    void apagar() override {
        estado = Estado::Apagado;
        apertura = 0;
    }

    /// @return "Persiana".
    std::string getTipo() const override {
        return "Persiana";
    }

    /// @return "Abierta 100%", "Cerrada" o el aviso de falla.
    std::string getDetalle() const override {
        if (estado == Estado::Fallo) return "Motor bloqueado";
        if (apertura == 0) return "Cerrada";
        return "Abierta " + std::to_string(apertura) + "%";
    }

    /**
     * @brief Abre la persiana parcialmente.
     * @param porcentaje Apertura deseada (0 a 100). Valores fuera de rango se ignoran.
     */
    void setApertura(int porcentaje) {
        if (estado == Estado::Fallo) return;
        if (porcentaje >= 0 && porcentaje <= 100) {
            apertura = porcentaje;
            estado = (porcentaje > 0) ? Estado::Encendido : Estado::Apagado;
        }
    }

    /// @return Porcentaje de apertura actual.
    int getApertura() const { return apertura; }
};

#endif
