#include <Arduino.h>
#include "microsd.h"
#include "camera.h"

#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid_ota = "BANKSY-9!";
const char* password_ota = "bwh4ck34d0";

#define button_capture  3
#define led_record      4
#define TIMEFORRECORD 3000

unsigned long timeLongPress = millis();
unsigned long count_camera = 0;

static void IRAM_ATTR button_ISR_handler();

void setup() {
  Serial.begin(115200);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

  // pinMode(led_record, OUTPUT);
  pinMode(button_capture, INPUT);

  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid_ota, password_ota);

  // while (WiFi.status() != WL_CONNECTED) {
  //     delay(500);
  //     printf(".");
  // }
  // printf("\nWiFi connected\n");
  // // printf("Camera Stream Ready! Go to: http://%s", WiFi.localIP());

  // // Port defaults to 3232
  // ArduinoOTA.setPort(3232);
  
  // // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("Esp_MiniCam");

  // ArduinoOTA
  //   .onStart([]() {
  //     String type;
  //     if (ArduinoOTA.getCommand() == U_FLASH)
  //       type = "sketch";
  //     else // U_SPIFFS
  //       type = "filesystem";
  //     // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
  //     Serial.println("Start updating " + type);
  //   })
  //   .onEnd([]() {
  //     Serial.println("nEnd");
  //   })
  //   .onProgress([](unsigned int progress, unsigned int total) {
  //     Serial.printf("Progress: %u%%r", (progress / (total / 100)));
  //   })
  //   .onError([](ota_error_t error) {
  //     Serial.printf("Error[%u]: ", error);
  //     if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
  //     else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
  //     else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
  //     else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
  //     else if (error == OTA_END_ERROR) Serial.println("End Failed");
  //   });
  // ArduinoOTA.begin();
  // Serial.println("Ready");
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());

  if(ESP_OK != camera_init()) {
    return;
  }

  initMicroSDCard();
  EEPROM.begin(EEPROM_SIZE);
}

void loop() {
  while(gpio_get_level((gpio_num_t)button_capture) == 0) {
    count_camera++;
    delay(1);

    // if(((millis() - timeLongPress) > TIMEFORRECORD) && (camera_state == STANDBY)) { //stream init
    //   printf("stream init\n");
    //   timeLongPress = millis();
    //   camera_state = RECORDING;
    //   startCameraServer();
    //   // gpio_set_level((gpio_num_t)led_record, 1);
    // }
    // if(((millis() - timeLongPress) > TIMEFORRECORD) && (camera_state == RECORDING)) { //stream finish
    //   printf("stream finish\n");
    //   timeLongPress = millis();
    //   stopCameraServer();
    //   camera_state = STANDBY;
    //   // gpio_set_level((gpio_num_t)led_record, 0);
    // }
    // if(((millis() - timeLongPress) < TIMEFORRECORD) && (camera_state == STANDBY)) { //capture photo
    //   printf("camera\n");
    //   timeLongPress = millis();
    //   camera_state = CAPTURE;
    //   take_photo();
    //   camera_state = STANDBY;
    //   // gpio_set_level((gpio_num_t)led_record, 0);
    // }
  }
  if(count_camera < TIMEFORRECORD && count_camera > 0) {
    camera_state = CAPTURE;
    take_photo();
    camera_state = STANDBY;
    count_camera = 0;
  }
  else if(count_camera > TIMEFORRECORD && (camera_state == STANDBY)) {
    printf("stream init\n");
    timeLongPress = millis();
    camera_state = RECORDING;
    startCameraServer();
    count_camera = 0;
    // gpio_set_level((gpio_num_t)led_record, 1);
  }
  else if(count_camera > TIMEFORRECORD && (camera_state == RECORDING)) {
    printf("stream finish\n");
    timeLongPress = millis();
    stopCameraServer();
    camera_state = STANDBY;
    count_camera = 0;
    // gpio_set_level((gpio_num_t)led_record, 0);
  }
  // else {
    // timeLongPress = millis();
  // }

  // if(button_pressed == 1 && button_pressed_ant == 0) {
  //   button_pressed = 1;
  // }
  // if(gpio_get_level((gpio_num_t)button_capture) == 0) {
  //   take_photo();
  // }
  // take_photo();

  // ArduinoOTA.handle();
  // delay(1);//(250)
}


static void IRAM_ATTR button_ISR_handler() {
  if(camera_state == STANDBY) {
    camera_state = CAPTURE;
    take_photo();
    camera_state = STANDBY;
  }
}
