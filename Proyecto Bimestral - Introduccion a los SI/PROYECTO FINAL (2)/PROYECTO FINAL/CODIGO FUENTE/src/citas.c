#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/citas.h"
#include "../include/validaciones.h"
#include "../include/medicos.h"

Cita db_citas[MAX_CITAS];
int total_citas = 0;

// --- PERSISTENCIA: CARGAR DATOS ---
void cargar_citas() {
	FILE *f = fopen("data_citas.txt", "r");
	if (!f) { 
		total_citas = 0; 
		return; 
	}
	
	total_citas = 0;
	char linea[256];
	
	while (fgets(linea, sizeof(linea), f) && total_citas < MAX_CITAS) {
		linea[strcspn(linea, "\n")] = 0; 
		
		char *token = strtok(linea, "|");
		if(token) {
			db_citas[total_citas].id = atoi(token);
			token = strtok(NULL, "|"); if(token) strcpy(db_citas[total_citas].id_paciente, token);
			token = strtok(NULL, "|"); if(token) strcpy(db_citas[total_citas].codigo_medico, token);
			token = strtok(NULL, "|"); if(token) strcpy(db_citas[total_citas].fecha, token);
			token = strtok(NULL, "|"); if(token) strcpy(db_citas[total_citas].motivo, token);
			token = strtok(NULL, "|"); if(token) db_citas[total_citas].estado = atoi(token);
			total_citas++;
		}
	}
	fclose(f);
}

// --- PERSISTENCIA: GUARDAR DATOS ---
void guardar_citas() {
	FILE *f = fopen("data_citas.txt", "w");
	if (!f) return;
	
	for(int i=0; i<total_citas; i++) {
		fprintf(f, "%d|%s|%s|%s|%s|%d\n", 
				db_citas[i].id, 
				db_citas[i].id_paciente, 
				db_citas[i].codigo_medico,
				db_citas[i].fecha, 
				db_citas[i].motivo, 
				db_citas[i].estado);
	}
	fclose(f);
}

// --- FUNCIONES DEL PACIENTE ---
void solicitar_cita(const char *mi_cedula) {
	if(total_citas >= MAX_CITAS) {
		printf(" [!] Sistema de citas lleno.\n"); 
		system("pause");
		return;
	}
	
	Cita nueva;
	nueva.id = total_citas + 1; 
	strcpy(nueva.id_paciente, mi_cedula);
	
	printf("\n --- NUEVA SOLICITUD DE CITA ---\n");
	listar_medicos(); 
	
	int medico_valido = 0;
	do {
		leer_texto(nueva.codigo_medico, 10, " Ingrese Código del Médico (ej. MED-001): ");
		if(buscar_indice_medico(nueva.codigo_medico) != -1) {
			medico_valido = 1;
		} else {
			printf(" [!] Médico no encontrado. Intente de nuevo.\n");
		}
	} while(!medico_valido);
	
	leer_texto(nueva.fecha, 20, " Fecha deseada (DD-MM-AAAA): ");
	leer_texto(nueva.motivo, 100, " Motivo de consulta: ");
	
	nueva.estado = 0; 
	
	db_citas[total_citas++] = nueva;
	guardar_citas();
	
	printf("\n [OK] Cita #%d agendada correctamente.\n", nueva.id);
	system("pause");
}

void ver_mis_citas(const char *mi_cedula) {
	printf("\n --- MIS CITAS ---\n");
	printf(" %-4s | %-12s | %-10s | %-10s\n", "ID", "FECHA", "MEDICO", "ESTADO");
	printf(" ------------------------------------------------\n");
	
	int encontradas = 0;
	for(int i=0; i<total_citas; i++) {
		if(strcmp(db_citas[i].id_paciente, mi_cedula) == 0) {
			printf(" #%-3d | %-12s | %-10s | %s\n", 
				   db_citas[i].id, 
				   db_citas[i].fecha, 
				   db_citas[i].codigo_medico, 
				   (db_citas[i].estado == 0 ? "ACTIVA" : "CANCELADA"));
			encontradas++;
		}
	}
	
	if(!encontradas) printf(" [i] No tiene citas registradas.\n");
	printf(" ------------------------------------------------\n");
	system("pause");
}

