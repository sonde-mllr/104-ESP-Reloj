#include "funciones.h"

void wifiNTPSetup(Preferences &prefer,WebServer &server,WiFiUDP &ntpUDP,NTPClient &timeClient){
  // Documentación de preferences https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/preferences.html
  prefer.begin("wifi-creds",false); // Crea y abre el nameespace si no existe || si existe lo abre
  // Con el segundo argumento siendo false, el namespace se abre en modo lectura-escritura (si fuese true sería solo lectura)
  // Para entender los namespace vamos a hacer un simil. Tengo una familia por parte de padre y otra por parte de madre...
  // puedo identificar a cada familia por mis apellidos siendo el primero el apellido de mi padre y el segundo el de mi madre
  // si tengo 2 primos uno hijo de un hermano de mi padre y otro hijo de un hermano de mi madre que se llaman igual...
  // para identificar a mis primos tendré que tener en cuenta su apellido
  // Ahora en c++ sería algo así:
  // module Foo
  //     BAZ = 1
  // end
  //
  // Module Bar
  //     BAZ = 2
  // end
  //
  // puts Foo::BAZ => 1
  // puts Bar::BAZ => 2
  // Explicación sacada de https://stackoverflow.com/questions/991036/what-is-a-namespace
  
  // Intenta conexion con credenciales guardadas
  String savedSSID = prefer.getString("ssid","");
  String savedPass = prefer.getString("password","");
  
  // Si el ssid es diferente a nada se intenta conectar a ese ssid
  if( savedSSID != ""){
    // Documentacion relacionada con API WIFI de esp32 aqui: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
    // c_str() devuelve un const char* del string al que se le aplique
    WiFi.begin(savedSSID.c_str(),savedPass.c_str()); // Inicia el modo wifiSTA (station) para conectrse a una red
    Serial.print("Conectando a red: ");
    Serial.println(savedSSID);
    
    // Prepara variables para la animación
    unsigned long startTime = millis();
    frame = 0;
    // Espera 10 segundos mientras se conecta y dibuja la animación
    while(WiFi.status() != WL_CONNECTED && millis() - startTime < 10000){
      playAnimation(wifiAnim,frame,totalFramesWifi);
      frame = (frame+1) % totalFramesWifi;
      Serial.print(".");
      delay(100); // Pausa para estabilidad wifi
    }
  }

  // Si no está conectado luego de intentar la conexión con datos que ya tenía
  // inicia el modo punto de acceso donde creará un servidor web donde añadir las credenciales WiFi
  if(WiFi.status() != WL_CONNECTED){
      Serial.println("\n fallo de conexión iniciando modo AP");
      WiFi.softAP("RelojConfig","culodemono");
      Serial.print("AP creado. IP: ");
      Serial.println(WiFi.softAPIP());

      // Servidor web
      server.on("/", HTTP_GET, [&]() {
        server.send(200, "text/html", 
          "<form action='/save'>"
          "SSID: <input name='ssid'><br>"
          "Password: <input name='pass' type='password'><br>"
          "<input type='submit'></form>");
    });

      server.on("/save",HTTP_GET,[&](){
        String newSSID = server.arg("ssid");
        String newPass = server.arg("pass");
        Serial.println(newPass);
        prefer.putString("ssid",newSSID);
        prefer.putString("password",newPass);

        server.send(200,"text/plain","Credenciales guardadas. Reiniciando...");
        
        delay(1000);
        ESP.restart();
      });
  }

  server.begin();
  // Mientras está en modo AP reproduce la animación
  while(true){
    server.handleClient();
    playAnimation(wifiAnim,frame,totalFramesWifi);
    frame = (frame+1) % totalFramesWifi;
    delay(FRAME_DELAY);
  }

  // Si se conectó:
  timeClient.begin();
  // COMPROBAR [[TODO]] -> Esta animación podría quitarla creo que no añade nada
  frame = 0;
  /*while(!timeClient.update()){
    playAnimation(wifiAnim,frame,totalFramesWifi);
    frame = (frame+1) % totalFramesWifi;
    delay(FRAME_DELAY);
    Serial.println("Esperando actualización NTP...");
  }*/

  currentTime = timeClient.getEpochTime();
  mode = 4; // Mode es la variable que indica en qué menú está el reloj siendo 4 el principal (Reloj y fecha) lo fuerza para asegurar que se inicia en esta pantalla
  Serial.print("Hora NTP obtenida: ");
  Serial.println(currentTime);
  // Comprobar eque el timepo interno y el tiempo del servidor son iguales durante 5 segundos
  // [[TODO]] -> Comprobar si este código es necesario
  Serial.println("Comprobando incremento de tiempo durante 5 segundos");
  unsigned long t0 = currentTime;
  for(int i = 0;i<5;i++){
    delay(1000);
    currentTime++;
    Serial.print("Tiempo interno: ");
    Serial.println(currentTime);
  }
  Serial.print("Diferencia con la hora NTP inicial: ");
  Serial.println(currentTime);

  // Desconectar wifi para ahorrar recursos
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("Wifi Desconectado");
  // Inicializa el contador para actualizar cada segundo
  lastUpdate = millis();
}


// Función para reproducir una animación
void playAnimation(const byte anim[][FRAME_SIZE], int frame, int totalFrames) {
  display.clearDisplay();
  // Se asume que drawBitmap puede leer directamente desde PROGMEM
  display.drawBitmap((SCREEN_WIDTH - FRAME_WIDTH) / 2, 0, anim[frame], FRAME_WIDTH, FRAME_HEIGHT, SSD1306_WHITE);
  display.display();
  delay(FRAME_DELAY);
}

// Función para mostrar el reloj (sin retardo bloqueante)
void displayClock() {
  display.clearDisplay();
  
  // Convierte currentTime a una estructura tm
  time_t rawTime = currentTime;
  struct tm timeInfo;
  localtime_r(&rawTime, &timeInfo);
  
  char timeStr[9];  // "HH:MM:SS"
  char dateStr[11]; // "DD/MM/YYYY"
  sprintf(timeStr, "%02d:%02d:%02d", timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
  sprintf(dateStr, "%02d/%02d/%04d", timeInfo.tm_mday, timeInfo.tm_mon + 1, timeInfo.tm_year + 1900);
  
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.println(timeStr);
  display.setTextSize(1);
  display.setCursor(10, 40);
  display.println(dateStr);
  display.display();
}
