#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include "../include/pacientes.h"
#include "../include/medicos.h"
#include "../include/citas.h"
#include "../include/validaciones.h"

// --- MENU ADMINISTRADOR ---
void menu_admin() {
	char op = ' ';
	while(op != 'x') {
		system("cls");
		printf("\n*************************************************\n");
		printf("          PANEL DE ADMINISTRADOR\n");
		printf("*************************************************\n");
		printf(" [a] Gestion de Personal Medico\n");
		printf(" [b] Control de Citas\n");
		printf(" --- REPORTES ---\n");
		printf(" [c] Exportar Lista de Medicos (TXT)\n");
		printf(" [d] Exportar Lista de Pacientes (TXT)\n");
		printf(" [e] Generar Reporte Estadistico General\n");
		printf(" [x] Cerrar Sesion\n");
		printf("*************************************************\n");
		
		char buff[10];
		leer_texto(buff, 10, " [>] Opcion: ");
		op = tolower(buff[0]);
		
		if(op == 'a') gestion_medicos_admin();
		else if(op == 'b') gestion_citas_admin();
		else if(op == 'c') exportar_reporte_medicos();
		else if(op == 'd') exportar_reporte_pacientes();
		else if(op == 'e') generar_reporte_general();
	}
}

// --- MENU MEDICO ---
void menu_medico(char *codigo) {
	char op = ' ';
	while(op != 'x') {
		system("cls");
		printf("\n*************************************************\n");
		printf("          PANEL MEDICO (%s)\n", codigo);
		printf("*************************************************\n");
		printf(" [a] Ver Mi Agenda del Dia\n");
		printf(" [b] Ver Historial Completo\n");
		printf(" [x] Cerrar Sesion\n");
		printf("*************************************************\n");
		
		char buff[10];
		leer_texto(buff, 10, " [>] Opcion: ");
		op = tolower(buff[0]);
		
		if(op == 'a') ver_agenda_medico(codigo);
		else if(op == 'b') ver_historial_medico(codigo);
	}
}

// --- MENU PACIENTE ---
void menu_paciente(char *cedula) {
	char op = ' ';
	while(op != 'x') {
		system("cls");
		printf("\n*************************************************\n");
		printf("          PANEL PACIENTE (%s)\n", cedula);
		printf("*************************************************\n");
		printf(" [a] Solicitar Nueva Cita\n");
		printf(" [b] Ver Historial de Citas\n");
		printf(" [c] Ver Mis Datos Personales\n");
		printf(" [d] Modificar Perfil\n");
		printf(" [e] Eliminar Cuenta\n");
		printf(" [x] Cerrar Sesion\n");
		printf("*************************************************\n");
		
		char buff[10];
		leer_texto(buff, 10, " [>] Opcion: ");
		op = tolower(buff[0]);
		
		if(op == 'a') solicitar_cita(cedula);
		else if(op == 'b') ver_mis_citas(cedula);
		else if(op == 'c') ver_perfil_paciente(cedula);
		else if(op == 'd') {
			modificar_paciente(cedula);
		}
		else if(op == 'e') {
			if(eliminar_paciente(cedula) == 1) return;
		}
	}
}

// --- LOGIN (ACTUALIZADO) ---
void iniciar_sesion() {
	char credencial[30];
	char pass_input[30];
	
	printf("\n");
	leer_texto(credencial, 30, " [>] Ingrese Credencial (Cedula, Codigo o Clave Admin): ");
	
	// 1. Verificar si es Admin
	if(strcmp(credencial, "1234") == 0) {
		menu_admin();
		return;
	}
	
	// 2. Verificar si es Médico (Si contiene "MED-")
	if(strstr(credencial, "MED-") != NULL) {
		if(buscar_indice_medico(credencial) != -1) {
			// AHORA SI PIDE Y VALIDA LA CONTRASEÑA
			leer_texto(pass_input, 30, " [>] Contrasena Medica: ");
			
			if(verificar_contrasena_medico(credencial, pass_input)) {
				printf("\n [OK] Bienvenido Doctor(a).\n");
				system("pause");
				menu_medico(credencial);
			} else {
				printf("\n [!] Contrasena incorrecta.\n");
				system("pause");
			}
		} else {
			printf("\n [!] Codigo medico no encontrado.\n");
			system("pause");
		}
		return;
	}
	
	// 3. Verificar si es Paciente
	int idx = buscar_indice_paciente(credencial);
	if(idx != -1) {
		printf(" [i] Usuario encontrado: %s\n", db_pacientes[idx].nombre_completo);
		leer_texto(pass_input, 30, " [>] Ingrese su Contrasena: ");
		
		if(verificar_contrasena_paciente(idx, pass_input)) {
			printf("\n [OK] Acceso concedido.\n");
			system("pause");
			menu_paciente(credencial);
		} else {
			printf("\n [!] Contrasena incorrecta.\n");
			system("pause");
		}
		return;
	}
	
	printf("\n [!] Credencial no reconocida.\n");
	system("pause");
}

// --- MENU REGISTRO ---
void menu_registro() {
	system("cls");
	printf("\n*************************************************\n");
	printf("      SISTEMA DE SALUD - ACCESO SEGURO\n");
	printf("          NUEVO REGISTRO DE USUARIO\n");
	printf("*************************************************\n");
	printf("   Seleccione el tipo de cuenta:\n");
	printf("   [a] Paciente\n");
	printf("   [b] Medico / Especialista\n");
	printf("   [c] Administrativo\n");
	printf("-------------------------------------------------\n");
	
	char buff[10];
	leer_texto(buff, 10, " [>] Seleccione: ");
	char op = tolower(buff[0]);
	
	if (op == 'a') {
		registrar_paciente();
	} else if (op == 'b') {
		registrar_medico();
	} else if (op == 'c') {
		printf("\n [!] El registro administrativo requiere autorizacion de TI.\n");
		system("pause");
	} else {
		printf("\n [!] Opcion no valida.\n");
		system("pause");
	}
}

// --- MAIN PRINCIPAL ---
int main() {
	setlocale(LC_ALL, ""); 
	
	// Cargar datos al inicio
	cargar_medicos();
	cargar_pacientes();
	cargar_citas();
	
	char opcion = ' ';
	while(opcion != 'q') {
		system("cls");
		printf("*************************************************\n");
		printf(" SISTEMA DE SALUD - ACCESO SEGURO\n");
		printf(" BIENVENIDO\n");
		printf("*************************************************\n");
		printf(" [a] Iniciar Sesion\n");
		printf(" [b] Registrarse\n");
		printf(" [q] Salir\n");
		printf("*************************************************\n");
		
		char buff[10];
		leer_texto(buff, 10, " [>] Seleccione: ");
		opcion = tolower(buff[0]);
		
		switch(opcion) {
		case 'a': 
			iniciar_sesion(); 
			break;
		case 'b': 
			menu_registro(); 
			break;
		case 'q': 
			printf("\n [i] Cerrando sistema...\n"); 
			break;
		default: 
			printf("\n [!] Opcion invalida.\n");
			system("pause");
		}
	}
	
	return 0;
}
