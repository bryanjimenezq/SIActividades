#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/pacientes.h"
#include "../include/validaciones.h"

// Referencia externa para limpiar citas al borrar usuario
extern void cancelar_citas_usuario(const char *cedula);

Paciente db_pacientes[MAX_PACIENTES];
int total_pacientes = 0;

void cargar_pacientes() {
	FILE *f = fopen("data_pacientes.txt", "r");
	if (!f) return;
	
	total_pacientes = 0;
	char linea[250];
	
	while (fgets(linea, sizeof(linea), f) && total_pacientes < MAX_PACIENTES) {
		linea[strcspn(linea, "\n")] = 0;
		char *token = strtok(linea, "|");
		if(token) {
			strcpy(db_pacientes[total_pacientes].cedula, token);
			token = strtok(NULL, "|"); if(token) strcpy(db_pacientes[total_pacientes].nombre_completo, token);
			token = strtok(NULL, "|"); if(token) db_pacientes[total_pacientes].edad = atoi(token);
			token = strtok(NULL, "|"); if(token) strcpy(db_pacientes[total_pacientes].telefono, token);
			token = strtok(NULL, "|"); if(token) strcpy(db_pacientes[total_pacientes].contrasena, token);
			else strcpy(db_pacientes[total_pacientes].contrasena, "1234");
			
			total_pacientes++;
		}
	}
	fclose(f);
}

void guardar_pacientes() {
	FILE *f = fopen("data_pacientes.txt", "w");
	if (!f) return;
	for(int i=0; i<total_pacientes; i++) {
		fprintf(f, "%s|%s|%d|%s|%s\n", 
				db_pacientes[i].cedula, db_pacientes[i].nombre_completo, 
				db_pacientes[i].edad, db_pacientes[i].telefono, db_pacientes[i].contrasena);
	}
	fclose(f);
}

int buscar_indice_paciente(const char *cedula) {
	for(int i=0; i < total_pacientes; i++) {
		if(strcmp(db_pacientes[i].cedula, cedula) == 0) return i;
	}
	return -1;
}

int verificar_contrasena_paciente(int index, const char *pass_intento) {
	if(strcmp(db_pacientes[index].contrasena, pass_intento) == 0) return 1;
	return 0;
}

void registrar_paciente() {
	if(total_pacientes >= MAX_PACIENTES) {
		printf(" [!] Base de datos llena.\n"); return;
	}
	
	Paciente nuevo;
	printf("\n --- REGISTRO DE NUEVO PACIENTE ---\n");
	
	// 1. CÉDULA
	int cedula_valida = 0;
	do {
		leer_texto(nuevo.cedula, 20, " Ingrese su Número de Cédula: ");
		if(buscar_indice_paciente(nuevo.cedula) != -1) printf(" [!] Error: Esta cédula ya está registrada.\n");
		else if(validar_cedula_ecuatoriana(nuevo.cedula) == 0) printf(" [!] Error: Cédula inválida.\n");
		else cedula_valida = 1;
	} while(!cedula_valida);
	
	// 2. NOMBRE
	leer_texto(nuevo.nombre_completo, 60, " Nombre Completo: ");
	
	// 3. EDAD
	do {
		nuevo.edad = leer_entero(" Edad: ");
		if(nuevo.edad < 1 || nuevo.edad > 100) {
			printf(" [!] Error: La edad debe estar entre 1 y 100 años.\n");
		}
	} while(nuevo.edad < 1 || nuevo.edad > 100);
	
	// 4. TELÉFONO
	int tel_valido = 0;
	do {
		leer_texto(nuevo.telefono, 15, " Teléfono de contacto: ");
		if(validar_telefono_movil(nuevo.telefono)) tel_valido = 1;
		else printf(" [!] Error: El teléfono debe tener 10 dígitos numéricos (ej. 099...).\n");
	} while(!tel_valido);
	
	// 5. CONTRASEÑA
	printf("\n [ SEGURIDAD ]\n");
	leer_texto(nuevo.contrasena, 20, " Cree su Contraseña: ");
	
	db_pacientes[total_pacientes++] = nuevo;
	guardar_pacientes();
	
	printf("\n [OK] Paciente registrado exitosamente. Ya puede iniciar sesión.\n");
	system("pause");
}

void listar_pacientes() {
	printf("\n --- LISTADO DE PACIENTES ---\n");
	for(int i=0; i<total_pacientes; i++) {
		printf(" %s | %s | %d años\n", db_pacientes[i].cedula, db_pacientes[i].nombre_completo, db_pacientes[i].edad);
	}
	system("pause");
}

