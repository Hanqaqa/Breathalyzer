/*
Alcoholímetro

Creado por Marco Vázquez Madero para Ingeniería Electrónica Industrial en la asignatura de Procesadores Integrados.

Este proyecto usará el sensor MQ3 y una pantalla OLED para mostrar los datos obtenidos por este, para evitar
La memoria EEPROM se usará para guardar el mayor dato obtenido desde que se enciende el aparato

*/

//---------------------------------------------------------------------------------
//      Inclusión de librerías
//---------------------------------------------------------------------------------
#include <SPI.h>                                                     //Librería para transmision de datos
#include <Wire.h>                                                    //Libreria para comunicacion I2C
#include <Adafruit_GFX.h>                                            //Libreria para hacer graficos en pantalla
#include <Adafruit_SSD1306.h>                                        //Librerías propias de pantalla OLED Adafruit
#include <EEPROM.h>                                                  //Libreria para leer y escribir en la EEPROM
#define OLED_RESET 4                                                 //Instruccion necesaria para eliminar informacion en pantalla

//---------------------------------------------------------------------------------
//      Definicion de variables
//---------------------------------------------------------------------------------
int SEGUNDOS;
int T_ESPERA = 20;
int SENSOR = 0;                                                      //Numero de pin para el sensor MQ3
int BUZZER=9;                                                        //Numero de pin para el buzzer con modulación de pulso
int VAL = 0;                                                         //Variable usada para almacenar el valor directo del sensor en partes por millon (Aprox)
int VALORPANTALLA = 0;                                               //Variable usada para almacenar el valor aproximado del sensor en mg/L (Aprox)
int VAL1;                                                            
int VAL2;
int VAL3;                                                            //Variables usadas para almacenar datos con un espacio de 10 milisegundos
int VAL4;
int VAL5; 
unsigned int FREQ=2000;                                              //Frecuencia para el buzzer
unsigned long duracion=500;
Adafruit_SSD1306 display(OLED_RESET);                                //Reset de la pantalla OLED

//---------------------------------------------------------------------------------
//      Preparación del programa
//---------------------------------------------------------------------------------
void setup()   {    
 pinMode(BUZZER,OUTPUT);                                             
 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);                          //Direccion 3C para la pantalla en I2C
 display.clearDisplay();                                             //Reset de la pantalla OLED
 EEPROM.write(1, 0);                                                 //Instruccion que se puede añadir o quitar para eliminar el dato de si se ha acercado el alcohol
}
//---------------------------------------------------------------------------------
//      Programa
//---------------------------------------------------------------------------------
void loop() {                                                        
  
  VAL = LECTORALCOHOL();                                             //Declaracion del programa que leerá el valor de alcohol, se iniciará con el arduino
  TITULO();                                                          //Declaración del programa que mantedrá el titulo en la pantalla durante todo el rato
  PRECALENTAMIENTO();                                                //Declaración del programa que dibujará una barra y rellenará en el tiempo de calentamiento
  VALORPANTALLA=VAL/12;                                              //12 es el valor aproximado experimental con el que se pasa del voltaje leido a mg/L de alcohol
  SEGUNDOS = millis()/1000;                                          //Variable para usar el tiempo de precalentamiento
  
  
  if(SEGUNDOS<=T_ESPERA)                                             //Decisión inicial del programa de hacer el precalentamiento y al cabo de T_ESPERA mostrar medidas
  {
    SEGUNDOS = map(SEGUNDOS, 0, T_ESPERA, 0, 160);                   //Instruccion de libreria Adafruit para crear un efecto de pantalla que se desvanece
    display.fillRect(0, 0, SEGUNDOS,200,BLACK);                      
  }else                                                              //Mediciones del alcoholimetro y mostrar por pantalla los distintos datos
  {
     TITULO();                                                       //Instruccion para el display de mantener un titulo constante
     EEPROMPANTALLA();                                               //Instruccion para mostrar si se ha detectado alcohol en alguna
     NIVELDEALCOHOL(VAL);                                            //Instruccion para leer el voltaje dado por el sensor
     CONCENTRACIONENAIRE(VAL);                                       //Instruccion para mostrar los distintos niveles de alcohol
  }
  display.display();                                                 //Instruccion necesaria para mostrar en pantalla

}

