#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/validaciones.h"

void leer_texto(char *buffer, int tamano, const char *mensaje) {
	printf("%s", mensaje);
	fflush(stdout);
	fgets(buffer, tamano, stdin);
	size_t len = strlen(buffer);
	if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';
	fflush(stdin);
}

int leer_entero(const char *mensaje) {
	char temp_buff[64];
	leer_texto(temp_buff, 64, mensaje);
	for(int i = 0; temp_buff[i] != '\0'; i++) {
		if(!isdigit(temp_buff[i])) return -1;
	}
	if(strlen(temp_buff) == 0) return 0;
	return atoi(temp_buff);
}

int validar_cedula_ecuatoriana(const char *cedula) {
	if (strlen(cedula) != 10) return 0;
	for(int i=0; i<10; i++) if(!isdigit(cedula[i])) return 0;
	int suma_total = 0;
	int codigo_provincia = (cedula[0] - '0') * 10 + (cedula[1] - '0');
	if (codigo_provincia < 1 || codigo_provincia > 24) return 0;
	int coeficientes[] = {2, 1, 2, 1, 2, 1, 2, 1, 2};
	for (int k = 0; k < 9; k++) {
		int digito_actual = cedula[k] - '0';
		int producto = digito_actual * coeficientes[k];
		if (producto >= 10) producto -= 9;
		suma_total += producto;
	}
	int digito_verificador = cedula[9] - '0';
	int decena_superior = ((suma_total / 10) + 1) * 10;
	if (suma_total % 10 == 0) decena_superior = suma_total;
	return ((decena_superior - suma_total) == digito_verificador);
}


int validar_telefono_movil(const char *telefono) {
	// 1. Longitud exacta
	if(strlen(telefono) != 10) return 0;
	
	// 2. Solo numeros
	for(int i=0; i<10; i++) {
		if(!isdigit(telefono[i])) return 0;
	}
	
	// 3. (Opcional) Que empiece con 09
	if(telefono[0] != '0' || telefono[1] != '9') return 0;
	
	return 1;
}
