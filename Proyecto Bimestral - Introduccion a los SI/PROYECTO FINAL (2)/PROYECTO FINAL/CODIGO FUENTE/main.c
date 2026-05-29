#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../include/pacientes.h"
#include "../include/medicos.h"
#include "../include/citas.h"
#include "../include/auth.h" 
#include "../include/validaciones.h"

// --- UTILIDADES VISUALES ---
void draw_dashboard_header(const char *role_name, const char *user_id) {
	system("cls");
	printf("\n *****************\n");
	printf("   PERFIL: %s | USUARIO: %s\n", role_name, user_id);
	printf(" *****************\n");
}

// --- DASHBOARD: PACIENTE ---
void dashboard_patient(const char *user_id) {
	char op = ' ';
	while(tolower(op) != 'q') {
		draw_dashboard_header("PACIENTE", user_id);
		printf("   [a] Mis Datos Personales\n");
		printf("   [b] Ver Mis Citas Agendadas\n");
		printf("   [c] Solicitar Nueva Cita\n");
		printf("   [q] CERRAR SESION\n"); // Deje claro que es Logout
		printf(" *****************\n");
		
		char buff[10];
		get_clean_input(buff, 10, ">> Su opcion: ");
		op = tolower(buff[0]);
		
		switch(op) {
		case 'a': access_patient_profile(user_id); break;
		case 'b': patient_view_appointments(user_id); break;
		case 'c': patient_request_appointment(user_id); break;
		case 'q': printf("\n Cerrando sesion...\n"); break; // Mensaje de salida
		default:  printf("\n [!] Opcion no valida.\n"); system("pause");
		}
	}
}

// --- DASHBOARD: MEDICO ---
void dashboard_doctor(const char *doc_code) {
	char op = ' ';
	while(tolower(op) != 'q') {
		draw_dashboard_header("MEDICO ESPECIALISTA", doc_code);
		printf("   [a] Ver Mi Agenda del Dia\n");
		printf("   [b] Historial de Pacientes Atendidos\n"); // Este lo dejamos asi u opcional
		printf("   [q] CERRAR SESION\n");
		printf(" *************************************************\n");
		
		char buff[10];
		get_clean_input(buff, 10, ">> Su opcion: ");
		op = tolower(buff[0]);
		
		switch(op) {
			// --- CAMBIO AQUI: Llamamos a la funcion real ---
		case 'a': doctor_view_agenda(doc_code); break; 
		
		case 'b': printf("\n [i] Historial en desarrollo...\n"); system("pause"); break;
		case 'q': printf("\n Cerrando sesion...\n"); break;
		}
	}
}
// --- DASHBOARD: ADMINISTRADOR ---
void dashboard_admin(const char *admin_user) {
	char selection = ' ';
	while (tolower(selection) != 'q') {
		draw_dashboard_header("ADMINISTRADOR DEL SISTEMA", admin_user);
		printf("   [a] Gestion de Pacientes \n");
		printf("   [b] Personal Medico \n");
		printf("   [c] Control Central de Citas\n");
		printf("   [q] CERRAR SESION\n");
		printf(" *****************\n");
		
		char buff[10];
		get_clean_input(buff, 10, ">> Seleccione modulo: ");
		selection = tolower(buff[0]);
		
		switch(selection) {
		case 'a': core_patient_manager(); break;
		case 'b': core_staff_manager(); break;
		case 'c': core_citas_manager(); break;
		case 'q': printf("\n Cerrando sesion...\n"); break;
		default: printf("\n [!] Opcion no reconocida.\n"); system("pause");
		}
	}
}


// --- PUNTO DE ENTRADA PRINCIPAL ---

int main() {
	// 1. CARGA INICIAL DE DATOS
	printf(" [i] Inicializando sistema...\n");
	printf("     > Cargando Medicos...\n");
	load_staff_from_file();
	
	printf("     > Cargando Pacientes...\n");
	load_patients_from_file();
	
	printf("     > Cargando Citas...\n");  // <--- NUEVA LINEA
	load_appointments_from_file();     // <--- NUEVA FUNCION
	
	printf(" [OK] Sistema listo.\n");
	// (Opcional) system("pause"); o sleep si quieres que se vea la carga
	
	char current_user_id[20]; 
	int role_id = ROLE_NONE;

	
	while(1) {
		role_id = core_perform_login(current_user_id);
		
		switch(role_id) {
		case ROLE_ADMIN: dashboard_admin(current_user_id); break;
		case ROLE_DOCTOR: dashboard_doctor(current_user_id); break;
		case ROLE_PATIENT: dashboard_patient(current_user_id); break;
		}
	}
	return 0;
}
