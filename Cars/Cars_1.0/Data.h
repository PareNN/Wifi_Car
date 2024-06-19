
// ------------------- МАКРО --------------------
#ifdef DEBUG_SERIAL
#define DEBUGLN(x) Serial.println(x)
#define DEBUG(x) Serial.print(x)
#else
#define DEBUGLN(x)
#define DEBUG(x)
#endif

struct Config {

#ifdef Start_Mode

  int Speed = 100;  // Скорость, значение 0-255
  int Wheel = 100;
  // Нсатройки сети
  bool WiFimode = 0;  // 0 AP, 1 local

  //char ssid[32] = "Parepko_NN";  // логин wifi
  char ssid[32] = "";  // логин wifi
  //char pass[32] = "28101979n";   // пароль wifi
  char pass[32] = "";  // пароль wifi

#else

  int Speed;
  int Wheel;
  bool WiFimode;  // 0 AP, 1 local
  char ssid[32];  // логин wifi
  char pass[32];  // пароль wifi
#endif
};
