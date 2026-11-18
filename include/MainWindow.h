#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GestorDomotica.h"
#include "BaseDatos.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void refrescarTodo();

private slots:
    void on_btnEncender_clicked();
    void on_btnApagar_clicked();
    void on_btnSimularFallo_clicked();
    void on_btnRegistrar_clicked();

//Modos noche,salir de casa,dia

    void on_btnModoNoche_clicked();
    void on_btnSalirCasa_clicked();
    void on_btnBuenosDias_clicked();
//Boton Para calcular el consumo 

    void on_btnCalcularConsumo_clicked();

private:
    Ui::MainWindow *ui;
    GestorDomotica gestor;
    BaseDatos bd;

    void actualizarListaDispositivos();
    void actualizarTablaEventos();
    void actualizarConsumoYGraficas();
};

#endif
