#ifndef UNIT_TEST
#include <Arduino.h>
#include <SPIFFS.h>

#include "Application/Config/Config.h"
#include "Application/Recode/RecodeApplicationService.h"
#include "TFT_eSPI.h"
#include "Test/TestJudgeOutput.h"
#include "Test/TestRepository.h"
#include "UI/PageFactory.h"
#include "UI/PageManager.h"
#include "UI/PartsFactory.h"
#include "infrastructure/Button/ButtonInput.h"

TFT_eSPI display(320, 240);

TestRepository repository;
TestJudgeOutput judgeOutput;
RecodeApplicationService recodeService(judgeOutput, repository);
Config config(judgeOutput);
ButtonInput button;
PageFactory pageFactory(&display, &recodeService, &config, &button);
PartsFactory partsFactory(&display, &recodeService, &config, &button);
PageManager manager(pageFactory, partsFactory, &display);

#include <AudioFileSourceSPIFFS.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2SNoDAC.h>

AudioFileSourceSPIFFS *hoge, *fuga;
AudioGeneratorMP3* mp3;
AudioOutputI2SNoDAC* i2s;

void setup() {
  Serial.begin(115200);

  Serial.println("start");

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield();  // Stay here twiddling thumbs waiting
  }
  Serial.println("\r\nSPIFFS available!");

  // ESP32 will crash if any of the fonts are missing
  if (SPIFFS.exists("/YuGothic20.vlw") == false) Serial.println("no font");
  if (SPIFFS.exists("/YuGothic12.vlw") == false) Serial.println("no font");
  if (SPIFFS.exists("/YuGothic80.vlw") == false) Serial.println("no font");

  display.begin();
  display.setRotation(0);

  PageList pageArray[] = {PageList::Menu, PageList::ConfigLimit,
                          PageList::ConfigRecode};

  button.init();

  try {
    manager.init();

    manager.update();
    manager.draw();

    Serial.println("end first draw");

    for (byte i = 0; i < 3; i++) {
      manager.update();
      manager.changePage(pageArray[i]);
      manager.draw();
      Serial.println("draw");
      vTaskDelay(0);
    }
  } catch (const char* e) {
    Serial.println("exception");
    Serial.println(e);
  }

  Serial.println("end");

  hoge = new AudioFileSourceSPIFFS("/hoge.mp3");
  i2s = new AudioOutputI2SNoDAC();
  mp3 = new AudioGeneratorMP3();

  unsigned long timer = millis();
  mp3->begin(hoge, i2s);
  while (true) {
    if (mp3->isRunning()) {
      if (!mp3->loop()) mp3->stop();
    } else {
      Serial.printf("MP3 done\n");
      Serial.println(millis() - timer);
      delay(1000);
    }
    vTaskDelay(1);
  }
}
void loop() {}

#endif