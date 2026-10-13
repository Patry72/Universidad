
ISR () {
	if (ciclo_menor == 1) {
		A();
		B();
		C();
		ciclo_menor++;
	} else if (ciclo_menor == 2) {
		A();
		ciclo_menor++;
	} else if (ciclo_menor == 3) {
		A();
		B();
		ciclo_menor++;
	} else if (ciclo_menor == 4) {
		A();
		C();
		ciclo_menor++:
	} else if (ciclo_menor == 5) {
		A();
		B();
		ciclo_menor++;
	} else if (ciclo_menor == 6) {
		A();
		ciclo_menor = 0;
	}
}

void main () {
	ciclo_menor = 0;
	
	// En algún momento ciclo_menor = 1 y entra en la ISR
}