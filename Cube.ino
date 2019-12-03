#include <Wire.h>
#include <SPI.h>
#include <Arduino_LSM6DS3.h>
#include <MFRC522.h>
#include "MAX1704X.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const byte Off = 0;                   //Definimos colores del led RGB
const byte Red = 1;
const byte Green = 2;
const byte Blue = 3;
const byte White = 4;
const byte Yellow = 5;
const byte Purple = 6;


const int Eeprom_Address = 0x50;
byte Eeprom;

int Menu = 0;                       //Control de nivel del menu
int ID;                            //Identificador del cubo
int Mode;                         //Modo de funcionamiento
byte X_pass;                       //Contraseña del modo XYZ
byte Y_pass;
byte Z_pass;

byte X_sense;
byte Y_sense;
byte Z_sense;

byte treas[20];

byte EN_PIR;                      //Control de sensores habilitados
byte EN_US;
byte EN_Light;
byte EN_RFID;
byte EN_Key;
byte EN_RGB;
byte EN_Display;
byte EN_Buzz;

int win = 0;

byte readCard[4];                  //Identificador de la tarjeta RFID

int cm;                           //distancia del sensor de US

bool button1_state;                   //Control del estado de los botones
bool button2_state; 

const int RLed_Pin = 5;               //Definimos los pines del Led RGB
const int GLed_Pin = 3;
const int BLed_Pin = 2;

const int Buzz_Pin = 6;               //Definimos el pin del zumbador

const int Trig_Pin = A1;              //Definimos los pines del sensor de US
const int Echo_Pin = A0;

const int Pres_Pin = A3;              //Definimos el pin del sensor de presencia

const int Light_Pin = A2;             //Definimos el pin del sensor de luz

const int Key_Pin = 8;               //Definimos el pin del interruptor de llave

const int Button1_Pin = 4;            //Definimos los pines de los tres pulsadores
const int Button2_Pin = 7;


const int RST_PIN = 9;              // Pin 9 para el reset del RC522
const int SS_PIN = 10;              // Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Crear instancia del MFRC522

Adafruit_SSD1306 display(128, 32, &Wire, -1);

