
/******************************************************************
** Código creado en Electrónica 5Hz                              **
** www.5hz-electronica.com                                       **
** Por:                                                          **
** Basado en el código de:                                       **
** Descripción del código:                                       **
**                                                               **
** Ejemplo para demostrar la librería del TSL2561                **
** ¡Dominio público!                                             **
*******************************************************************

Conexiones para la comunicacion i2c usando el breakout de 5Hz con un Arduino Uno / Duemilanove:
Arduino <-> Breakout board
Gnd      -  GND
3.3v     -  VCC
3.3v     -  CS
Analog 4 -  SDA
Analog 5 -  SCL
ADDR     -  GND o VCC 

Conexiones para la comunicacion i2c usando el breakout de 5Hz con un Arduino Mega / Mega ADK:
Arduino <-> Breakout board
Gnd      -  GND
3.3v     -  VCC
3.3v     -  CS
20       -  SDA
21       -  SCL
ADDR     -  GND o VCC 
*/


#include <Wire.h>
#include "TSL2561.h"


// La dirección  será diferente dependiendo si se deja 
// el pin ADDR flotado (addr 0x39), o si se conecta a tierra o a vcc. En esos casos
// se usa TSL2561_ADDR_LOW (0x29) or TSL2561_ADDR_HIGH (0x49) respectivamente
TSL2561 tsl(TSL2561_ADDR_FLOAT); 

void setup(void) {
  Serial.begin(9600);
  
  if (tsl.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No sensor?");
    while (1);
  }
    
  // Puede cambiar la ganancia sobre la marcha, para adaptarse a las situaciones de luz más brillantes o más oscuras
  //tsl.setGain(TSL2561_GAIN_0X);         // Configura no ganancia (para situaciones de brillo)
  tsl.setGain(TSL2561_GAIN_16X);      // Configura ganancia 16x (para situaciones oscuras)
  
  // Cambiar el tiempo de integración le da más tiempo para sentir la luz
  // plazos más largos son más lentos, pero son buenos en situaciones de muy poca luz. 
  tsl.setTiming(TSL2561_INTEGRATIONTIME_13MS);  // tiempo de integración más corto (luz brillante)
  //tsl.setTiming(TSL2561_INTEGRATIONTIME_101MS);  // tiempo de integración mediano (luz media)
  //tsl.setTiming(TSL2561_INTEGRATIONTIME_402MS);  // tiempo de integración más largo (luz tenue)
  
  // ¡Ahora estamos listos para tomar lecturas! 
}

void loop(void) {
  // Basta con leer el diodo infrarrojo fullspectrum  
  // o 'visible' (diferencia entre los dos) canales.
  // Esto puede tardar 13-402 milisegundos.  Descomente la línea que desea leer 
  uint16_t x = tsl.getLuminosity(TSL2561_VISIBLE);     
  //uint16_t x = tsl.getLuminosity(TSL2561_FULLSPECTRUM);
  //uint16_t x = tsl.getLuminosity(TSL2561_INFRARED);
  
  Serial.println(x, DEC);

  // Ejemplo para leer datos más avanzados. Lee 32 bits con máximo 16 bits IR, mínimo 16 bits full spectrum
  // De esta manera puedes hacer cualquier operación y comparación que quieras.
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.print("IR: "); Serial.print(ir);   Serial.print("\t\t");
  Serial.print("Full: "); Serial.print(full);   Serial.print("\t");
  Serial.print("Visible: "); Serial.print(full - ir);   Serial.print("\t");
  
  Serial.print("Lux: "); Serial.println(tsl.calculateLux(full, ir));
  
  delay(100); 
}
