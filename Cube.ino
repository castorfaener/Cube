#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include "MAX1704X.h"




const int Eeprom_Address = 0x50;
byte Eeprom;

int Menu = 0;                       //Control de nivel del menu
int ID;                            //Identificador del cubo
int Mode;                         //Modo de funcionamiento
int X_pass;                       //Contraseña del modo XYZ
int Y_pass;
int Z_pass;

int cm;                           //distancia del sensor de US

const int RLed_Pin = 5;               //Definimos los pines del Led RGB
const int GLed_Pin = 3;
const int BLed_Pin = 2;

const int Trig_Pin = A1;              //Definimos los pines del sensor de US
const int Echo_Pin = A0;

const int Pres_Pin = A3;              //Definimos el pin del sensor de presencia

const int Light_Pin = A2;             //Definimos el pin del sensor de luz

//const int Key_Pin = ;               //Definimos el pin del interruptor de llave

const int Button1_Pin = 3;            //Definimos los pines de los tres pulsadores
const int Button2_Pin = 7;
const int Button3_Pin = 8;

const int RST_PIN = 9;              // Pin 9 para el reset del RC522
const int SS_PIN = 10;              // Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Crear instancia del MFRC522


void setup() 
{
  Serial1.begin(9600);                //Inicializamos Serial 1   

  SPI.begin();                        //Inicializamos SPI   
  mfrc522.PCD_Init();                 //Inicializamos el lector RFID
  Wire.begin();                       //Inicializamos puerto I2C
  
  pinMode(RLed_Pin, OUTPUT);            //Configuramos los pines de E/S
  pinMode(GLed_Pin, OUTPUT);
  pinMode(BLed_Pin, OUTPUT);
  pinMode(Trig_Pin, OUTPUT);
  pinMode(Echo_Pin, INPUT);
  pinMode(Pres_Pin, INPUT);
  pinMode(Light_Pin, INPUT);
  //pinMode(Key_Pin, INPUT);
  pinMode(Button1_Pin, INPUT);
  pinMode(Button2_Pin, INPUT);
  pinMode(Button3_Pin, INPUT);

  Mode = i2c_eeprom_read_byte(Eeprom_Address,0);    //Leemos el modo de funcionamiento guardado en la EEPROM

}

void loop() 
{

  


  if (Serial1.available()) 
  {
    Serial_menu();
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
      Serial1.println("5. ");
      Serial1.println("8. Prueba de sensores y displays");
      Serial1.println("9. Mostrar configuracion");
      Serial1.println("x. Salir del Menu Serial");
      break;

     case '1':
      if(Menu==1)
      {
        //Moving_psw_setup();
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

void Moving_psw_setup(void)                     //Funcion de configuracion del modo 1
{

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
      Serial1.println("8. Led RGB");
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

void Moving_psw(void)                           //Funcion de modo 1
{
  Serial1.println("Moving_psw");
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

//Funciones para testear los diferentes sensores y elementos

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
  Serial1.println("Light_test");
}

void RFID_test(void)
{
  Serial1.println("RFID_test");
}

void Key_test(void)
{
  Serial1.println("Key_test");
}

//Funcion para probar el led RGB
void RGB_test(void)                             
{
  digitalWrite(RLed_Pin, 255);                  //Encendemos el color Rojo
  digitalWrite(GLed_Pin, 0);
  digitalWrite(BLed_Pin, 0);
  delay(500);
  digitalWrite(RLed_Pin, 0);                     //Encendemos el color Verde
  digitalWrite(GLed_Pin, 255);
  digitalWrite(BLed_Pin, 0);
  delay(500);
  digitalWrite(RLed_Pin, 0);                    //Encendemos el color Azul
  digitalWrite(GLed_Pin, 0);
  digitalWrite(BLed_Pin, 255);
  delay(500);
  digitalWrite(RLed_Pin, 0);                    //Apagamos el led
  digitalWrite(GLed_Pin, 0);
  digitalWrite(BLed_Pin, 0);

}

void Display_test(void)
{
  Serial1.println("Display_test");
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
}