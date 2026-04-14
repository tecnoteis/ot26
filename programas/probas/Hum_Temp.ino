#include "ABlocks_DHT.h"
#include <Wire.h>
#include <math.h>
#include "AHT20.h"
#include <ESP32Servo.h>

double choiva;
double Temp_exterior;
DHT dht2(2,DHT11);
AHT20 sensor_th_aht20;
Servo servo_12;

double fnc_sensor_th_aht20(int _type){
  sensor_th_aht20.available();
  if(_type==1)return sensor_th_aht20.getTemperature();
  if(_type==2)return sensor_th_aht20.getHumidity();
}

void setup()
{
  	pinMode(12, INPUT);
	pinMode(2, INPUT);
	servo_12.attach(12);

	Serial.begin(115200);
	Serial.flush();
	while(Serial.available()>0)Serial.read();

	dht2.begin();
	Wire.begin();
	sensor_th_aht20.begin();

}


void loop()
{
	yield();

  	choiva = ((float)map(analogRead(12),0,4095,0,100));
  	Temp_exterior = dht2.readTemperature();
  	if ((dht2.readTemperature() > 23)) {
  		if (((fnc_sensor_th_aht20(1) < dht2.readTemperature()) && (choiva < 100))) {
  			servo_12.write(90);
  		}
  		else {
  			servo_12.write(0);
  		}

  	}
  	else {
  		servo_12.write(0);
  	}

}
