void _display_initialization(){
  // Inicialización de la comunicación con la pantalla
  display.begin();
  // Establecer el contraste del display (0 a 127)
  display.setContrast(50);
  // Limpiar la pantalla
  display.clearDisplay();
  // Establecer el tamaño del texto
  display.setTextSize(1);
  // Establecer el color del texto (1: negro, 0: blanco)
  display.setTextColor(WHITE);
}


