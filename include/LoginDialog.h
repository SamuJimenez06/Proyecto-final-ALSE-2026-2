/**
 * @file LoginDialog.h
 * @brief Ventana de inicio de sesión para el sistema domótico.
 *        Valida usuario y contraseña contra la tabla Usuarios de la base de datos.
 * @author Samuel Jiménez, Brayan Villamil
 * @date 2026-11
 */

#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "BaseDatos.h"

/**
 * @class LoginDialog
 * @brief Diálogo de inicio de sesión que se muestra antes de la ventana principal.
 *
 * Permite máximo 3 intentos fallidos; después se cierra la aplicación.
 */
class LoginDialog : public QDialog {
    Q_OBJECT
private:
    BaseDatos& bd;          ///< Base de datos donde están los usuarios
    QLineEdit *txtUsuario;
    QLineEdit *txtClave;
    QPushButton *btnIngresar;
    QLabel *lblMensaje;     ///< Muestra el error si las credenciales son incorrectas
    int intentos;           ///< Intentos fallidos

private slots:
    /// Compara usuario y contraseña con la base de datos.
    void verificarCredenciales() {
        QString usuario = txtUsuario->text().trimmed();
        QString clave = txtClave->text();

        if (usuario.isEmpty() || clave.isEmpty()) {
            lblMensaje->setText("Ingrese usuario y contraseña.");
            return;
        }

        if (bd.validarUsuario(usuario.toStdString(), clave.toStdString())) {
            accept();   // cierra el diálogo con resultado "Accepted"
        } else {
            intentos++;
            lblMensaje->setText(QString("Usuario o contraseña incorrectos (intento %1 de 3).").arg(intentos));
            txtClave->clear();
            txtClave->setFocus();
            if (intentos >= 3) {
                reject();   // demasiados intentos: se cierra el programa
            }
        }
    }

public:
    /**
     * @brief Construye el formulario de inicio de sesión.
     * @param baseDatos Base de datos donde se validan los usuarios.
     * @param parent Widget padre (normalmente nullptr).
     */
    explicit LoginDialog(BaseDatos& baseDatos, QWidget *parent = nullptr)
        : QDialog(parent), bd(baseDatos), intentos(0) {
        setWindowTitle("Iniciar Sesión - Domótica ALSE");
        setFixedSize(320, 200);

        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *lblTitulo = new QLabel("<b>Acceso al Sistema Domótico</b>", this);
        lblTitulo->setAlignment(Qt::AlignCenter);
        layout->addWidget(lblTitulo);

        txtUsuario = new QLineEdit(this);
        txtUsuario->setPlaceholderText("Usuario");
        layout->addWidget(txtUsuario);

        txtClave = new QLineEdit(this);
        txtClave->setEchoMode(QLineEdit::Password);
        txtClave->setPlaceholderText("Contraseña");
        layout->addWidget(txtClave);

        btnIngresar = new QPushButton("Ingresar", this);
        btnIngresar->setDefault(true);
        layout->addWidget(btnIngresar);

        lblMensaje = new QLabel("", this);
        lblMensaje->setStyleSheet("color: red;");
        lblMensaje->setAlignment(Qt::AlignCenter);
        lblMensaje->setWordWrap(true);
        layout->addWidget(lblMensaje);

        connect(btnIngresar, &QPushButton::clicked, this, &LoginDialog::verificarCredenciales);
        connect(txtClave, &QLineEdit::returnPressed, this, &LoginDialog::verificarCredenciales);
    }
};

#endif