void EEPROMPANTALLA()
{                                                                      //Si se sobrepasa un cierto nivel
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(90,55);
  
  if(EEPROM.read(1)==0)                                                //Si previamente no se ha llegado al nivel, se mantendrá un NO en la esquina inferior derecha              
  {
      display.println("NO");                                    
  }
  if (EEPROM.read(1)==1)                                              //Al llegar al nivel de alcohol en aire se guardará en la EEPROM un dato como señal de que se
  {                                                                   //ha alcanzado alguna vez esa concentracion de alcohol en aire
      display.println("SI");
  }
}
void TITULO()                                                       //Programa para mostrar en la pantalla un titulo
{
  display.clearDisplay();                                           //Instruccion para borrar de la pantalla el precalentamiento
  display.setTextSize(1);                                           //Tamaño de texto
  display.setTextColor(WHITE);                                      //Color de texto , esta pantalla solo ofrece blanco, negro e invertir los colores ya existentes
  display.setCursor(28,0);                                          //Pixel donde empezará el texto en coordenadas
  display.println("Alcoholimetro");                                 //Texto que se mostrará
}

void PRECALENTAMIENTO()                                             //Programa que se iniciara con el arduino y mostrara el texto preparando, mas tarde sera borrado
{                                                                   //por el LECTORALCOHOL, con la instruccion clearDisplay, que dejará la pantalla lista para escribir
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5,20);
  display.println("Preparando");
}

 int LECTORALCOHOL()                                               //Programa que leerá cinco datos del sensor en un espacio de tiempo de 50ms
 {


  display.clearDisplay();                                          //Instruccion para borrar las letras que anteriormente estaban en esa posicion
  VAL1 = analogRead(SENSOR);                                       //Primer valor leido
  delay(10);
  VAL2 = analogRead(SENSOR);                                       //Segundo valor leido
  delay(10);
  VAL3 = analogRead(SENSOR);                                       //Tercer valor leido
  delay(10);
  VAL4 = analogRead(SENSOR);                                       //Cuarto valor leido
  delay(10);
  VAL5 = analogRead(SENSOR);                                       //Quinto valor leido
  
  VAL = (VAL1+VAL2+VAL3+VAL4+VAL5)/5;                              //Media aritemetica de los cinco valores
  return VAL;
 }
 
void NIVELDEALCOHOL(int VALue)                                     //Programa que se iniciara al finalizar el precalentamiento y mostrará el valor aproximado
{                                                                  //de mg/L de alcohol en la cercania del sensor
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20,55);
  display.println(VALORPANTALLA);
}

void CONCENTRACIONENAIRE(int VALue)                                    //Programa que mostrara los distintos niveles de alcohol, haciendo un sonido mediante el buzzer
{                                                                      //si se sobrepasa un cierto nivel
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(30,25);
  
  if(VALue<150)                                                        //Para valores practicamente nulos de alcohol en el aire, aunque puede variar debido a temperatura
  {
      display.println("0 mg/L");                                    
  }
  if (VALue>=150 && VALue<220)                                         //Para valores de aproximadamente de 15 mg/L se mostrará en pantalla el texto
  {
      display.println("15 mg/L");
  }
  if (VALue>=220 && VALue<300)                                         //Para valores cercanos a 30 mg/L, ya sancionables
  {
      display.println("30 mg/L");
  }
  if (VALue>=300 && VALue <350)                                        //Para valores cercanos a 45 mg/L
  {
      display.println("45 mg/L");
      tone(BUZZER,5000,500);                                           //Sonará un tono a 5000Hz cuando se encuentre entre esos dos valroes durante 500ms
  }
  if(VALue>350)                                                        //Para valores superiores, se mostrará el texto Alcohol
  {
     display.println("Alcohol");
     tone(BUZZER,FREQ,500);                                            //Sonará un tono de frencuencia FREQ, especificada al inicio, durante medio segundo
     EEPROM.write(1, 1);
  }
 }
 

//---------------------------------------------------------------------------------
//      Final del programa
//---------------------------------------------------------------------------------

/*

Los valores medidos son totalemnte orientativos y, la principal diferencia con un alcoholímetro de calidad es que el sensor se encuentra en una pequeña camara alejada
de la humedad y temperatura y tener un tiempo de calentamiento mayor.

*/
