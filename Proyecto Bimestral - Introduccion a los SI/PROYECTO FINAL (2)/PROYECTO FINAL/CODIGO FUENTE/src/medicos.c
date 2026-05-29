#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/medicos.h"
#include "../include/validaciones.h"

// Base de datos en memoria
Medico db_medicos[MAX_MEDICOS];
int total_medicos = 0;

// --- PERSISTENCIA: CARGAR ---
void cargar_medicos() {
	FILE *f = fopen("data_medicos.txt", "r");
	
	if (!f) {
		total_medicos = 0;
		return;
	}
	
	total_medicos = 0;
	char linea[300]; 
	
	while (fgets(linea, sizeof(linea), f) && total_medicos < MAX_MEDICOS) {
		linea[strcspn(linea, "\n")] = 0;
		
		char *token = strtok(linea, "|");
		if(token) {
			strcpy(db_medicos[total_medicos].codigo, token);
			token = strtok(NULL, "|"); if(token) strcpy(db_medicos[total_medicos].nombre, token);
			token = strtok(NULL, "|"); if(token) db_medicos[total_medicos].edad = atoi(token);
			else db_medicos[total_medicos].edad = 30;
			token = strtok(NULL, "|"); if(token) strcpy(db_medicos[total_medicos].especialidad, token);
			token = strtok(NULL, "|"); if(token) strcpy(db_medicos[total_medicos].horario, token);
			token = strtok(NULL, "|"); if(token) strcpy(db_medicos[total_medicos].contrasena, token);
			else strcpy(db_medicos[total_medicos].contrasena, "1234"); 
			
			total_medicos++;
		}
	}
	fclose(f);
}

// --- PERSISTENCIA: GUARDAR ---
void guardar_medicos() {
	FILE *f = fopen("data_medicos.txt", "w");
	if (!f) return;
	
	for(int i=0; i<total_medicos; i++) {
		fprintf(f, "%s|%s|%d|%s|%s|%s\n", 
				db_medicos[i].codigo, 
				db_medicos[i].nombre, 
				db_medicos[i].edad,
				db_medicos[i].especialidad, 
				db_medicos[i].horario,
				db_medicos[i].contrasena); 
	}
	fclose(f);
}

// --- BUSQUEDA ---
int buscar_indice_medico(const char *codigo) {
	for(int i=0; i < total_medicos; i++) {
		if(strcmp(db_medicos[i].codigo, codigo) == 0) return i;
	}
	return -1;
}

// --- NUEVA FUNCIÓN DE VALIDACIÓN DE PASSWORD ---
int verificar_contrasena_medico(const char *codigo, const char *pass_intento) {
	int idx = buscar_indice_medico(codigo);
	if(idx == -1) return 0; 
	
	if(strcmp(db_medicos[idx].contrasena, pass_intento) == 0) {
		return 1; // Correcto
	}
	return 0; // Incorrecto
}

// --- REGISTRO (ADMIN) ---
void registrar_medico() {
	if(total_medicos >= MAX_MEDICOS) {
		printf(" [!] Base de datos de medicos llena.\n");
		system("pause");
		return;
	}
	
	Medico nuevo;
	printf("\n --- ALTA DE PERSONAL MEDICO ---\n");
	
	int siguiente_num = total_medicos + 1;
	sprintf(nuevo.codigo, "MED-%03d", siguiente_num);
	printf(" > ID Asignado Automaticamente: %s\n", nuevo.codigo);
	
	// --- CORRECCIÓN AQUI: Limpieza de buffer ---
	fflush(stdin); 
	leer_texto(nuevo.nombre, 60, " Nombre del Doctor(a): ");
	
	// Validar edad
	do {
		nuevo.edad = leer_entero(" Edad: ");
		if(nuevo.edad < 20 || nuevo.edad > 90) {
			printf(" [!] Error: La edad debe ser realista (20-90).\n");
		}
	} while(nuevo.edad < 20 || nuevo.edad > 90);
	
	leer_texto(nuevo.especialidad, 50, " Especialidad: ");
	leer_texto(nuevo.horario, 20, " Horario (ej. 08:00-16:00): ");
	
	leer_texto(nuevo.contrasena, 20, " Asignar Contrasena de Acceso: ");
	
	db_medicos[total_medicos++] = nuevo;
	guardar_medicos(); 
	
	printf("\n [OK] Medico registrado correctamente.\n");
	system("pause");
}

// --- LISTADO ---
void listar_medicos() {
	if (total_medicos == 0) {
		printf("\n [i] No hay medicos registrados en el sistema.\n");
		system("pause"); 
		return;
	}
	
	printf("\n --- PLANTILLA DE MEDICOS ---\n");
	printf(" %-8s | %-25s | %-15s\n", "CODIGO", "NOMBRE", "ESPECIALIDAD");
	printf(" --------------------------------------------------------\n");
	
	for(int i=0; i<total_medicos; i++) {
		printf(" %-8s | %-25s | %-15s\n", 
			   db_medicos[i].codigo, 
			   db_medicos[i].nombre,
			   db_medicos[i].especialidad);
	}
	printf(" --------------------------------------------------------\n");
	system("pause"); 
}

// --- MENU DE GESTION ---
void gestion_medicos_admin() {
	char op = ' ';
	while(op != 'x') {
		system("cls");
		printf("\n [ GESTION DE PERSONAL MEDICO ]\n");
		printf("   [a] Registrar nuevo medico\n");
		printf("   [b] Ver plantilla completa\n");
		printf("   [x] Volver\n");
		
		char buff[10];
		leer_texto(buff, 10, " >> Seleccione: ");
		op = tolower(buff[0]);
		
		if (op == 'a') registrar_medico();
		else if (op == 'b') listar_medicos();
	}
}

// --- REPORTES ---
void exportar_reporte_medicos() {
	FILE *f = fopen("reporte_staff.txt", "w");
	if (!f) {
		printf(" [!] Error al crear el reporte.\n");
		return;
	}
	fprintf(f, "================================================================\n");
	fprintf(f, "                NOMINA DE PERSONAL MEDICO\n");
	fprintf(f, "================================================================\n");
	fprintf(f, " %-10s | %-25s | %-15s | %-10s\n", "CODIGO", "NOMBRE", "ESPECIALIDAD", "HORARIO");
	fprintf(f, "----------------------------------------------------------------\n");
	for(int i=0; i<total_medicos; i++) {
		fprintf(f, " %-10s | %-25s | %-15s | %-10s\n", 
				db_medicos[i].codigo, 
				db_medicos[i].nombre, 
				db_medicos[i].especialidad,
				db_medicos[i].horario);
	}
	fprintf(f, "================================================================\n");
	fclose(f);
	printf("\n [OK] Archivo 'reporte_staff.txt' generado.\n");
	system("pause");
}

void ver_perfil_medico(const char *codigo) {
	int idx = buscar_indice_medico(codigo); 
	if(idx == -1) return;
	
	printf("\n =========================================\n");
	printf("        FICHA DE PERSONAL MEDICO\n");
	printf(" =========================================\n");
	printf("  Codigo Institucional: %s\n", db_medicos[idx].codigo);
	printf("  Nombre Completo:      %s\n", db_medicos[idx].nombre);
	printf("  Edad:                 %d anos\n", db_medicos[idx].edad);
	printf("  Especialidad:         %s\n", db_medicos[idx].especialidad);
	printf("  Horario Asignado:     %s\n", db_medicos[idx].horario);
	printf("  Contrasena:           %s\n", db_medicos[idx].contrasena);
	printf(" =========================================\n");
	system("pause");
}
