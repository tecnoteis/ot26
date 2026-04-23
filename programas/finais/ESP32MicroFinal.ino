#include "WiFi.h"
#include "PubSubClient.h"
#include <MicroMatrix.h>
#include <ESP32Servo.h>
#include "ABlocks_DHT.h"
#include <Wire.h>
#include <math.h>
#include "AHT20.h"
#include "ABLocks_TimerFreeTone.h"
#include "ABlocks_Button.h"

double temp_in;
double servo1;
double hum_in;
double servo2;
double luz;
double temp_ext;
double son;
double hum_ext;
double neo;
double alarma;
double nivel;
double poten;
double tds;
double turb;
double choiva;
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
DHT dht2(2,DHT11);
AHT20 sensor_th_aht20;
double micro_esp32ks_energy_WH=0.0;
Button button_debounced_17(17,50);

CRGB micromatrix_steamakers_bitmap[25];
Button button_debounced_0(0,50);

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
	if(espmqtt_topic.equals(String(String("olimpiada/actuadores/servo1"))))servo1=espmqtt_msg_number;
	if(espmqtt_topic.equals(String(String("olimpiada/actuadores/servo2"))))servo2=espmqtt_msg_number;
	if(espmqtt_topic.equals(String(String("olimpiada/actuadores/son"))))son=espmqtt_msg_number;
	if(espmqtt_topic.equals(String(String("olimpiada/actuadores/neo"))))neo=espmqtt_msg_number;
	if(espmqtt_topic.equals(String(String("olimpiada/actuadores/alarma"))))alarma=espmqtt_msg_number;
	if(espmqtt_topic.equals(String(String("olimpiada/sensores/tds"))))tds=espmqtt_msg_number;
	if(espmqtt_topic.equals(String(String("olimpiada/sensores/turb"))))turb=espmqtt_msg_number;
	if(espmqtt_topic.equals(String(String("olimpiada/sensores/nivel"))))nivel=espmqtt_msg_number;
	espmqtt_onreceive();
}

void espmqtt_subscribe(){
	espmqtt_client.subscribe(String(String("olimpiada/actuadores/servo1")).c_str());
	espmqtt_client.subscribe(String(String("olimpiada/actuadores/servo2")).c_str());
	espmqtt_client.subscribe(String(String("olimpiada/actuadores/son")).c_str());
	espmqtt_client.subscribe(String(String("olimpiada/actuadores/neo")).c_str());
	espmqtt_client.subscribe(String(String("olimpiada/actuadores/alarma")).c_str());
	espmqtt_client.subscribe(String(String("olimpiada/sensores/tds")).c_str());
	espmqtt_client.subscribe(String(String("olimpiada/sensores/turb")).c_str());
	espmqtt_client.subscribe(String(String("olimpiada/sensores/nivel")).c_str());
}

void espmqtt_setup_str(String _broker, int _port, String _u, String _p, String _id){
	espmqtt_broker=_broker;
	espmqtt_port=_port;
	espmqtt_user=_u;
	espmqtt_pass=_p;
	espmqtt_clientid=_id;
	espmqtt_setup();
}

double fnc_sensor_th_aht20(int _type){
  sensor_th_aht20.available();
  if(_type==1)return sensor_th_aht20.getTemperature();
  if(_type==2)return sensor_th_aht20.getHumidity();
}

double fnc_micro_esp32ks_consumption(int _t){  //0=V, 1=A, 2=W, 3=Wh

  if(_t==0)return 3.3;

  double acs_samples = 500;
  double amp_total=0.0;
  double amp_media,amp_real;

  for (int _i = 0; _i < acs_samples; _i++) {
  	amp_total += analogRead(36);
  }
  amp_media = (amp_total / acs_samples);
  amp_real = (amp_media * 0.0004278763)+ 0.043560611;

  if(_t==1) return amp_real;
  if(_t==2) return (amp_real*3.3);
  if(_t==3) return micro_esp32ks_energy_WH;

  return 0.0;
}


