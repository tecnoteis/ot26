#include "WiFi.h"
#include "PubSubClient.h"
#include <MicroMatrix.h>
#include <ESP32Servo.h>

double led;
double rele;
double servo2;
double servo22;
double servovalvula;
double consumo_electrico;
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
MicroMatrix micromatrix_steamakers;
Servo servo_25;
Servo servo_16;
Servo servo_32;

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
	if(espmqtt_topic.equals(String(String("fabrica/actuadores/servov1"))))servo2=espmqtt_msg_number;
	if(espmqtt_topic.equals(String(String("fabrica/actuadores/servov2"))))servo22=espmqtt_msg_number;
	if(espmqtt_topic.equals(String(String("fabrica/actuadores/servovalvula"))))servovalvula=espmqtt_msg_number;
	if(espmqtt_topic.equals(String(String("fabrica/actuadores/rele"))))rele=espmqtt_msg_number;
	if(espmqtt_topic.equals(String(String("fabrica/actuadores/consumoelectrico"))))consumo_electrico=espmqtt_msg_number;
	if(espmqtt_topic.equals(String(String("fabrica/actuadores/Led"))))led=espmqtt_msg_number;
	espmqtt_onreceive();
}

void espmqtt_subscribe(){
	espmqtt_client.subscribe(String(String("fabrica/actuadores/servov1")).c_str());
	espmqtt_client.subscribe(String(String("fabrica/actuadores/servov2")).c_str());
	espmqtt_client.subscribe(String(String("fabrica/actuadores/servovalvula")).c_str());
	espmqtt_client.subscribe(String(String("fabrica/actuadores/rele")).c_str());
	espmqtt_client.subscribe(String(String("fabrica/actuadores/consumoelectrico")).c_str());
	espmqtt_client.subscribe(String(String("fabrica/actuadores/Led")).c_str());
	espmqtt_client.subscribe(String(String("fabrica/sensores/humedad")).c_str());
	espmqtt_client.subscribe(String(String("fabrica/sensores/luz")).c_str());
	espmqtt_client.subscribe(String(String("fabrica/sensores/choiba")).c_str());
	espmqtt_client.subscribe(String(String("fabrica/sensores/temperatura")).c_str());
}

void espmqtt_setup_str(String _broker, int _port, String _u, String _p, String _id){
	espmqtt_broker=_broker;
	espmqtt_port=_port;
	espmqtt_user=_u;
	espmqtt_pass=_p;
	espmqtt_clientid=_id;
	espmqtt_setup();
}

void setup()
{
  	pinMode(19, OUTPUT);
	pinMode(5, OUTPUT);
	servo_25.attach(25);
	servo_16.attach(16);
	servo_32.attach(32);

	espwifi_setup(String(""),String(""));
	espmqtt_setup_str(String("domoteis.iesteis.es"),1883,String("maquina1"),String("teismaqueta1"),String("esp32n1"));
	micromatrix_steamakers.Clear();
	micromatrix_steamakers.SetBrightness(100);
	if ((WiFi.status() == WL_CONNECTED)) {
		micromatrix_steamakers.TextScroll(String("Conectado"),CRGB(255,0,0),150);
	}
	else {
		micromatrix_steamakers.TextScroll(String("No"),CRGB(255,0,0),150);
	}


}


void loop()
{
	yield();

	espwifi_check();

	espmqtt_loop();
  	if ((led == 1)) {
  		digitalWrite(19, HIGH);
  	}

  	if ((led == 0)) {
  		digitalWrite(19, LOW);
  	}

  	if ((rele == 1)) {
  		digitalWrite(5,HIGH);
  	}

  	if ((rele == 0)) {
  		digitalWrite(5,LOW);
  	}

  	if ((servo2 == 0)) {
  		servo_25.write(90);
  	}

  	if ((servo2 == 1)) {
  		servo_25.write(0);
  	}

  	if ((servo22 == 0)) {
  		servo_16.write(90);
  	}

  	if ((servo22 == 1)) {
  		servo_16.write(0);
  	}

  	if ((servovalvula == 0)) {
  		servo_32.write(90);
  	}

  	if ((servovalvula == 1)) {
  		servo_32.write(0);
  	}

}