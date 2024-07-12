/*
  Wifi управление машинкой Модуль MX1508 
*/

#define F_pin 14      //Пин D5
#define B_pin 12      //Пин D6
#define L_pin 13      //Пин D7
#define R_pin 15      //Пин D8

#define DEBUG_SERIAL  //Закомментируй чтобы выключить отладку (скорость 115200)
// #define Start_Mode    //Закоментируй после первой прошивки


#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif
#include <EEPROM.h>
#include "Data.h"  // данные
#include <GyverTimer.h>
GTimer Timer_connect(MS, 300000);
#include <GyverHub.h>
GyverHub hub("MyDevices", "ESP8266_Car", "");  // префикс, имя, иконка
// GyverHub hub;
gh::Pos pos1, pos2;

Config cfg;
int Speed;
int Wheel;

// ------------ WiFi AP ------------
const char* apssid = "Cars";
const char* appass = "";

void build(GH::Builder& b) {
  hub.setVersion("PareNN/Cars@1.0");
  b.Menu(F("Управление;Настройки;Настройки WIFI"));

  if (b.show(b.menu() == 0)) {
    b.Title_("tit_main", "Машинка WIFI");
    b.beginRow();

    b.Dpad(&pos1).color(gh::Colors::Green);
    if (pos1.changed()) {
      if ((pos1.x == 0) && (pos1.y == 1)) {
        DEBUGLN("Forward:");
        Forward();
      }
      if ((pos1.x == 0) && (pos1.y == -1)) {
        DEBUGLN("Back:");
        Back();
      }
      if ((pos1.x == 0) && (pos1.y == 0)) {
        DEBUGLN("Stop Speed:");
        Stop_Speed();
      }
    }

    b.Dpad(&pos2).color(gh::Colors::Green);
    if (pos2.changed()) {
      if ((pos2.x == 1) && (pos2.y == 0)) {
        DEBUGLN("Right:");
        Right();
      }
      if ((pos2.x == -1) && (pos2.y == 0)) {
        DEBUGLN("Left:");
        Left();
      }
      if ((pos2.x == 0) && (pos2.y == 0)) {
        DEBUGLN("Stop Wheel:");
        Stop_Wheel();
      }
    }

    b.endRow();
  }

  if (b.show(b.menu() == 1)) {
    b.Title_("tit_seting", "Настройки");

    b.Label("Скорость движения").noLabel();
    b.Slider_("Speed", &cfg.Speed).color(gh::Colors::Green).range(0, 255, 1, 2).noLabel();
    // EE_update();
    b.Label("Скорость поворота").noLabel();
    b.Slider(&cfg.Wheel).color(gh::Colors::Green).range(0, 255, 1, 2).noLabel();
    
    EE_update();
  }


  if (b.show(b.menu() == 2)) {
    b.Title_("tit_seting", "Настройки WIFI");

    b.beginRow();
    b.Label("Подключение").noLabel().size(3);
    b.Switch_("WiFimode", &cfg.WiFimode).noLabel().size(1);
    b.endRow();

   
    b.Label("Логин").noLabel();
    b.Input(cfg.ssid).noLabel();
    EE_update();
    b.Label("Пароль").noLabel();
    b.Pass(cfg.pass).noLabel();
    EE_update();
   
  }
}

void setup() {
  pinMode(F_pin, OUTPUT);  // Устанавливаем Pin как выход
  pinMode(B_pin, OUTPUT);  // Устанавливаем Pin как выход
  pinMode(L_pin, OUTPUT);  // Устанавливаем Pin как выход
  pinMode(R_pin, OUTPUT);  // Устанавливаем Pin как выход

#ifdef DEBUG_SERIAL
  Serial.begin(115200);
  DEBUGLN();
#endif

#ifdef Start_Mode
  EEPROM.begin(1000);  // старт епром
  EE_update();
#else
  EE_startup();  // читаем епром
#endif
  /*
  Speed = cfg.Speed;
  Wheel = cfg.Wheel;
*/
  // подключение к WiFi...
  delay(3000);
  startWiFi();


  hub.onBuild(build);
  hub.begin();
}


void loop() {
  hub.tick();  // обязательно тикаем тут
  /*
  if (cfg.Speed = !Speed) {
    EE_update();
    Speed = cfg.Speed;
  }
  if (cfg.Wheel = !Wheel) {
    EE_update();
    Wheel = cfg.Wheel;
  }
  */
}

void Forward() {
  analogWrite(F_pin, cfg.Speed);
  analogWrite(B_pin, 0);
}

void Back() {
  analogWrite(F_pin, 0);
  analogWrite(B_pin, cfg.Speed);
}

void Right() {
  analogWrite(L_pin, 0);
  analogWrite(R_pin, cfg.Wheel);
}

void Left() {
  analogWrite(L_pin, cfg.Wheel);
  analogWrite(R_pin, 0);
}
void Stop_Speed() {
  analogWrite(F_pin, 0);
  analogWrite(B_pin, 0);
}

void Stop_Wheel() {
  analogWrite(L_pin, 0);
  analogWrite(R_pin, 0);
}
