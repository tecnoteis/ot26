double nivel;
double Turbidez;
double TDS;

double fnc_ultrasonic_distance(int _t, int _e){
	unsigned long dur=0;
	digitalWrite(_t, LOW);
	delayMicroseconds(5);
	digitalWrite(_t, HIGH);
	delayMicroseconds(10);
	digitalWrite(_t, LOW);
	dur = pulseIn(_e, HIGH, 18000);
	if(dur==0)return 999.0;
	return (dur/57);
}

void setup()
{
  	pinMode(16, OUTPUT);
	pinMode(17, INPUT);
	pinMode(27, OUTPUT);

	Serial.begin(115200);
	Serial.flush();
	while(Serial.available()>0)Serial.read();

	pinMode(2, INPUT);
	pinMode(4, INPUT);

}


void loop()
{
	yield();

  	nivel = fnc_ultrasonic_distance(16,17);
  	Serial.println(String("Nivel:")+String(nivel)+String("cm"));
  	if ((nivel >= 11)) {
  		digitalWrite(27,HIGH);
  	}
  	else {
  		if ((nivel <= 11)) {
  			digitalWrite(27,LOW);
  		}
  		else if ((nivel >= 11)) {
  			Turbidez = ((float)analogRead(2));
  			Serial.println(String("Turbidez:")+String((map(Turbidez, 4095,15,0,100)))+String("%"));
  		}
  		else {
  			TDS = ((float)analogRead(4));
  			Serial.println(String("TDS:")+String((map(TDS, 0,4096,0,1000)))+String("ppm"));
  		}

  	}

  	delay(1000);

}
