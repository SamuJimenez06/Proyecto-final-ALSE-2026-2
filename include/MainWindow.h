/**
 * @file MainWindow.h
 * @brief Ventana principal: dispositivos, rutinas, historial de eventos y consumo con gráficas.
 * @author Samuel Jiménez, Brayan Villamil
 * @date 2026-11
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GestorDomotica.h"
#include "BaseDatos.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Panel de control del sistema domótico.
 *
 * Organiza la aplicación en cuatro pestañas:
 * - **Dispositivos:** registro, encendido/apagado y simulación de fallos.
 * - **Rutinas:** Modo Noche, Salir de Casa y Buenos Días.
 * - **Historial de Eventos:** acciones guardadas en la base de datos.
 * - **Consumo y Gráficas:** estimación de kWh por dispositivo.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Crea la ventana, abre la base de datos y carga los dispositivos guardados.
     * @param parent Widget padre (normalmente nullptr).
     */
    MainWindow(QWidget *parent = nullptr);

    /// Libera la interfaz generada por Qt Designer.
    ~MainWindow();

    /// Actualiza la lista de dispositivos, el historial y la gráfica de consumo.
    void refrescarTodo();

private slots:
    /// Enciende el dispositivo seleccionado (bloqueado si está en fallo).
    void on_btnEncender_clicked();

    /// Apaga el dispositivo seleccionado (también lo reinicia si estaba en fallo).
    void on_btnApagar_clicked();

    /// Pone en estado de fallo el dispositivo seleccionado.
    void on_btnSimularFallo_clicked();

    /// Valida el formulario y registra un nuevo dispositivo.
    void on_btnRegistrar_clicked();

    /// Rutina: apaga luces y cierra persianas (los sensores siguen activos).
    void on_btnModoNoche_clicked();

    /// Rutina: apaga todos los dispositivos.
    void on_btnSalirCasa_clicked();

    /// Rutina: enciende las luces que no estén en fallo.
    void on_btnBuenosDias_clicked();

    /// Recalcula el consumo con las horas ingresadas por el usuario.
    void on_btnCalcularConsumo_clicked();

private:
    Ui::MainWindow *ui;       ///< Interfaz diseñada en MainWindow.ui.
    GestorDomotica gestor;    ///< Dispositivos en memoria.
    BaseDatos bd;             ///< Conexión a la base de datos.

    /// Muestra cada dispositivo con su tipo, estado (con color), potencia y detalle.
    void actualizarListaDispositivos();

    /// Carga los últimos 50 eventos de la base de datos en la tabla.
    void actualizarTablaEventos();

    /// Calcula el consumo de cada dispositivo y redibuja la gráfica de barras.
    void actualizarConsumoYGraficas();
};

#endif
