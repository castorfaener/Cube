#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>


char SerialIn;                    //
int mode;                         //Modo de funcionamiento

int cm;                           //distancia del sensor de US

const int RLed_Pin = 5;               //Definimos los pines del Led RGB
const int GLed_Pin = 4;
const int BLed_Pin = 2;

const int Trig_Pin = A1;              //Definimos los pines del sensor de US
const int Echo_Pin = A0;

const int Pres_Pin = A2;              //Definimos el pin del sensor de presencia

const int Light_Pin = A3;             //Definimos el pin del sensor de luz

const int Key_Pin = A6;               //Definimos el pin del interruptor de llave

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
  pinMode(Button3_Pin, INPUT);


}

void loop() 
{
  
  if (Serial1.available()) 
  {
    SerialIn = Serial1.read();
    print_menu(SerialIn);
  }

}
void print_menu(char selection)
{
   switch(selection)
   {
     case 'm':
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
      break;

     case '1':
      //Moving_psw_setup();
      delay(10);
      break;

     case '2':
      //TNT_setup();
      delay(10);
      break;

     case '3':
      //RFID_setup();
      delay(10);
      break;

     case '4':
      //Pollito_setup();
      delay(10);
      break;

     case '8':
      Check_sensor();
      delay(10);
      break;

      case '9':
      Show_config();
      delay(10);
      break;

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

      while(!Serial1.available())
      {
        char i = Serial1.read();

        switch(i)
        {
          case '1':
            Accel_test();
            break;

          case '2':
            Giro_test();
            break;

          case '3':
            PIR_test();
            break;

          case '4':
            US_test();
            break;

          case '5':
            Light_test();
            break;

          case '6':
            RFID_test();
            break;

          case '7':
            Key_test();
            break;

          case '8':
            RGB_test();
            break;

          case '9':
            Display_test();
            break;    

          default:
            exit(0);
        }
      }
      
}

void Show_config(void)                             //Funcion para mostrar la configuracion guardada en la EEPROM
{
      Serial1.println("----------------------------");
      Serial1.println("-- Configuracion guardada --");
      Serial1.println("----------------------------");
}

void Moving_psw(void)                           //Funcion de modo 1
{

}

void TNT(void)                                  //Funcion de modo 2
{

}

void RFID(void)                                 //Funcion de modo 3
{

}

void Pollito(void)                               //Funcion de modo 4
{

}

//Funciones para testear los diferentes sensores y elementos

void Accel_test(void)
{

}

void Giro_test(void)
{

}

void PIR_test(void)
{

}

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

}

void RFID_test(void)
{
  
}

void Key_test(void)
{

}

void RGB_test(void)                             //Funcion para probar el led RGB
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

}

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