// --- FUNCIONES DEL MÉDICO ---
void ver_agenda_medico(const char *mi_codigo) {
	printf("\n --- MI AGENDA (%s) ---\n", mi_codigo);
	printf(" %-5s | %-12s | %-12s | %-20s\n", "ID", "FECHA", "PACIENTE", "MOTIVO");
	printf(" ---------------------------------------------------------\n");
	
	int hay_citas = 0;
	for(int i=0; i<total_citas; i++) {
		if(strcmp(db_citas[i].codigo_medico, mi_codigo) == 0 && db_citas[i].estado == 0) {
			printf(" %-5d | %-12s | %-12s | %-20s\n", 
				   db_citas[i].id, 
				   db_citas[i].fecha, 
				   db_citas[i].id_paciente, 
				   db_citas[i].motivo);
			hay_citas = 1;
		}
	}
	
	if(!hay_citas) printf(" [i] No tiene citas activas programadas.\n");
	printf(" ---------------------------------------------------------\n");
	system("pause");
}

void ver_historial_medico(const char *mi_codigo) {
	printf("\n =========================================================\n");
	printf("        HISTORIAL DE PACIENTES (%s)\n", mi_codigo);
	printf(" =========================================================\n");
	printf(" %-5s | %-12s | %-12s | %-15s\n", "ID", "FECHA", "PACIENTE", "ESTADO");
	printf(" ---------------------------------------------------------\n");
	
	int encontrados = 0;
	for(int i=0; i<total_citas; i++) {
		if(strcmp(db_citas[i].codigo_medico, mi_codigo) == 0) {
			char estado_str[15];
			if(db_citas[i].estado == 0) strcpy(estado_str, "ACTIVA");
			else strcpy(estado_str, "CANCELADA");
			
			printf(" %-5d | %-12s | %-12s | %-15s\n", 
				   db_citas[i].id, 
				   db_citas[i].fecha, 
				   db_citas[i].id_paciente, 
				   estado_str);
			encontrados++;
		}
	}
	if(!encontrados) printf(" [i] No tiene historial registrado.\n");
	else printf("\n [i] Total registros encontrados: %d\n", encontrados);
	printf(" =========================================================\n");
	system("pause");
}

// --- FUNCIONES DE ADMINISTRADOR ---
void gestion_citas_admin() {
	printf("\n [!] Funcion administrativa: Cancelar Citas\n");
	int id = leer_entero(" Ingrese ID de la cita a cancelar: ");
	
	int encontrado = -1;
	for(int i=0; i<total_citas; i++) {
		if(db_citas[i].id == id) {
			encontrado = i;
			break;
		}
	}
	
	if(encontrado != -1) {
		db_citas[encontrado].estado = 1; 
		guardar_citas();
		printf(" [OK] La Cita #%d ha sido cancelada.\n", id);
	} else {
		printf(" [!] Cita con ID %d no encontrada.\n", id);
	}
	system("pause");
}

void generar_reporte_general() {
	FILE *f = fopen("reporte_gerencial.txt", "w");
	if (!f) {
		printf(" [!] Error al crear el reporte.\n");
		return;
	}
	
	int activas = 0;
	int canceladas = 0;
	
	for(int i=0; i<total_citas; i++) {
		if(db_citas[i].estado == 0) activas++;
		else canceladas++;
	}
	
	fprintf(f, "============================================\n");
	fprintf(f, "       REPORTE ESTADISTICO DEL HOSPITAL\n");
	fprintf(f, "============================================\n");
	fprintf(f, " Total de Citas Registradas:  %d\n", total_citas);
	fprintf(f, " --------------------------------------------\n");
	fprintf(f, " [v] Citas Activas (Pendientes): %d\n", activas);
	fprintf(f, " [x] Citas Canceladas:           %d\n", canceladas);
	fprintf(f, "============================================\n");
	
	fclose(f);
	printf("\n [OK] Reporte 'reporte_gerencial.txt' generado exitosamente.\n");
	system("pause");
}


void cancelar_citas_usuario(const char *cedula) {
	int cambios = 0;
	for(int i=0; i<total_citas; i++) {
	
		if(strcmp(db_citas[i].id_paciente, cedula) == 0 && db_citas[i].estado == 0) {
			db_citas[i].estado = 1; 
			cambios = 1;
		}
	}
	if(cambios) {
		guardar_citas();
	}
}
