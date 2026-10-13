
#include <stdio.h>
#include <string.h>

int number (char *str, long *valor)
{
	long num = 0;
	int len = strlen(str);
	int i = 0;
	int is_number = str[i] && (str[i] <= '9' && str[i] >= '0');
	
	while (is_number && i < len) {
		num = num * 10 + (str[i] - '0'); 
		i++;
		is_number = str[i] && (str[i] <= '9' && str[i] >= '0');
	}
	
	if (is_number) {
		*valor = num;
		
	/* Si no es un número comprobamos si es porque está leyendo el carácter final \0 ó \n, entonces será un número */
	} else if ((!is_number && str[i] == '\n' && i > 0) || (!is_number && str[i] == '\0' && i > 0)) {
		is_number = 1;
		*valor = num;
	}
	
	if (str[i] == '\n' && i > 0) {
		printf("barra n\n");
	} else if (str[i] == '\0' && i > 0) {
		printf("barra 0\n");
	}
	
	return is_number;
}

int main ()
{
	/*
	int num;
	char cero = '0'; 
	char cadena = '5';
	num = (int) cadena - (int) cero;
	
	printf("cadena: %c\n", cadena);
	printf("num: %d\n", num);
	
	// -----------------------------
	
	char *numero = "34";
	long valor;
	int res = number(numero, &valor);
	
	printf("numero: %s,  valor: %ld,  res: %d\n", numero, valor, res);
	*/
	/*
	int n1 = 7;
	int n2 = 4;
	double res = n1 / n2;   // división real
	int p_entera = res;    // parte entera
	res = (res - p_entera) * 100;  // dos decimales
	int dec = res * 100;
	
	printf("res: %f\n", res);
	printf("p_entera: %d, dec: %f", p_entera, res);
	printf("resultado: %f\n", p_entera + dec);
	
	int num = (n1 + n2) * 100;
	double res2 = (num / 2) / 100;
	
	printf("res: %f", res2);
	*/
	
	int suma_total = 56;
	int num_totales = 3;
	int media = (suma_total * 100) / num_totales;
	int p_entera = media / 100;
	int p_decimal = media % 100;
	printf("media: %d.%02d, de %d numeros\n", p_entera, p_decimal, num_totales);
	
	return 0;
}
