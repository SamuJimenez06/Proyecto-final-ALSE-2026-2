/**
 * @file Luz.h
 * @brief Clase derivada que modela una luz domótica con brillo regulable.
 * @author Samuel Jiménez, Brayan Villamil
 * @date 2026-11
 */

#ifndef LUZ_H
#define LUZ_H

#include "Dispositivo.h"

/**
 * @class Luz
 * @brief Luz del hogar. Al encenderse recupera su último nivel de brillo.
 */
class Luz : public Dispositivo {
private:
    int brillo;            ///< Nivel de brillo actual (0 a 100 %). 0 cuando está apagada.
    int brilloGuardado;    ///< Último brillo usado, para restaurarlo al encender.

public:
    /**
     * @brief Crea una luz apagada con brillo predeterminado del 100 %.
     * @param id Identificador.
     * @param nombre Nombre de la luz.
     * @param potencia Potencia en vatios.
     */
    Luz(int id, std::string nombre, double potencia)
        : Dispositivo(id, nombre, potencia), brillo(0), brilloGuardado(100) {}

    /// Enciende la luz con el último brillo guardado (si no está en fallo).
    void encender() override {
        if (estado != Estado::Fallo) {
            estado = Estado::Encendido;
            brillo = brilloGuardado;
        }
    }

    /// Apaga la luz; el brillo pasa a 0 pero se recuerda el anterior.
    void apagar() override {
        estado = Estado::Apagado;
        brillo = 0;
    }

    /// @return "Luz".
    std::string getTipo() const override {
        return "Luz";
    }

    /// @return Texto con el brillo actual o el aviso de falla.
    std::string getDetalle() const override {
        if (estado == Estado::Fallo) return "Sin respuesta";
        return "Brillo " + std::to_string(brillo) + "%";
    }

    /**
     * @brief Ajusta el brillo. Valores fuera del rango 0-100 se ignoran.
     * @param valor Nuevo brillo en porcentaje.
     */
    void setBrillo(int valor) {
        if (valor >= 0 && valor <= 100) {
            brilloGuardado = valor;
            if (estado == Estado::Encendido) brillo = valor;
        }
    }

    /// @return Brillo actual en porcentaje.
    int getBrillo() const { return brillo; }
};

#endif
