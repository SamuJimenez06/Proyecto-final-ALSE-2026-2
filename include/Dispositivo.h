/**
 * @file Dispositivo.h
 * @brief Definición de la clase base abstracta Dispositivo y del enumerador Estado.
 * @author Samuel Jiménez, Brayan Villamil
 * @date 2026-11
 */

#ifndef DISPOSITIVO_H
#define DISPOSITIVO_H

#include <string>

enum class Estado {
    Encendido,
    Apagado,
    Fallo
};

class Dispositivo {
protected:
    int id;
    std::string nombre;
    double potenciaWatts;
    Estado estado;

public:
    Dispositivo(int id, std::string nombre, double potencia)
        : id(id), nombre(nombre), potenciaWatts(potencia), estado(Estado::Apagado) {}

    virtual ~Dispositivo() {}

    virtual void encender() = 0;
    virtual void apagar() = 0;
    virtual std::string getTipo() const = 0;
    virtual std::string getDetalle() const { return ""; }

    Estado getEstado() const { return estado; }
    void setEstado(Estado e) { estado = e; }
    std::string getNombre() const { return nombre; }
    double getPotencia() const { return potenciaWatts; }
    int getId() const { return id; }
};

#endif
