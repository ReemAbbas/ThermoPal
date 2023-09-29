#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//−−−−−−−−−−−−−−−−−Relay−−−−−−−−−−−−−−−−−−−−−−−−
const int relayPin =12;
unsigned long relayStart =−8000;

//−−−−−−−−−−Room Temperature Sensor−−−−−−−−−−−−−
const int roomTempSensorPin=A0;
float celsius;
float roomTempSensor;

//−−−−−−−−−−Peltier Temperature Sensor−−−−−−−−−−
float peltierTemp;
unsigned long peltierTempStart =−1000;

//−−−−−−−−−−Body Temperature Sensor−−−−−−−−−−−−−
#define ONE_WIRE_BUS 9
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float bodyTemp;
unsigned long bodyTempStart=−1000;

//−−−−−−−−−−−−−−−−−−−−LCD−−−−−−−−−−−−−−−−−−−−−−−−
Adafruit_PCD8544 display = Adafruit_PCD8544( 7 , 6 , 5 , 4 , 3);
//CLK−Din−DC−CE−RST

void setup() {
  pinMode( roomTempSensorPin , INPUT);
  pinMode( relayPin ,OUTPUT);
    
  Serial.begin(9600) ;
    
  sensors.begin();
  sensors.setResolution (11);
    
  display.begin();
  display.setContrast(40);
  display.clearDisplay();
}

void loop() {
//−−−−−−−−−−Room Temperature Sensor−−−−−−−−−−−−−
  roomTempSensor=analogRead(roomTempSensorPin);
  roomTempSensor=(roomTempSensor ∗ 4.88)/10;
  celsius=roomTempSensor; // Storing value in Degree Celsius
  display.setTextSize(1);
  display.setTextColor(BLACK,WHITE);
  display.setCursor(0,0);
  display.println( ”Room temp : ” );
  display.print(celsius);
  display.print((char)247);
  display.println( ”C” );
  display.display();

//−−−−−−−−−−Peltier Temperature Sensor−−−−−−−−−−
  if( millis()>= peltierTempStart+1000){
  peltierTempStart +=1000;
  sensors.requestTemperatures() ;
  peltierTemp=sensors.getTempCByIndex(0) ;
  display.println( ” Peltier temp : ” ) ;
  display.print( peltierTemp ) ;
  display.print((char)247) ;
  display.println( ”C” ) ;
  display.display() ;
  }
//−−−−−−−−−−Body Temperature Sensor−−−−−−−−−−−−−
  if( millis()>=bodyTempStart+1000){
  bodyTempStart+=1000;
  sensors.requestTemperatures();
  bodyTemp= sensors.getTempCByIndex(1) ;
  display.println( ”Body temp : ” ) ;
  display.print(bodyTemp) ;
  display.print(( char)247) ;
  display.print( ”C” ) ;
  display.display() ;
  }
//−−−−−−−−−−−−−−−−−Relay−−−−−−−−−−−−−−−−−−−−−−−−
  if ( peltierTemp >29){                                 // hot mode, we create pulses through toggling
    if ( millis()>= relayStart +8000){
      if ( digitalRead ( relayPin )==1){
          digitalWrite(relayPin ,LOW) ; // turn off
          relayStart +=14000;
      } else {
          digitalWrite( relayPin ,HIGH ) ; // turn on
          relayStart +=6000;
      }
    }
  } else {       // cold mode
  digitalWrite(relayPin ,HIGH ) ;
  }
}
