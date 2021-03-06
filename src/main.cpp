/*
  This is a simple MJPEG streaming webserver implemented for AI-Thinker ESP32-CAM and
  ESP32-EYE modules.
  This is tested to work with VLC and Blynk video widget.
  Inspired by and based on this Instructable: $9 RTSP Video Streamer Using the ESP32-CAM Board
  (https://www.instructables.com/id/9-RTSP-Video-Streamer-Using-the-ESP32-CAM-Board/)
  Board: AI-Thinker ESP32-CAM
*/

#include "OV2640.h"
// #include <WiFi.h>
// #include <WebServer.h>
// #include <WiFiClient.h>

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT
// #define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE
#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"

/*
Next one is an include with wifi credentials.
This is what you need to do:
1. Create a file called "home_wifi_multi.h" in the same folder   OR   under a separate subfolder of the "libraries" folder of Arduino IDE. (You are creating a "fake" library really - I called it "MySettings").
2. Place the following text in the file:
#define SSID1 "replace with your wifi ssid"
#define PWD1 "replace your wifi password"
3. Save.
Should work then
*/
// #include "home_wifi_multi.h"

OV2640 cam;

void handle_jpg_stream(void)
{

  int s;

  while (true)
  {
    if (!Serial1.available())
    {
      Serial.write("Serial1 not available");
      break;
    }

    cam.run();
    s = cam.getSize();

    Serial1.write(0x15);
    Serial1.write(0x04);
    Serial1.write(s);
    Serial1.write((char *)cam.getfb());
    Serial.write("Serial1 write data len=" + s + 3);
  }
}

void setup()
{

  Serial.begin(115200);

  Serial1.begin(115200); // data serial
  // while (!Serial);            //wait for serial connection.

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Frame parameters
  //  config.frame_size = FRAMESIZE_UXGA;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 12;
  config.fb_count = 2;

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  cam.init(config);

  // IPAddress ip;

  // WiFi.mode(WIFI_STA);
  // WiFi.begin(SSID1, PWD1);
  while (!Serial1.available())
  {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println(F("Serial1(data chanl) connected"));
  Serial.print("baudRate???");
  Serial.println(Serial1.baudRate());
}

void loop()
{
  handle_jpg_stream();
}