void fnc_micromatrix_esp32ks_bitmap(   CRGB _p0, CRGB _p1, CRGB _p2, CRGB _p3, CRGB _p4,
                                CRGB _p5, CRGB _p6, CRGB _p7, CRGB _p8, CRGB _p9,
                                CRGB _p10,CRGB _p11,CRGB _p12,CRGB _p13,CRGB _p14,
                                CRGB _p15,CRGB _p16,CRGB _p17,CRGB _p18,CRGB _p19,
                                CRGB _p20,CRGB _p21,CRGB _p22,CRGB _p23,CRGB _p24){

    //load matrix points into array
    micromatrix_steamakers_bitmap[0]=_p0;
    micromatrix_steamakers_bitmap[1]=_p1;
    micromatrix_steamakers_bitmap[2]=_p2;
    micromatrix_steamakers_bitmap[3]=_p3;
    micromatrix_steamakers_bitmap[4]=_p4;
    micromatrix_steamakers_bitmap[5]=_p5;
    micromatrix_steamakers_bitmap[6]=_p6;
    micromatrix_steamakers_bitmap[7]=_p7;
    micromatrix_steamakers_bitmap[8]=_p8;
    micromatrix_steamakers_bitmap[9]=_p9;
    micromatrix_steamakers_bitmap[10]=_p10;
    micromatrix_steamakers_bitmap[11]=_p11;
    micromatrix_steamakers_bitmap[12]=_p12;
    micromatrix_steamakers_bitmap[13]=_p13;
    micromatrix_steamakers_bitmap[14]=_p14;
    micromatrix_steamakers_bitmap[15]=_p15;
    micromatrix_steamakers_bitmap[16]=_p16;
    micromatrix_steamakers_bitmap[17]=_p17;
    micromatrix_steamakers_bitmap[18]=_p18;
    micromatrix_steamakers_bitmap[19]=_p19;
    micromatrix_steamakers_bitmap[20]=_p20;
    micromatrix_steamakers_bitmap[21]=_p21;
    micromatrix_steamakers_bitmap[22]=_p22;
    micromatrix_steamakers_bitmap[23]=_p23;
    micromatrix_steamakers_bitmap[24]=_p24;

    //set bitmap
    micromatrix_steamakers.Bitmap(micromatrix_steamakers_bitmap);
}

