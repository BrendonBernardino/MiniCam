#include <Arduino.h>
#include "esp_camera.h"
#include <WiFi.h>

#include "esp_timer.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems
#include "esp_http_server.h"

#define STANDBY     0
#define CAPTURE     1
#define RECORDING   2

extern const char* ssid;
extern const char* password;

extern uint8_t camera_state;

esp_err_t camera_init();
esp_err_t camera_capture(String path);
void take_photo();
void startCameraServer();
void stopCameraServer();