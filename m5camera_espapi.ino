#include "esp_camera.h"

//#define M5CAM_MODEL_A
#define M5CAM_MODEL_B

#if defined(M5CAM_MODEL_A)
  #define PWDN_GPIO_NUM     -1
  #define RESET_GPIO_NUM    15
  #define XCLK_GPIO_NUM     27
  #define SIOD_GPIO_NUM     25
  #define SIOC_GPIO_NUM     23
  
  #define Y9_GPIO_NUM       19
  #define Y8_GPIO_NUM       36
  #define Y7_GPIO_NUM       18
  #define Y6_GPIO_NUM       39
  #define Y5_GPIO_NUM        5
  #define Y4_GPIO_NUM       34
  #define Y3_GPIO_NUM       35
  #define Y2_GPIO_NUM       32
  #define VSYNC_GPIO_NUM    22
  #define HREF_GPIO_NUM     26
  #define PCLK_GPIO_NUM     21
#elif defined(M5CAM_MODEL_B)
  #define PWDN_GPIO_NUM     -1
  #define RESET_GPIO_NUM    15
  #define XCLK_GPIO_NUM     27
  #define SIOD_GPIO_NUM     22
  #define SIOC_GPIO_NUM     23
  
  #define Y9_GPIO_NUM       19
  #define Y8_GPIO_NUM       36
  #define Y7_GPIO_NUM       18
  #define Y6_GPIO_NUM       39
  #define Y5_GPIO_NUM        5
  #define Y4_GPIO_NUM       34
  #define Y3_GPIO_NUM       35
  #define Y2_GPIO_NUM       32
  #define VSYNC_GPIO_NUM    25
  #define HREF_GPIO_NUM     26
  #define PCLK_GPIO_NUM     21
#else
  #error "Not supported"
#endif


void startCameraServer();

void getJPEG()
{
  camera_fb_t * fb = NULL;
  /*  数回撮影してAE(自動露出)をあわせる */
  esp_camera_fb_get();
  esp_camera_fb_get();
  esp_camera_fb_get();
  esp_camera_fb_get();
  fb = esp_camera_fb_get();  //JPEG取得
  /*
  fb->buf JPEGバッファ
  fb->len JPEGサイズ
  */
  
  if (!fb) 
  {
    Serial.printf("Camera capture failed");
  }
  esp_camera_fb_return(fb); //後始末
  Serial.printf("JPG: %uB ", (uint32_t)(fb->len));
  Serial.println();
  Serial.println();
  
  Serial.write(fb->buf ,fb->len); //バイナリ出力 FF D8 FFがJPEGシグネチャなのでそこから先を取り出すとJPEGが取り出せる。
}



void setup() 
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  /*  カメラ初期化設定*/
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

  config.frame_size = FRAMESIZE_UXGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;

  //カメラ初期化
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) 
  {
    Serial.printf("Camera init failed with error 0x%x", err);
  }
  else
  {
      getJPEG();
  }
  /*
  あとでカメラ設定変更するときは
  sensor_t * s = esp_camera_sensor_get(); //カメラプロパティのポインタ取得
  s->set_framesize(s, FRAMESIZE_UXGA);  //解像度設定
  とかやればおｋ
  */
}

/*  特に何もしない */
void loop() 
{
  delay(10000);
}