void ver_perfil_paciente(const char *mi_cedula) {
	int idx = buscar_indice_paciente(mi_cedula);
	if(idx == -1) return;
	
	system("cls");
	printf("\n*************************************************\n");
	printf("              MIS DATOS PERSONALES\n");
	printf("*************************************************\n");
	printf("  Nombre:   %s\n", db_pacientes[idx].nombre_completo);
	printf("  Cédula:   %s\n", db_pacientes[idx].cedula);
	printf("  Edad:     %d años\n", db_pacientes[idx].edad);
	printf("  Teléfono: %s\n", db_pacientes[idx].telefono);
	printf("*************************************************\n");
	system("pause");
}

void exportar_reporte_pacientes() {
	FILE *f = fopen("reporte_pacientes.txt", "w");
	if (!f) {
		printf(" [!] Error al crear el reporte.\n");
		return;
	}
	fprintf(f, "====================================================\n");
	fprintf(f, "              BASE DE DATOS DE PACIENTES\n");
	fprintf(f, "====================================================\n");
	fprintf(f, " %-12s | %-25s | %-12s\n", "CEDULA", "NOMBRE", "TELEFONO");
	fprintf(f, "----------------------------------------------------\n");
	for(int i=0; i<total_pacientes; i++) {
		fprintf(f, " %-12s | %-25s | %-12s\n", 
				db_pacientes[i].cedula, db_pacientes[i].nombre_completo, db_pacientes[i].telefono);
	}
	fprintf(f, "====================================================\n");
	fclose(f);
	printf("\n [OK] Archivo 'reporte_pacientes.txt' generado.\n");
	system("pause");
}

// --- NUEVA FUNCIÓN: MODIFICAR PACIENTE ---
void modificar_paciente(const char *cedula) {
	int idx = buscar_indice_paciente(cedula);
	if (idx == -1) return;
	
	system("cls");
	printf("\n =========================================\n");
	printf("        MODIFICAR DATOS PERSONALES\n");
	printf(" =========================================\n");
	printf("  [1] Nombre Completo\n");
	printf("  [2] Edad\n");
	printf("  [3] Telefono\n");
	printf("  [4] Cambiar Contrasena\n");
	printf("  [x] Cancelar\n");
	printf(" -----------------------------------------\n");
	
	char buff[10];
	leer_texto(buff, 10, " [>] Seleccione opcion: ");
	char op = buff[0];
	
	if (op == '1') {
		leer_texto(db_pacientes[idx].nombre_completo, 60, " Nuevo Nombre: ");
	} else if (op == '2') {
		int nueva_edad;
		do {
			nueva_edad = leer_entero(" Nueva Edad: ");
		} while (nueva_edad < 1 || nueva_edad > 100);
		db_pacientes[idx].edad = nueva_edad;
	} else if (op == '3') {
		char nuevo_tel[15];
		int valido = 0;
		do {
			leer_texto(nuevo_tel, 15, " Nuevo Telefono: ");
			if (validar_telefono_movil(nuevo_tel)) valido = 1;
			else printf(" [!] Telefono invalido. Intente de nuevo.\n");
		} while (!valido);
		strcpy(db_pacientes[idx].telefono, nuevo_tel);
	} else if (op == '4') {
		leer_texto(db_pacientes[idx].contrasena, 20, " Nueva Contrasena: ");
	} else {
		return;
	}
	
	guardar_pacientes();
	printf("\n [OK] Perfil actualizado correctamente.\n");
	system("pause");
}

// --- NUEVA FUNCIÓN: ELIMINAR PACIENTE ---
int eliminar_paciente(const char *cedula) {
	int idx = buscar_indice_paciente(cedula);
	if (idx == -1) return 0;
	
	printf("\n [!!!] ALERTA DE SEGURIDAD [!!!]\n");
	printf(" Esta a punto de ELIMINAR su cuenta permanentemente.\n");
	printf(" Sus citas pendientes seran canceladas.\n");
	
	char confirm[20];
	leer_texto(confirm, 20, " Escriba 'CONFIRMAR' para borrar: ");
	
	if (strcmp(confirm, "CONFIRMAR") == 0) {
		// 1. Cancelar citas asociadas
		cancelar_citas_usuario(cedula);
		
		// 2. Eliminar paciente del arreglo
		for (int i = idx; i < total_pacientes - 1; i++) {
			db_pacientes[i] = db_pacientes[i + 1];
		}
		total_pacientes--;
		
		// 3. Guardar cambios en disco
		guardar_pacientes();
		
		printf("\n [i] Cuenta eliminada. Hasta luego.\n");
		system("pause");
		return 1; 
	} else {
		printf("\n [i] Operacion cancelada.\n");
		system("pause");
		return 0;
	}
}
