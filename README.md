# Gestor de Domótica para Hogares — Proyecto Final ALSE

Aplicación de escritorio en **C++ con Qt5** para controlar dispositivos domóticos (luces, persianas y sensores). Permite registrar dispositivos, encenderlos y apagarlos desde una interfaz gráfica, validar su estado, ejecutar rutinas y estimar el consumo energético. Toda la información se guarda en una base de datos **SQLite**.

**Autores:** Samuel Jiménez, Brayan Villamil
**Curso:** Algoritmos en Sistemas Electrónicos (ALSE) — 2026-2

---

## Funcionalidades

### Requerimientos funcionales

| # | Requerimiento | Implementación |
|---|---|---|
| 1 | Autenticación de usuarios | Ventana de inicio de sesión (`LoginDialog`). Valida contra la tabla `Usuarios`; las contraseñas se guardan cifradas con SHA-256. Máximo 3 intentos. |
| 2 | Registro de dispositivos | Formulario con nombre, tipo (Luz, Persiana, Sensor) y potencia. Valida campos vacíos, potencias inválidas y nombres repetidos. Los dispositivos se guardan en la BD y se cargan al iniciar. |
| 3 | Activación y desactivación desde la GUI | Botones **Encender** y **Apagar** sobre el dispositivo seleccionado. |
| 4 | Validación de estados | Estados `Encendido`, `Apagado` y `Fallo` (con colores verde, gris y rojo). Un dispositivo en fallo no puede encenderse hasta que se apague (reinicio). Botón **Simular Fallo** para probarlo. |
| 5 | Registro de eventos en la BD | Cada acción se guarda en la tabla `Eventos` con fecha y hora, y se muestra en la pestaña **Historial de Eventos**. |

### Requerimientos no funcionales

- **Interfaz amigable:** ventana organizada en pestañas (Dispositivos, Rutinas, Historial, Consumo y Gráficas) y mensajes de aviso claros.
- **Código modular y documentado:** cabeceras en `include/`, implementación en `src/` y comentarios en formato Doxygen.
- **Rutinas de dispositivos:**
  - **Modo Noche:** apaga luces y cierra persianas; los sensores siguen activos.
  - **Salir de Casa:** apaga todos los dispositivos.
  - **Buenos Días:** enciende las luces que no estén en fallo.

### Adicionales

- **Estimación de consumo energético:** `consumo (kWh) = potencia (W) × horas de uso / 1000`. El usuario elige las horas; se muestra el consumo por dispositivo, el total estimado y el total de los dispositivos encendidos.
- **Panel de control con gráficas:** gráfica de barras (QtCharts) con el consumo de cada dispositivo.

---

## Diseño orientado a objetos

```
             Dispositivo  (clase abstracta)
     ┌────────────┼────────────┐
    Luz        Persiana      Sensor
 (brillo %)  (apertura %)  (lectura)
```

- **`Dispositivo`** es una clase abstracta con métodos virtuales puros: `encender()`, `apagar()` y `getTipo()`, además del método virtual `getDetalle()`.
- **Polimorfismo:** cada clase derivada implementa esos métodos a su manera:
  - `Luz`: al encender recupera su último brillo; al apagar el brillo pasa a 0 %.
  - `Persiana`: "encender" la abre al 100 % y "apagar" la cierra.
  - `Sensor`: al encender toma una lectura (simulada); al apagar deja de medir.
- **`GestorDomotica`** guarda todos los dispositivos en un `std::vector<Dispositivo*>` y los trata de forma uniforme a través de la clase base.
- **`BaseDatos`** encapsula todo el acceso a SQLite usando sentencias preparadas, para evitar inyección SQL.
- **`MainWindow`** y **`LoginDialog`** forman la interfaz gráfica.

### Base de datos (`domotica.db`)

| Tabla | Campos |
|---|---|
| `Usuarios` | `id`, `usuario`, `clave_hash` |
| `Dispositivos` | `id`, `nombre`, `tipo`, `potencia_watts`, `estado` |
| `Eventos` | `id`, `dispositivo`, `accion`, `fecha` |

El archivo `domotica.db` se crea automáticamente junto al ejecutable (dentro de `build/`) la primera vez que se ejecuta el programa.

---

## Estructura del proyecto

```
Proyecto-final-ALSE-2026-2/
├── include/            Cabeceras (.h)
│   ├── Dispositivo.h   Clase base abstracta y enum Estado
│   ├── Luz.h           Luz con brillo regulable
│   ├── Persiana.h      Persiana con porcentaje de apertura
│   ├── Sensor.h        Sensor con lectura simulada
│   ├── GestorDomotica.h
│   ├── BaseDatos.h
│   ├── LoginDialog.h
│   └── MainWindow.h
├── src/                Implementación (.cpp) e interfaz (.ui)
│   ├── main.cpp
│   ├── MainWindow.cpp
│   ├── MainWindow.ui
│   └── BaseDatos.cpp
├── docs/html/          Documentación generada con Doxygen
├── CMakeLists.txt
├── Doxyfile
└── README.md
```

---

## Requisitos

- Ubuntu 22.04 (o WSL con Ubuntu)
- CMake 3.10 o superior y un compilador de C++ (g++)
- Qt5 (Widgets) y QtCharts
- SQLite3

Instalación de dependencias:

```bash
sudo apt update
sudo apt install build-essential cmake qtbase5-dev libqt5charts5-dev libsqlite3-dev
```

---

## Compilación y ejecución

```bash
git clone https://github.com/SamuJimenez06/Proyecto-final-ALSE-2026-2.git
cd Proyecto-final-ALSE-2026-2
mkdir build && cd build
cmake ..
make
./ProyectoFinal
```

### Usuario de prueba

| Usuario | Contraseña |
|---|---|
| `admin` | `1234` |

---

## Uso rápido

1. Inicie sesión con el usuario de prueba.
2. En **Dispositivos**, escriba un nombre, elija el tipo, ingrese la potencia y presione **Registrar Dispositivo**.
3. Seleccione un dispositivo de la lista y use **Encender**, **Apagar** o **Simular Fallo**.
4. En **Rutinas**, ejecute Modo Noche, Salir de Casa o Buenos Días.
5. En **Historial de Eventos**, consulte todas las acciones con fecha y hora.
6. En **Consumo y Gráficas**, ingrese las horas de uso y presione **Calcular kWh**.

---

## Documentación

La documentación del código está en `docs/html/index.html`. Para generarla de nuevo:

```bash
sudo apt install doxygen graphviz
doxygen Doxyfile
```
