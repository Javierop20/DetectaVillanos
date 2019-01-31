/*
 * Modificación del ejemplo de Line Follower basado en PID (cálculo de errores sucesivos con integración y rectificación). 
 * En el GPIO, mandamos un flag de 1 desde Arduino si ha encontrado algo y de 0 si no hay nada delante de él.
 */

#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanMotors.h>
#include <OrangutanAnalog.h>
#include <OrangutanLEDs.h>
#include <OrangutanLCD.h>
#include <OrangutanPushbuttons.h>
#include <OrangutanBuzzer.h>
#define MELODY_LENGTH 95
Pololu3pi robot;
unsigned int sensors[5]; 
unsigned char currentIdx=0;
OrangutanLCD lcd;
OrangutanPushbuttons buttons;
OrangutanBuzzer buzzer;

#include <avr/pgmspace.h>

const char welcome_line1[] PROGMEM = " Pololu";
const char welcome_line2[] PROGMEM = "3\xf7 Robot";
const char demo_name_line1[] PROGMEM = "Evitador";
const char demo_name_line2[] PROGMEM = "de linea";

int maxv=140;
const char welcome[] PROGMEM = ">g32>>c32";
const char go[] PROGMEM = "L16 cdegreg4";


const char levels[] PROGMEM = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

unsigned char note[MELODY_LENGTH] =  //Super Mario Star Theme, escrita por el equipo
{
  NOTE_C(5), SILENT_NOTE, NOTE_C(5), SILENT_NOTE, NOTE_C(5), SILENT_NOTE, NOTE_C(5), SILENT_NOTE, NOTE_C(5), SILENT_NOTE, NOTE_A(4), SILENT_NOTE, NOTE_C(5),
  SILENT_NOTE, NOTE_C(5), SILENT_NOTE, NOTE_B(4), SILENT_NOTE, NOTE_B(4), SILENT_NOTE, NOTE_B(4), SILENT_NOTE, NOTE_B(4), SILENT_NOTE,
  NOTE_B(4), SILENT_NOTE, NOTE_A(4), SILENT_NOTE, NOTE_B(4), SILENT_NOTE, NOTE_B(4), SILENT_NOTE, NOTE_C(5), SILENT_NOTE, NOTE_C(5), SILENT_NOTE, NOTE_C(5), SILENT_NOTE, NOTE_C(5), SILENT_NOTE, NOTE_C(5), SILENT_NOTE, NOTE_A(4), SILENT_NOTE, NOTE_C(5),
  SILENT_NOTE, NOTE_C(5), SILENT_NOTE, NOTE_B(4), SILENT_NOTE, NOTE_B(4), SILENT_NOTE, NOTE_B(4), SILENT_NOTE, NOTE_B(4), SILENT_NOTE,
  NOTE_B(4), SILENT_NOTE, NOTE_A(4), SILENT_NOTE, NOTE_B(4), SILENT_NOTE, NOTE_B(4), SILENT_NOTE
};
unsigned int duration[MELODY_LENGTH] = //Duración de cada nota
{
  175, 10, 175, 10, 275, 10, 175, 10, 175,10, 125,10, 125, 10, 125,10, 175, 10, 175, 10, 275, 10, 175, 10, 175,10, 125,10, 125, 10, 125,10, 175, 10, 175, 10, 275, 10, 175, 10, 175,10, 125,10, 125, 10, 125,10, 175, 10, 175, 10, 275, 10, 175, 10, 175,10, 125,10, 125, 10, 125,10
};





//Display inicial
void load_custom_characters()
{
  OrangutanLCD::loadCustomCharacter(levels + 0, 0); 
  OrangutanLCD::loadCustomCharacter(levels + 1, 1); 
  OrangutanLCD::loadCustomCharacter(levels + 2, 2); 
  OrangutanLCD::loadCustomCharacter(levels + 3, 3);
  OrangutanLCD::loadCustomCharacter(levels + 4, 4);
  OrangutanLCD::loadCustomCharacter(levels + 5, 5);
  OrangutanLCD::loadCustomCharacter(levels + 6, 6);
  OrangutanLCD::clear(); 
}


