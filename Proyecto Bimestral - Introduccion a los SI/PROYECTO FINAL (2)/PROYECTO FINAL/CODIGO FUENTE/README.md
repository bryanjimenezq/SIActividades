# Sistema de Gestión de Citas Médicas (SGCM)

Proyecto semestral para la materia de **Programación I** de la **Escuela Politécnica Nacional**.

Este sistema de consola automatiza el flujo operativo de un centro de salud de nivel primario, permitiendo la gestión de pacientes, médicos, citas y reportes.

## Características

* **Autenticación por Roles:**
    * **Administrador:** Gestión total (médicos, citas, reportes).
    * **Médico:** Agenda personal e historial de pacientes.
    * **Paciente:** Registro, agendamiento y consulta de historial.
* **Gestión de Datos (CRUD):** Registro y edición de pacientes y personal médico.
* **Validaciones Reales:** Algoritmo de validación de cédula ecuatoriana (Módulo 10).
* **Persistencia:** Almacenamiento de datos en archivos planos (`.txt`).
* **Reportes:** Generación de archivos de texto para auditoría (Staff, Pacientes, Gerencial).

##  Instalación y Ejecución
1.  Descarga el código fuente o el ejecutable.
2.  Asegúrate de tener los archivos de datos en la misma carpeta del ejecutable (si no existen, el sistema los crea automáticamente):
    * `data_pacientes.txt`
    * `data_medicos.txt`
    * `data_citas.txt`
3.  Ejecuta el archivo **`MiProyecto.exe`**.

##  Tecnologías

* **Lenguaje:** C
* **Estructura:** Modular Monolítico (`src`, `include`)
* **Persistencia:** Archivos de texto plano

##  Autores

* **Derlis Ledesma**
* **Nancy Insuasti**

---
*Escuela Politécnica Nacional - 2025-B*