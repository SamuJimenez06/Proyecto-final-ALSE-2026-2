/**
 * @file Dispositivo.h
 * @brief Definición de la clase base abstracta Dispositivo y del enumerador Estado.
 * @author Samuel Jiménez, Brayan Villamil
 * @date 2026-11
 */

#ifndef DISPOSITIVO_H
#define DISPOSITIVO_H

#include <string>

/**
 * @enum Estado
 * @brief Estados operativos posibles de un dispositivo domótico.
 */
enum class Estado {
    Encendido,  ///< El dispositivo está activo.
    Apagado,    ///< El dispositivo está inactivo.
    Fallo       ///< El dispositivo presenta una falla y no puede encenderse.
};

/**
 * @class Dispositivo
 * @brief Clase base abstracta para todos los dispositivos del hogar.
 *
 * Define la interfaz común (encender, apagar, tipo y detalle) que cada
 * dispositivo concreto (Luz, Persiana, Sensor) implementa a su manera.
 * Gracias al polimorfismo, el GestorDomotica y la interfaz gráfica pueden
 * trabajar con cualquier dispositivo a través de un puntero a Dispositivo.
 */
class Dispositivo {
protected:
    int id;                 ///< Identificador del dispositivo.
    std::string nombre;     ///< Nombre que le asigna el usuario (ej. "Luz Sala").
    double potenciaWatts;   ///< Potencia nominal en vatios (W), usada para estimar el consumo.
    Estado estado;          ///< Estado actual del dispositivo.

public:
    /**
     * @brief Construye un dispositivo apagado.
     * @param id Identificador del dispositivo.
     * @param nombre Nombre del dispositivo.
     * @param potencia Potencia nominal en vatios.
     */
    Dispositivo(int id, std::string nombre, double potencia)
        : id(id), nombre(nombre), potenciaWatts(potencia), estado(Estado::Apagado) {}

    /// Destructor virtual: garantiza que se destruya correctamente la clase derivada.
    virtual ~Dispositivo() {}

    /**
     * @brief Activa el dispositivo. Cada tipo define qué significa "encender".
     * @note No tiene efecto si el dispositivo está en Estado::Fallo.
     */
    virtual void encender() = 0;

    /**
     * @brief Desactiva el dispositivo. También reinicia un dispositivo en fallo.
     */
    virtual void apagar() = 0;

    /**
     * @brief Devuelve el tipo de dispositivo.
     * @return "Luz", "Persiana" o "Sensor".
     */
    virtual std::string getTipo() const = 0;

    /**
     * @brief Información propia de cada tipo de dispositivo (brillo, apertura, lectura...).
     * @return Texto descriptivo para mostrar en la interfaz.
     */
    virtual std::string getDetalle() const { return ""; }

    /// @return Estado actual del dispositivo.
    Estado getEstado() const { return estado; }

    /**
     * @brief Cambia el estado directamente (se usa para simular fallos o restaurar desde la BD).
     * @param e Nuevo estado.
     */
    void setEstado(Estado e) { estado = e; }

    /// @return Nombre del dispositivo.
    std::string getNombre() const { return nombre; }

    /// @return Potencia nominal en vatios.
    double getPotencia() const { return potenciaWatts; }

    /// @return Identificador del dispositivo.
    int getId() const { return id; }
};

#endif