void display_readings(const unsigned int *calibrated_values)
{
  unsigned char i;

  for (i=0;i<5;i++) {
    
    const char display_characters[10] = { ' ', 0, 0, 1, 2, 3, 4, 5, 6, 255 };

    
    char c = display_characters[calibrated_values[i] / 101];

   
    OrangutanLCD::print(c);
  }
}


void setup() //Se ejecuta una única vez, usado para calibrar los sensores, con una pequeña interfaz de usuario
{
  unsigned int counter; // used as a simple timer

  
  robot.init(2000);

  load_custom_characters(); 


  OrangutanLCD::printFromProgramSpace(welcome_line1);
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::printFromProgramSpace(welcome_line2);
  OrangutanBuzzer::playFromProgramSpace(welcome);
  delay(1000);

  OrangutanLCD::clear();
  OrangutanLCD::printFromProgramSpace(demo_name_line1);
  OrangutanLCD::gotoXY(0, 1);
  OrangutanLCD::printFromProgramSpace(demo_name_line2);
  delay(1000);

  
  while (!OrangutanPushbuttons::isPressed(BUTTON_B))
  {
    int bat = OrangutanAnalog::readBatteryMillivolts();

    OrangutanLCD::clear();
    OrangutanLCD::print(bat);
    OrangutanLCD::print("mV");
    OrangutanLCD::gotoXY(0, 1);
    OrangutanLCD::print(OrangutanAnalog::read(PC5));

    delay(100);
  }

  
  OrangutanPushbuttons::waitForRelease(BUTTON_B);
  delay(1000);

  
  for (counter=0; counter<80; counter++)
  {
    if (counter < 20 || counter >= 60)
      OrangutanMotors::setSpeeds(40, -40);
    else
      OrangutanMotors::setSpeeds(-40, 40);

    
    robot.calibrateLineSensors(IR_EMITTERS_ON);

    
    delay(20);
  }
  OrangutanMotors::setSpeeds(0, 0);

  
  while (!OrangutanPushbuttons::isPressed(BUTTON_B))
  {
   
    unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON);

    
    OrangutanLCD::clear();
    OrangutanLCD::print(position);
    OrangutanLCD::gotoXY(0, 1);
    display_readings(sensors);

    delay(100);
  }
  OrangutanPushbuttons::waitForRelease(BUTTON_B);

  OrangutanLCD::clear();

  OrangutanLCD::print("Vamos!");    

  
  OrangutanBuzzer::playFromProgramSpace(go);
  while(OrangutanBuzzer::isPlaying());
}


void loop()
{

  unsigned int position = robot.readLine(sensors, IR_EMITTERS_ON);

  if ((position == 4000  || position ==0) && OrangutanAnalog::read(PC5)<100) //Si la posición no está en los extremos y no hay flag mandado por Arduino, continúa a máxima velocidad
  {
    
    OrangutanMotors::setSpeeds(maxv, maxv);
    OrangutanLEDs::left(HIGH);
    OrangutanLEDs::right(HIGH);
  }
  else if (position < 2000 && OrangutanAnalog::read(PC5)<100)//Si se acerca al extremo izquierdo, gira a la derecha
  {
    
    OrangutanMotors::setSpeeds(maxv, maxv/4);
    OrangutanLEDs::left(HIGH);
    OrangutanLEDs::right(LOW);
  }
  
  else if(OrangutanAnalog::read(PC5)>100){ //Flag del Arduino, escribiendo en el GPIO (PC5) que hay algo delante
    OrangutanMotors::setSpeeds(0,0);
    OrangutanLEDs::left(LOW);
    OrangutanLEDs::right(HIGH);
  }
  else
  {                                   //Último caso, girar a la izquierda
    
    OrangutanMotors::setSpeeds(maxv/4, maxv); 
    OrangutanLEDs::left(LOW);
    OrangutanLEDs::right(HIGH);
  }

    //Tocar la canción en bucle. A la hora de probarlo, comentar estas líneas

    if (currentIdx < MELODY_LENGTH && !buzzer.isPlaying())
    {
    buzzer.playNote(note[currentIdx], duration[currentIdx], 15);
    currentIdx++;
    }
    
    

  
}

 





  