void monitor() {
	if (button_debounced_17.pressed()) {
		if (((nivel >= 10) && ((tds < 200) && (turb > 3800)))) {
			fnc_micromatrix_esp32ks_bitmap(CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255));
		}

		if ((((nivel < 10) && (nivel >= 8)) && ((tds < 200) && (turb > 3800)))) {
			fnc_micromatrix_esp32ks_bitmap(CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255));
		}

		if ((((nivel < 8) && (nivel >= 6)) && ((tds < 200) && (turb > 3800)))) {
			fnc_micromatrix_esp32ks_bitmap(CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255));
		}

		if ((((nivel < 6) && (nivel >= 4)) && ((tds < 200) && (turb > 3800)))) {
			fnc_micromatrix_esp32ks_bitmap(CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255));
		}

		if (((nivel < 4) && ((tds < 200) && (turb > 3800)))) {
			fnc_micromatrix_esp32ks_bitmap(CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255));
		}

		if (((nivel >= 10) && (((tds > 200) || (turb < 3800))))) {
			fnc_micromatrix_esp32ks_bitmap(CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0));
		}

		if ((((nivel < 10) && (nivel >= 8)) && (((tds > 200) || (turb < 3800))))) {
			fnc_micromatrix_esp32ks_bitmap(CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0));
		}

		if ((((nivel < 8) && (nivel >= 6)) && (((tds > 200) || (turb < 3800))))) {
			fnc_micromatrix_esp32ks_bitmap(CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0));
		}

		if ((((nivel < 6) && (nivel >= 4)) && (((tds > 200) || (turb < 3800))))) {
			fnc_micromatrix_esp32ks_bitmap(CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0));
		}

		if (((nivel < 4) && (((tds > 200) || (turb < 3800))))) {
			fnc_micromatrix_esp32ks_bitmap(CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0),CRGB(153,51,0));
		}

	}
	else {
		alarma2();
	}

}
void ventas() {
	if (((!digitalRead(0)) == false)) {
		controlTemp();
	}
	else {
		if (button_debounced_0.released()) {
			servo_25.write(45);
		}

		if (button_debounced_0.released()) {
			servo_16.write(45);
		}

		if (button_debounced_0.pressed()) {
			servo_25.write(8);
		}

		if (button_debounced_0.pressed()) {
			servo_16.write(8);
		}

	}

}
void alarma2() {
	if (((!digitalRead(17)) || (alarma == 1))) {
		micromatrix_steamakers.FillColor(CRGB(255,0,0));
		TimerFreeTone(33,1000,500);
		delay(1000);
		micromatrix_steamakers.Clear();
		micromatrix_steamakers.TextScroll(String("ALARMA"),CRGB(255,0,0),150);
		delay(5000);
		micromatrix_steamakers.FillColor(CRGB(255,0,0));
		TimerFreeTone(33,1000,500);
		delay(1000);
	}
	else {
		espmqtt_client.publish(String(String("olimpiada/actuadores/alarma")).c_str(),String(0).c_str(),false);
	}

}
void topicsActuadores() {
}
void controlTemp() {
	if (((temp_in > 23) && (temp_ext < temp_in))) {
		servo_16.write(45);
		servo_25.write(45);
	}
	else if ((servo1 == 1)) {
		servo_25.write(45);
	}
	else if ((servo1 == 0)) {
		servo_25.write(8);
	}
	else if ((servo2 == 1)) {
		servo_16.write(45);
	}
	else if ((servo2 == 0)) {
		servo_16.write(6);
	}

}
void topicsSensores() {
	espmqtt_client.publish(String(String("olimpiada/sensores/luz")).c_str(),String(luz).c_str(),false);
	espmqtt_client.publish(String(String("olimpiada/sensores/temp_in")).c_str(),String(temp_in).c_str(),false);
	espmqtt_client.publish(String(String("olimpiada/sensores/temp_ext")).c_str(),String(temp_ext).c_str(),false);
	espmqtt_client.publish(String(String("olimpiada/sensores/hum_in")).c_str(),String(hum_in).c_str(),false);
	espmqtt_client.publish(String(String("olimpiada/sensores/hum_ext")).c_str(),String(hum_ext).c_str(),false);
	espmqtt_client.publish(String(String("olimpiada/sensores/poten")).c_str(),String(poten).c_str(),false);
	espmqtt_client.publish(String(String("olimpiada/sensores/choiva")).c_str(),String(choiva).c_str(),false);
}

void setup()
{
  	servo_25.attach(25);
	servo_16.attach(16);
	pinMode(2, INPUT);

	dht2.begin();
	Wire.begin();
	sensor_th_aht20.begin();
pinMode(39, INPUT);
	pinMode(36, INPUT);
pinMode(33, OUTPUT);
	pinMode(17, INPUT);
	pinMode(0, INPUT);

	espwifi_setup(String("S25Tecnoteis"),String("olimTele26"));
	espmqtt_setup_str(String("domoteis.iesteis.es"),1883,String("maqueta1"),String("teismaqueta1"),String("esp32m"));
	micromatrix_steamakers.SetBrightness(125);
	micromatrix_steamakers.Clear();
	servo_25.write(0);
	servo_16.write(0);
	topicsActuadores();

}


void loop()
{
	yield();

	espwifi_check();

	espmqtt_loop();
  	temp_in = dht2.readTemperature();
  	hum_in = dht2.readTemperature();
  	temp_ext = (fnc_sensor_th_aht20(1) - 4);
  	hum_ext = fnc_sensor_th_aht20(2);
  	luz = map(analogRead(39),0,4095,0,100);
  	poten = fnc_micro_esp32ks_consumption(2);
  	if ((son == 1)) {
  		TimerFreeTone(33,500,2000);
  	}
  	else {
  		TimerFreeTone(33,0,0);
  	}

  	ventas();
  	monitor();
  	topicsSensores();
  	alarma2();
  	delay(1000);

}