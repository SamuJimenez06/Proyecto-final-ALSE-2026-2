/**
 * @file main.cpp
 * @brief Punto de entrada: muestra primero el inicio de sesión y, si es correcto, la ventana principal.
 */

#include <QApplication>
#include "BaseDatos.h"
#include "LoginDialog.h"
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    BaseDatos bdLogin;
    bdLogin.abrirConexion();
    bdLogin.inicializarTablas();


    LoginDialog login(bdLogin);
    if (login.exec() != QDialog::Accepted) {
        return 0;
    }

    //  Login: abrir la ventana principal
    MainWindow w;
    w.show();
    return app.exec();
}