const unsigned char logo_cube [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0x40, 0x60, 0x20,
0x20, 0x60, 0x40, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0x0E, 0x1A, 0x1B, 0x31, 0x31, 0x60, 0x60, 0xC0, 0xC0, 0x80,
0x80, 0xC0, 0xC0, 0xE0, 0xE0, 0xF1, 0xF1, 0xFB, 0xFA, 0xFE, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x18, 0x1F,
0x1F, 0x1F, 0x0F, 0x0F, 0x07, 0x07, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


void setup() 
{


  Serial1.begin(9600);                //Inicializamos Serial 1   

  SPI.begin();                        //Inicializamos SPI   
  mfrc522.PCD_Init();                 //Inicializamos el lector RFID
  Wire.begin();                       //Inicializamos puerto I2C

   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Inicializamos el display I2C 128x32
   display.clearDisplay();

  /* 
   display.drawBitmap(96,0,logo_cube,32,32,WHITE);
   display.display();
   delay(1000);
   display.clearDisplay();
   display.display();
  */

  if (!IMU.begin()) 
  {
    Serial1.println("Failed to initialize IMU!");
    while (1);
  }
  
  pinMode(RLed_Pin, OUTPUT);            //Configuramos los pines de E/S
  pinMode(GLed_Pin, OUTPUT);
  pinMode(BLed_Pin, OUTPUT);
  pinMode(Trig_Pin, OUTPUT);
  pinMode(Echo_Pin, INPUT);
  pinMode(Pres_Pin, INPUT);
  pinMode(Light_Pin, INPUT);
  pinMode(Key_Pin, INPUT);
  pinMode(Button1_Pin, INPUT);
  pinMode(Button2_Pin, INPUT);
  pinMode(Buzz_Pin, OUTPUT);
  

  //Recuperamos la configuracion guardada de la Eeprom

  Mode = i2c_eeprom_read_byte(Eeprom_Address,0x08);    //Leemos el modo de funcionamiento guardado en la EEPROM
  X_pass = i2c_eeprom_read_byte(Eeprom_Address,0xA0);   //Contraseña x y z
  Y_pass = i2c_eeprom_read_byte(Eeprom_Address,0xA1);
  Z_pass = i2c_eeprom_read_byte(Eeprom_Address,0xA2);
  X_sense = i2c_eeprom_read_byte(Eeprom_Address,0xA4);
  Y_sense = i2c_eeprom_read_byte(Eeprom_Address,0xA5);
  Z_sense = i2c_eeprom_read_byte(Eeprom_Address,0xA6);


}

void loop() 
{

  if(Serial1.available())
  {
    Serial_menu();
  }

  if(Menu == 0)
  {
    switch(Mode)
    {
      case 1:
        Moving_psw();
        break;

      case 2:  
      break;
    }
  }

}








void Serial_menu(void)
{
  char SerialIn = Serial1.read();


   switch(SerialIn)
   {
     case 'm':
      Menu = 1;
      Serial1.println("-----------------------------------------");
      Serial1.println("-- Seleccion de modo de funcionamiento --");
      Serial1.println("-----------------------------------------");
      Serial1.println("1. Contraseña con movimientos");
      Serial1.println("2. Traslado sin vibraciones");
      Serial1.println("3. RFID Tag");
      Serial1.println("4. Pollito ingles");
      Serial1.println("8. Prueba de sensores y actuadores");
      Serial1.println("9. Mostrar configuracion");
      Serial1.println("x. Salir del Menu Serial");
      break;

     case '1':
      if(Menu==1)
      {
        Moving_psw_setup();
        break;
      }
      else if(Menu==8)
      {
        Accel_test();
        
        break;
      }
      

     case '2':
      if(Menu==1)
      {
        //TNT_setup();
        break;
      }
      else if(Menu==8)
      {
       Giro_test();
       break;
       
      }

     case '3':
      if(Menu==1)
      {
        //RFID_setup();
        break;
      }
      else if(Menu==8)
      {
        PIR_test();
        
        break;
      }

     case '4':
      if(Menu==1)
      {
        //Pollito_setup();
        break;
      }
      else if(Menu==8)
      {
        US_test();
        
        break;
      }

     case '5':
        if(Menu==1)
        {
         Serial1.println("Seleccion no valida");
         break;
        }
        else if(Menu==8)
        {
         Light_test();
         
        break;
        }

        case '6':
        if(Menu==1)
        {
         Serial1.println("Seleccion no valida");
         break;
        }
        else if(Menu==8)
        {
         RFID_test();
         break;
        }

        case '7':
        if(Menu==1)
        {
         Serial1.println("Seleccion no valida");
         break;
        }
        else if(Menu==8)
        {
         Key_test();
         
        break;
        }
     case '8':
      
      if(Menu==1)
      {
        Menu = 8;
        Check_sensor();
        break;
      }
      else if(Menu==8)
      {
        RGB_test();
        
        break;
      }

      case '9':
      
      if(Menu==1)
      {
        Show_config();
        break;
      }
      else if(Menu==8)
      {
        Display_test();
        
        break;
      }

      case'x':
        Menu = 0;
        return;

      default:
        Serial1.println("Error en la seleccion. Intentalo de nuevo");
        delay(10);

   }
   }
   

void Moving_psw_setup(void)                     //Funcion de configuracion del modo 1   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
{
  Serial1.println("----------------------------------------------------");
  Serial1.println("-- Configuracion de la contraseña de movimientos  --");
  Serial1.println("----------------------------------------------------");
  Serial1.println("La contraseña almacenada es:");
  Serial1.print("X: ");
  Serial1.println(X_pass);
  Serial1.print("Y: ");
  Serial1.println(Y_pass);
  Serial1.print("Z: ");
  Serial1.println(Z_pass);
  Serial1.println("¿Quieres modificar la contraseña (s/n)?");
  while(Serial1.available() <= 0);                                  //Esperamos hasta que recibamos un dato por Serial1
  while(Serial1.available())
  {
    char SerialIn = Serial1.read();
    switch(SerialIn)
    {
      case 'n':
      break;

      case 's':
      Serial1.println("Introduce un valor para X");
      while(Serial1.available() <= 0);                                  //Esperamos hasta que recibamos un dato por Serial1
      while(Serial1.available())
      {
        X_pass = Serial1.parseInt();
        i2c_eeprom_write_byte(Eeprom_Address,0xA0, X_pass);
        delay(10);
      }
      Serial1.println("Introduce un valor para Y");
      while(Serial1.available() <= 0);                                  //Esperamos hasta que recibamos un dato por Serial1
      while(Serial1.available())
      {
        Y_pass = Serial1.parseInt();
        i2c_eeprom_write_byte(Eeprom_Address,0xA1, Y_pass);
        delay(10);
      }
      Serial1.println("Introduce un valor para Z");
      while(Serial1.available() <= 0);                                  //Esperamos hasta que recibamos un dato por Serial1
      while(Serial1.available())
      {
        Z_pass = Serial1.parseInt();
        i2c_eeprom_write_byte(Eeprom_Address,0xA2, Z_pass);
        delay(10);
      }
     
      
      
      Serial1.println("Contraseña guardada");
      Menu = 0;

      return;
      break;

      default:
      return;
    }
  } 
  Mode = 1;
  Serial1.println("Ya puedes empezar");
  i2c_eeprom_write_byte(Eeprom_Address, 0x08, Mode);
  
  return;
}



void TNT_setup(void)                            //Funcion de configuracion del modo 2
{

}


void RFID_setup(void)                           //Funcion de configuracion del modo 3
{

}

void Pollito_setup(void)                         //Funcion de configuracion del modo 4
{

}

void Check_sensor(void)                         //Funcion para comprobar el funcionamiento de los sensores
{
      Serial1.println("----------------------------------------");
      Serial1.println("-- Selecciona el sensor o dispositivo --");
      Serial1.println("----------------------------------------");
      Serial1.println("1. Acelerometro");
      Serial1.println("2. Giroscopio");
      Serial1.println("3. PIR");
      Serial1.println("4. US");
      Serial1.println("5. Luminosidad");
      Serial1.println("6. RFID");
      Serial1.println("7. Llave");
      Serial1.println("8. Led RGB y alarma");
      Serial1.println("9. Display y pulsadores");
      Serial1.println("m. Volver al menu anterior");
      Serial1.println("x. Salir del Menu Serial");
      
      
      
}

void Show_config(void)                             //Funcion para mostrar la configuracion guardada en la EEPROM
{
      Serial1.println("----------------------------");
      Serial1.println("-- Configuracion del cubo --");
      Serial1.println("----------------------------");
      Serial1.print("El identificador del cubo es: ");
      Serial1.println(ID);
      Serial1.print("El modo de funcionamiento es: ");
      Serial1.println(Mode);
      Serial1.println("La contraseña XYZ es: ");
      Serial1.print("X: ");
      Serial1.println(X_pass);
      Serial1.print("Y: ");
      Serial1.println(Y_pass);
      Serial1.print("Z: ");
      Serial1.println(Z_pass);
   }

void Moving_psw(void)                           //PENDIENTE DE PROBAR XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
{
  float X_accel;                      //Variables para la lectura del acelerometro
  float Y_accel;
  float Z_accel;

  int start = 0;
  

  int X_count = 0;                        //Variables para contar los movimientos de los ejes
  int Y_count = 0;
  int Z_count = 0;

  
  button1_state = digitalRead(Button1_Pin);
  delay(100);
  
  
  if(button1_state == HIGH)
  {
    Serial1.println("empezamos");
    display.setTextColor(WHITE); 
    display.setTextSize(2);
    display.setCursor(40,8);
    display.print("START");
    display.display();
    display.clearDisplay();
    delay(2000);
    start = 1;
    do
    {
      
      if (IMU.accelerationAvailable()) 
      {
        IMU.readAcceleration(X_accel, Y_accel, Z_accel);
        if(X_accel >= 1.5)
        {
          X_count++;
          beep(50,1);
        }
        if(Y_accel >= 1.5)
        {
          Y_count++;
          beep(50,1);
        }
        if(Z_accel <= -1.5)
        {
          Z_count++;
          beep(50,1);
        } 

        if(X_count >= 10)
        {
         X_count = 0;
        }
        if(Y_count >= 10)
        {
          Y_count = 0;
        }
        if(Z_count >= 10)
        {
          Z_count = 0;
        }
        
        display.setCursor(20,8);
        display.print(X_count);
        display.setCursor(60,8);
        display.print(Y_count);
        display.setCursor(100,8);
        display.print(Z_count);
        display.display();
        display.clearDisplay();
          
        button2_state = digitalRead(Button2_Pin);
        delay(100);
        if(button2_state == HIGH)
        {
          start = 0;
          Serial1.println(X_count);
          Serial1.println(Y_count);
          Serial1.println(Z_count);
          if(X_count == X_pass && Y_count == Y_pass && Z_count == Z_pass)
          {
            led(Green);
            Serial1.println("Has acertado");
            display.setCursor(15,8);
            display.print("CORRECTO");
            display.display();
            display.clearDisplay();
            beep(50, 5);
          }
          else
          {
            led(Red);
            Serial1.println("Has fallado. Intentalo de nuevo");
            display.setCursor(40,8);
            display.print("ERROR");
            display.display();
            display.clearDisplay();
            beep(200, 1);
          }
        }
      }
      
    }while(start == 1);
  }

   
}


void TNT(void)                                  //Funcion de modo 2
{
  Serial1.println("TNT");
}

void RFID(void)                                 //Funcion de modo 3
{
  Serial1.println("RFID");
}

void Pollito(void)                               //Funcion de modo 4
{
  Serial1.println("Pollito");
}

//Funciones para testear los sensores

void Accel_test(void)
{
  Serial1.println("Accel_test");
}

void Giro_test(void)
{
  Serial1.println("Giro_test");
}

void PIR_test(void)
{
  Serial1.println("PIR_test");
}

//Funcion para la prueba del sensor de US
void US_test(void)
{
  
  Serial1.println("Situa el sensor a un a distancia conocida de un objeto");
  Serial1.print(".");
  delay(500);
  Serial1.print(".");
  delay(500);
  Serial1.print(".");
  delay(500);
  Serial1.print(".");
  delay(500);
  Serial1.print(".");
  delay(500);
  Serial1.println(".");
  delay(500);

  cm = US(Trig_Pin, Echo_Pin);
  Serial1.print("Distancia al objeto: ");
  Serial1.print(cm);
  Serial1.println("cm");
  delay(1000);
  
}

void Light_test(void)
{
  int Light;
  Serial1.println("Realizamos 10 medidas en 5 segundos:");
  for(int i = 0 ; i < 10 ; i++ )
  {
    Light = analogRead(Light_Pin);
    Light = map(Light, 0, 100, 0, 255);
    Serial1.print("Sensor de luz al ");
    Serial1.print(Light);
    Serial1.println("%");
    delay(500);
  }
  
}

void RFID_test(void)                          //NO FUNCIONA
{
  Serial1.println("RFID_test");
}

void Key_test(void)
{
  
  int Key = digitalRead(Key_Pin);
  switch(Key){
    case HIGH:
      Serial1.println("La llave esta en posicion verde");
      break;
    case LOW:  
      Serial1.println("La llave esta en posicion roja");
      break;
  }

}

//Funcion para probar el led RGB
void RGB_test(void)                             
{
  led(Red);
  delay(500);
  led(Green);
  delay(500);
  led(Blue);
  delay(500);
  led(Off);
  beep(50, 2);
  beep(200, 1);

}

void Display_test(void)
{
  bool button1_state;
  bool button2_state;

  button1_state = digitalRead(Button1_Pin);
  delay(2);
  button2_state = digitalRead(Button2_Pin);
  delay(2);
  if(button1_state == HIGH) 
  {
    Serial1.println("El pulsador 1 está pulsado");
  }
  if(button2_state == HIGH) 
  {
    Serial1.println("El pulsador 2 está pulsado");
  }
  
  

}


//Funcion para la medida de distancia con US
int US(int Trigger, int Echo)
{
  long duration, distanceCm;
   
  digitalWrite(Trigger, LOW);  //para generar un pulso limpio ponemos a LOW 4us
  delayMicroseconds(4);
  digitalWrite(Trigger, HIGH);  //generamos Trigger (disparo) de 10us
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);
   
  duration = pulseIn(Echo, HIGH);  //medimos el tiempo entre pulsos, en microsegundos
   
  distanceCm = duration * 10 / 292/ 2;   //convertimos a distancia, en cm
  return distanceCm;
}
    
  
  

  
//Funcion para la lectura de un byte de la memoria EEPROM externa
byte i2c_eeprom_read_byte(int deviceaddress, unsigned int eeaddress) 
{
   byte rdata = 0xFF;
   Wire.beginTransmission(deviceaddress);
   Wire.write((int)(eeaddress >> 8)); // MSB
   Wire.write((int)(eeaddress & 0xFF)); // LSB
   Wire.endTransmission();
   Wire.requestFrom(deviceaddress, 1);
   if (Wire.available()) rdata = Wire.read();
   return rdata;
}

