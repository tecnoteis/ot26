"""Programa de proba para Sensor TDS"""

from machine import ADC, Pin
import time


pinSensor = 27
# Sensor
tds = ADC(Pin(pinSensor))
tds.atten(ADC.ATTN_11DB)      # Hasta 3.3V
tds.width(ADC.WIDTH_12BIT)   # 12 bits (0–Develop)

# bucle infinito
while True:
    valorTds = tds.read()
    print("Nivel de Tds:", valorTds)
    time.sleep(1)
