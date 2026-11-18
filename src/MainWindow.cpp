/**
 * @file MainWindow.cpp
 * @brief Implementación de la lógica de la ventana principal, eventos, rutinas y gráficas.
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Luz.h"
#include "Persiana.h"
#include "Sensor.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

static std::string estadoATexto(Estado e) {
    if (e == Estado::Encendido) return "Encendido";
    if (e == Estado::Fallo)     return "Fallo";
    return "Apagado";
}

static Dispositivo* crearDispositivo(const std::string& tipo, const std::string& nombre, double potencia) {
    if (tipo == "Luz")      return new Luz(0, nombre, potencia);
    if (tipo == "Persiana") return new Persiana(0, nombre, potencia);
    if (tipo == "Sensor")   return new Sensor(0, nombre, potencia);
    return nullptr;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    bd.abrirConexion();
    bd.inicializarTablas();

    for (const auto& r : bd.cargarDispositivos()) {
        Dispositivo* d = crearDispositivo(r.tipo, r.nombre, r.potencia);
        if (!d) continue;
        if (r.estado == "Encendido")  d->encender();
        else if (r.estado == "Fallo") d->setEstado(Estado::Fallo);
        gestor.agregarDispositivo(d);
    }

    ui->tableWidgetEventos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetEventos->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->txtHorasUso->setText("5.0");

    refrescarTodo();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::refrescarTodo() {
    actualizarListaDispositivos();
    actualizarTablaEventos();
    actualizarConsumoYGraficas();
}

void MainWindow::actualizarListaDispositivos() {
    ui->listWidgetDispositivos->clear();
    for (auto d : gestor.obtenerDispositivos()) {
        QString estadoStr = "APAGADO";
        QColor colorItem(100, 100, 100);

        if (d->getEstado() == Estado::Encendido) {
            estadoStr = "ENCENDIDO";
            colorItem = QColor(0, 128, 0);
        } else if (d->getEstado() == Estado::Fallo) {
            estadoStr = "FALLO";
            colorItem = QColor(200, 0, 0);
        }

        QString texto = QString::fromStdString(d->getNombre() + " [" + d->getTipo() + "] - " + estadoStr.toStdString() + " - " + std::to_string((int)d->getPotencia()) + " W");
        QListWidgetItem* item = new QListWidgetItem(texto);
        item->setForeground(colorItem);
        ui->listWidgetDispositivos->addItem(item);
    }
}

void MainWindow::actualizarTablaEventos() {
    ui->tableWidgetEventos->setRowCount(0);
    sqlite3_stmt* stmt;
    std::string query = "SELECT dispositivo, accion, fecha FROM Eventos ORDER BY id DESC LIMIT 50;";

    if (sqlite3_prepare_v2(bd.getConexion(), query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        int row = 0;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            ui->tableWidgetEventos->insertRow(row);

            const unsigned char* disp = sqlite3_column_text(stmt, 0);
            const unsigned char* acc = sqlite3_column_text(stmt, 1);
            const unsigned char* fec = sqlite3_column_text(stmt, 2);

            ui->tableWidgetEventos->setItem(row, 0, new QTableWidgetItem(disp ? QString::fromUtf8((const char*)disp) : ""));
            ui->tableWidgetEventos->setItem(row, 1, new QTableWidgetItem(acc ? QString::fromUtf8((const char*)acc) : ""));
            ui->tableWidgetEventos->setItem(row, 2, new QTableWidgetItem(fec ? QString::fromUtf8((const char*)fec) : ""));
            row++;
        }
        sqlite3_finalize(stmt);
    }
}

void MainWindow::actualizarConsumoYGraficas() {
    bool ok;
    double horasUso = ui->txtHorasUso->text().toDouble(&ok);
    if (!ok || horasUso < 0) horasUso = 5.0;

    double totalEstimado = 0.0;
    double totalEncendidos = 0.0;
    double maxKWh = 0.0;

    QString textoConsumo = "=== CONSUMO ENERGÉTICO (" + QString::number(horasUso) + " h de uso) ===\n\n";

    QBarSet* set = new QBarSet("Consumo (kWh)");
    QStringList categorias;

    for (auto d : gestor.obtenerDispositivos()) {
        double kWh = (d->getPotencia() * horasUso) / 1000.0;
        totalEstimado += kWh;
        if (d->getEstado() == Estado::Encendido) totalEncendidos += kWh;

        textoConsumo += QString::fromStdString(d->getNombre() + " (" + d->getTipo() + "):\n") +
                        "  Potencia: " + QString::number(d->getPotencia()) + " W\n" +
                        "  Consumo: " + QString::number(kWh, 'f', 3) + " kWh\n\n";

        *set << kWh;
        categorias << QString::fromStdString(d->getNombre());
        if (kWh > maxKWh) maxKWh = kWh;
    }

    textoConsumo += "---------------------------------\n";
    textoConsumo += "TOTAL ESTIMADO (todos): " + QString::number(totalEstimado, 'f', 3) + " kWh\n";
    textoConsumo += "TOTAL DISPOSITIVOS ENCENDIDOS: " + QString::number(totalEncendidos, 'f', 3) + " kWh";
    ui->txtConsumoTexto->setText(textoConsumo);

    QBarSeries* series = new QBarSeries();
    series->append(set);

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Consumo por Dispositivo (kWh)");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(false);

    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categorias);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    axisY->setTitleText("kWh");
    axisY->setRange(0, maxKWh > 0 ? maxKWh * 1.2 : 1.0);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView* chartView = ui->layoutGrafica->findChild<QChartView*>();
    if (!chartView) {
        if (!ui->layoutGrafica->layout()) {
            new QVBoxLayout(ui->layoutGrafica);
        }
        chartView = new QChartView(ui->layoutGrafica);
        chartView->setRenderHint(QPainter::Antialiasing);
        ui->layoutGrafica->layout()->addWidget(chartView);
    }

    QChart* graficaAnterior = chartView->chart();
    chartView->setChart(chart);
    delete graficaAnterior;
}

void MainWindow::on_btnCalcularConsumo_clicked() {
    actualizarConsumoYGraficas();
}

void MainWindow::on_btnEncender_clicked() {
    int fila = ui->listWidgetDispositivos->currentRow();
    if (fila < 0) {
        QMessageBox::information(this, "Aviso", "Seleccione un dispositivo de la lista.");
        return;
    }
    auto& lista = gestor.obtenerDispositivos();
    if (fila < (int)lista.size()) {
        Dispositivo* d = lista[fila];
        if (d->getEstado() == Estado::Fallo) {
            QMessageBox::warning(this, "Error", "Dispositivo en fallo. No se puede encender.");
            return;
        }
        d->encender();
        bd.registrarEvento(d->getNombre(), "Encendido");
        bd.actualizarEstado(d->getNombre(), estadoATexto(d->getEstado()));
        refrescarTodo();
    }
}

void MainWindow::on_btnApagar_clicked() {
    int fila = ui->listWidgetDispositivos->currentRow();
    if (fila < 0) {
        QMessageBox::information(this, "Aviso", "Seleccione un dispositivo de la lista.");
        return;
    }
    auto& lista = gestor.obtenerDispositivos();
    if (fila < (int)lista.size()) {
        lista[fila]->apagar();
        bd.registrarEvento(lista[fila]->getNombre(), "Apagado");
        bd.actualizarEstado(lista[fila]->getNombre(), estadoATexto(lista[fila]->getEstado()));
        refrescarTodo();
    }
}

void MainWindow::on_btnSimularFallo_clicked() {
    int fila = ui->listWidgetDispositivos->currentRow();
    if (fila < 0) {
        QMessageBox::information(this, "Aviso", "Seleccione un dispositivo de la lista.");
        return;
    }
    auto& lista = gestor.obtenerDispositivos();
    if (fila < (int)lista.size()) {
        lista[fila]->setEstado(Estado::Fallo);
        bd.registrarEvento(lista[fila]->getNombre(), "Fallo Simulado");
        bd.actualizarEstado(lista[fila]->getNombre(), "Fallo");
        refrescarTodo();
    }
}

void MainWindow::on_btnRegistrar_clicked() {
    QString nombreQ = ui->txtNombre->text().trimmed();
    QString potenciaQ = ui->txtPotencia->text().trimmed();
    QString tipo = ui->comboTipo->currentText();

    if (nombreQ.isEmpty()) {
        QMessageBox::warning(this, "Datos incompletos", "Escriba el nombre del dispositivo.");
        return;
    }
    bool ok;
    double potencia = potenciaQ.toDouble(&ok);
    if (!ok || potencia <= 0) {
        QMessageBox::warning(this, "Potencia inválida", "La potencia debe ser mayor que 0.");
        return;
    }

    std::string nombre = nombreQ.toStdString();
    std::string tipoStd = tipo.toStdString();

    if (bd.existeDispositivo(nombre)) {
        QMessageBox::warning(this, "Nombre repetido", "Ya existe un dispositivo con ese nombre.");
        return;
    }

    Dispositivo* nuevo = crearDispositivo(tipoStd, nombre, potencia);
    if (!nuevo) return;

    bd.guardarDispositivo(nombre, tipoStd, potencia);
    bd.registrarEvento(nombre, "Registrado");
    gestor.agregarDispositivo(nuevo);

    ui->txtNombre->clear();
    ui->txtPotencia->clear();
    refrescarTodo();
}

void MainWindow::on_btnModoNoche_clicked() {
    for (auto d : gestor.obtenerDispositivos()) {
        if (d->getTipo() == "Sensor") continue;
        d->apagar();
        bd.registrarEvento(d->getNombre(), "Modo Noche (Apagado)");
        bd.actualizarEstado(d->getNombre(), estadoATexto(d->getEstado()));
    }
    refrescarTodo();
}

void MainWindow::on_btnSalirCasa_clicked() {
    for (auto d : gestor.obtenerDispositivos()) {
        d->apagar();
        bd.registrarEvento(d->getNombre(), "Salir de Casa (Apagado)");
        bd.actualizarEstado(d->getNombre(), estadoATexto(d->getEstado()));
    }
    refrescarTodo();
}

void MainWindow::on_btnBuenosDias_clicked() {
    for (auto d : gestor.obtenerDispositivos()) {
        if (d->getTipo() == "Luz" && d->getEstado() != Estado::Fallo) {
            d->encender();
            bd.registrarEvento(d->getNombre(), "Buenos Días (Encendido)");
            bd.actualizarEstado(d->getNombre(), estadoATexto(d->getEstado()));
        }
    }
    refrescarTodo();
}