//Funcion para la escritura de un byte en la memoria EEPROM externa
void i2c_eeprom_write_byte(int deviceaddress, unsigned int eeaddress, byte data) 
{
   int rdata = data;
   Wire.beginTransmission(deviceaddress);
   Wire.write((int)(eeaddress >> 8)); // MSB
   Wire.write((int)(eeaddress & 0xFF)); // LSB
   Wire.write(rdata);
   Wire.endTransmission();
   delay(10);
}



void beep(int time, int repeat)                               //Control de pitido
{
                                    
  for(int i = 0; i < repeat; i++)  
  {
    digitalWrite(Buzz_Pin, HIGH);
    delay(time);
    digitalWrite(Buzz_Pin, LOW);
    delay(time);
  }                                 

}

void led(byte color)
{
  switch(color)
  {
    case 0:
      digitalWrite(RLed_Pin, 0);                    //Apagamos el led
      digitalWrite(GLed_Pin, 0);
      digitalWrite(BLed_Pin, 0);
      break;
    case 1:  
      digitalWrite(RLed_Pin, 255);                    //Rojo
      digitalWrite(GLed_Pin, 0);
      digitalWrite(BLed_Pin, 0);
      break;
    case 2:  
      digitalWrite(RLed_Pin, 0);                    //Verde
      digitalWrite(GLed_Pin, 255);
      digitalWrite(BLed_Pin, 0);
      break;
    case 3:  
      digitalWrite(RLed_Pin, 0);                    //Azul
      digitalWrite(GLed_Pin, 0);
      digitalWrite(BLed_Pin, 255);
      break;  
    case 4:  
      digitalWrite(RLed_Pin, 255);                    //Blanco
      digitalWrite(GLed_Pin, 255);
      digitalWrite(BLed_Pin, 255);
      break; 
    case 5:  
      digitalWrite(RLed_Pin, 255);                    //Amarillo
      digitalWrite(GLed_Pin, 255);
      digitalWrite(BLed_Pin, 0);
      break;   
    case 6:  
      digitalWrite(RLed_Pin, 255);                    //Rosa
      digitalWrite(GLed_Pin, 0);
      digitalWrite(BLed_Pin, 255);
      break;        
  }
}