#include "WiFi.h"
#include "PubSubClient.h"
#include <ESP32Servo.h>

double Turbidez;
double servoval;
double TDS;
double releb;
double Nivel;
String espwifi_ssid="";
String espwifi_pass="";
unsigned long espwifi_reconnect_last_time = 0;
String espmqtt_broker="";
int espmqtt_port=1883;
String espmqtt_user="";
String espmqtt_pass="";
String espmqtt_clientid="";
unsigned long espmqtt_reconnect_last_time = 0;
WiFiClient espmqtt_wifiClient;
PubSubClient espmqtt_client(espmqtt_wifiClient);
String espmqtt_topic="";
String espmqtt_msg_str="";
double espmqtt_msg_number=0;
char espmqtt_payload[128];
Servo servo_12;

void espwifi_check() {
    if (WiFi.status() != WL_CONNECTED) {
        if( (millis() - espwifi_reconnect_last_time) >= 5000) {
            espwifi_reconnect_last_time = millis();
            WiFi.reconnect();
        }
    }
}

void espwifi_setup(String _ssid, String _pass){
	WiFi.mode(WIFI_STA);
	espwifi_ssid=_ssid;
	espwifi_pass=_pass;
	WiFi.begin(_ssid.c_str(),_pass.c_str());
	unsigned long wc = millis();
	while ( (WiFi.status() != WL_CONNECTED) && ( (millis()-wc) < 10000) ) delay(250);
}

void espmqtt_onreceive(){};

void espmqtt_setup(){
	delay(10);
	randomSeed(micros());
	espmqtt_client.setServer(espmqtt_broker.c_str(), espmqtt_port);
	espmqtt_client.setCallback(espmqtt_callback);
	espmqtt_subscribe();
}

void espmqtt_loop(){
	if (!espmqtt_client.connected()) {
		if( (millis()-espmqtt_reconnect_last_time)>5000 ){
			espmqtt_client.connect(espmqtt_clientid.c_str(),espmqtt_user.c_str(),espmqtt_pass.c_str());
			espmqtt_subscribe();
			espmqtt_reconnect_last_time = millis();
		}
	}
	if(espmqtt_client.connected()) {

		espmqtt_client.loop();
	}
}

double espmqtt_payload2double(unsigned char *_payload, int _length){
	int i;
	for (i = 0; i<_length && i<128; i++){
		espmqtt_payload[i] = _payload[i];
	}
	espmqtt_payload[i] = 0;
	return atof(espmqtt_payload);
}

String espmqtt_payload2string(unsigned char *_payload, int _length){
	int i;
	for (i = 0; i<_length && i<128; i++){
		espmqtt_payload[i] = _payload[i];
	}
	espmqtt_payload[i] = 0;
	return String(espmqtt_payload);
}

void espmqtt_callback(char* _topic, unsigned char* _payload, unsigned int _payloadlength){
	espmqtt_topic=String(_topic);
	espmqtt_msg_str=espmqtt_payload2string(_payload,_payloadlength);
	espmqtt_msg_number=espmqtt_payload2double(_payload,_payloadlength);
	if(espmqtt_topic.equals(String(String("olimpiada/actuadores/servoval"))))servoval=espmqtt_msg_number;
	if(espmqtt_topic.equals(String(String("olimpiada/actuadores/releb"))))releb=espmqtt_msg_number;
	espmqtt_onreceive();
}

void espmqtt_subscribe(){
	espmqtt_client.subscribe(String(String("olimpiada/actuadores/servoval")).c_str());
	espmqtt_client.subscribe(String(String("olimpiada/actuadores/releb")).c_str());
}

void espmqtt_setup_str(String _broker, int _port, String _u, String _p, String _id){
	espmqtt_broker=_broker;
	espmqtt_port=_port;
	espmqtt_user=_u;
	espmqtt_pass=_p;
	espmqtt_clientid=_id;
	espmqtt_setup();
}
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

void topicsActuadores() {
}
void EnvioDatos() {
	espmqtt_client.publish(String(String("olimpiada/sensores/tds")).c_str(),String(TDS).c_str(),false);
	espmqtt_client.publish(String(String("olimpiada/sensores/turb")).c_str(),String(Turbidez).c_str(),false);
	espmqtt_client.publish(String(String("olimpiada/sensores/nivel")).c_str(),String(Nivel).c_str(),false);
}

void setup()
{
  	pinMode(16, OUTPUT);
	pinMode(17, INPUT);
	pinMode(27, OUTPUT);
	servo_12.attach(12);

	Serial.begin(115200);
	Serial.flush();
	while(Serial.available()>0)Serial.read();

	pinMode(2, INPUT);
	pinMode(4, INPUT);

	espwifi_setup(String("S25Tecnoteis"),String("olimTele26"));
	espmqtt_setup_str(String("domoteis.iesteis.es"),1883,String("maqueta1"),String("teismaqueta1"),String("esp32p"));
	topicsActuadores();

}


void loop()
{
	yield();

	espwifi_check();

	espmqtt_loop();
  	Turbidez = ((float)analogRead(2));
  	TDS = ((float)analogRead(4));
  	Nivel = fnc_ultrasonic_distance(16,17);
  	if ((releb == 1)) {
  		digitalWrite(27,HIGH);
  	}
  	else {
  		digitalWrite(27,LOW);
  	}

  	if ((servoval == 1)) {
  		servo_12.write(90);
  	}
  	else {
  		servo_12.write(0);
  	}

  	EnvioDatos();
  	delay(500);

}