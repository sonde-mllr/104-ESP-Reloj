#ifndef FUNCIONES_H

#define FUNCIONES_H

#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WebServer.h>
#include <Preferences.h>
#include <driver/rtc_io.h>

// Rotary encoder:
// A implemetar {Ajustar brillo pantalla,poner temporizador,poner hora,poner alarma}
// Hacer diagrama de flujo luego de terminar con el examen 
# define SW 2
# define pinA 32
# define pinB 33

// Pantalla
#define SCREEN_I2C_ADDR 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RST_PIN -1

// Pin animaciones (temporal)
#define PIN_SIM 5

// Pin pantalla (temporal)
#define PIN_OLED 15

// Configuración de las animaciones
#define FRAME_WIDTH 64
#define FRAME_HEIGHT 64
#define FRAME_SIZE 512    // Bytes por frame
#define FRAME_DELAY 42

// ========== VARIABLES GLOBALES ==================

extern bool displayOn;
extern const char* ssid;
extern const char* password;

extern const byte PROGMEM carga40[][512];
extern const byte PROGMEM carga60[][512];
extern const byte PROGMEM wifiAnim[][512];
extern const byte PROGMEM alarma[][512];
extern const byte PROGMEM apagado[][512];
// Cálculos para los arrays que entran en el ciclo de modos
extern const int totalFramesCarga40;
extern const int totalFramesCarga60;
extern const int totalFramesAlarma;
extern const int totalFramesApagado;

// wifiAnim se usa únicamente durante la conexión
extern const int totalFramesWifi;

// Modos: 0 = carga40, 1 = carga60, 2 = alarma, 3 = apagado, 4 = reloj
extern int mode;
extern int frame;

// Para detectar el flanco del botón
extern bool lastButtonState;
extern bool oledState;

// Variables para el reloj
extern unsigned long currentTime;  // Hora actual en epoch
extern unsigned long lastUpdate;  // Para controlar el incremento cada segundo

//extern NTPClient timeClient;
extern Adafruit_SSD1306 display;


// ============ FUNCIONES =================
void wifiNTPSetup(Preferences &prefer,WebServer &server,WiFiUDP &ntpUDP,NTPClient &timeClient);
void playAnimation(const byte anim[][FRAME_SIZE], int frame, int totalFrames);
void displayClock();

#endif
