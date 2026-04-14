#include <MicroMatrix.h>

double Auga_almacenada;
double TDS;
double Turbidez;
MicroMatrix micromatrix_steamakers;
CRGB micromatrix_steamakers_bitmap[25];

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

void setup()
{


	Serial.begin(115200);
	Serial.flush();
	while(Serial.available()>0)Serial.read();

	micromatrix_steamakers.SetBrightness(100);

}


void loop()
{
	yield();

  	if (((Auga_almacenada < 0) && (Auga_almacenada <= 20))) {
  		if (((TDS > 250) && (Turbidez < 3800))) {
  			fnc_micromatrix_esp32ks_bitmap(CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(102,51,0),CRGB(102,51,51),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0));
  		}
  		else {
  			fnc_micromatrix_esp32ks_bitmap(CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255));
  		}

  	}
  	else if (((Auga_almacenada < 20) && (Auga_almacenada <= 40))) {
  		if (((TDS > 250) && (Turbidez < 3800))) {
  			fnc_micromatrix_esp32ks_bitmap(CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(102,51,51),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,51),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0));
  		}
  		else {
  			fnc_micromatrix_esp32ks_bitmap(CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255));
  		}

  	}
  	else if (((Auga_almacenada < 40) && (Auga_almacenada <= 60))) {
  		if (((TDS > 250) && (Turbidez < 3800))) {
  			fnc_micromatrix_esp32ks_bitmap(CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,51),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0));
  		}
  		else {
  			fnc_micromatrix_esp32ks_bitmap(CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255));
  		}

  	}
  	else if (((Auga_almacenada < 60) && (Auga_almacenada <= 80))) {
  		if (((TDS > 250) && (Turbidez < 3800))) {
  			fnc_micromatrix_esp32ks_bitmap(CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,51),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0));
  		}
  		else {
  			fnc_micromatrix_esp32ks_bitmap(CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(0,0,0),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255));
  		}

  	}
  	else if (((Auga_almacenada < 80) && (Auga_almacenada <= 100))) {
  		if (((TDS > 250) && (Turbidez < 3800))) {
  			fnc_micromatrix_esp32ks_bitmap(CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,51),CRGB(102,51,0),CRGB(102,51,0),CRGB(102,51,0));
  		}
  		else {
  			fnc_micromatrix_esp32ks_bitmap(CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255),CRGB(255,255,255));
  		}

  	}

}
