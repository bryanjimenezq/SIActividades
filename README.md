# Proyecto Bimestral - Introducción a los Sistemas de Información

## Sistema de Gestión Médica y Análisis de KPIs (Bimestre 1)

Este repositorio contiene el desarrollo del proyecto final aplicable a la gestión hospitalaria y análisis de datos estadísticos para la toma de decisiones gerenciales.

### 📋 Descripción del Proyecto
El sistema está diseñado en dos grandes bloques para simular el funcionamiento de un Sistema de Información (SI) de salud real:
1. **Módulo Operativo (C):** Gestión en tiempo real de pacientes, médicos, autenticación de usuarios y agendamiento de citas médicas, con persistencia de datos en archivos de texto.
2. **Módulo Analítico (Google Colab / Python):** Análisis de indicadores clave de rendimiento (KPIs), analítica de datos y reportes gerenciales para la toma de decisiones utilizando entornos interactivos de Google Colab.

### 🛠️ Componentes del Sistema (Aplicados a los SI)

* **Hardware:** Computadores de escritorio o servidores en la nube (como la infraestructura de Google para ejecutar los cuadernos) donde se procesan las consultas.
* **Software:** * **Backend y Consola:** Desarrollado en Lenguaje C (`main.c`, carpetas `src` e `include`) para el control de procesos operativos locales.
  * **Analítica de Datos:** Desarrollado en Python mediante cuadernos de **Google Colab** (`KPIs_Proyecto_Bimestral.ipynb` y `Prototipos_de_MIS_Proyecto_Bimestral.ipynb`) para el procesamiento estadístico y visualización de métricas.
* **Datos:** Registros de pacientes (cédula, historial), datos de staff médico (especialidades, horarios), citas agendadas y métricas de rendimiento hospitalario.
* **Almacenamiento:** Persistencia de datos mediante archivos planos (`data_pacientes.txt`, `data_medicos.txt`, `data_citas.txt`) que emulan una base de datos relacional.
* **Procedimientos:** Algoritmos de validación de datos, flujos de agendamiento sin colisiones de horarios, y metodologías de análisis de KPIs gerenciales.
* **Usuarios:** Personal administrativo (ingreso de datos), médicos (consulta de agenda), directores de salud (visualización de reportes analíticos) y desarrolladores.

### 📁 Estructura del Repositorio
* `CODIGO FUENTE/`: Contiene el código base en C (`main.c`, `src/`, `include/`) y archivos de datos `.txt`.
* `DOCUMENTACION/`: Manual de usuario, manual técnico e informe ejecutivo del proyecto en formato PDF.
* `EJECUTABLE/`: Archivo binario compilado listo para su ejecución (`MiProyecto.exe`).
* Archivos `.ipynb`: Cuadernos de Google Colab con el análisis gráfico y matemático de los KPIs del sistema.
