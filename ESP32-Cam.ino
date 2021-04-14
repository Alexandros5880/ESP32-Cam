#include "esp_camera.h"
#include <WiFi.h>

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"




/*
// WiFi Connections data
static const char* ssid = "WIND_2.4G_E9E03F";
static const char* password = "DYk9RCbdEZ";
*/
// Fixed IP
/*
static IPAddress ip(192, 168, 1, 50);
static IPAddress gateway(192, 168, 1, 254); // Routers IP
static IPAddress subnet(255, 255, 255, 0);
*/


void startCameraServer();
bool camRuns = false;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include "Arduino.h"

#include "SPIFFS.h"
#include "SD.h"
#include "ESPAsyncWebServer.h"




// HOSTPOT
static const char* ssid_h     = "ESP32-Access-Point";
static const char* password_h = "123456789";
static IPAddress Ip_h(192, 168, 4, 1);
static IPAddress NMask_h(255, 255, 255, 0);



// Set web server HOSTPOT port number to 80
static AsyncWebServer serverh(80);



void startCameraServer();






const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>Controll Center</title>
        <!--<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>-->
        <style>

        </style>
    </head>
    <body>
        <div style="text-align: center;">
            <h1>WiFi:</h1>
            <label for="ssid"><p style="font-size: 21px;">ssid:</p></label>
            <input type="Text" style="font-size: 21px;" id="ssid" name="ssid" />
            <br/>
            <label for="password"><p style="font-size: 21px;">password:</p></label>
            <input type="Text" style="font-size: 21px;" id="password" name="password" />
        </div>
        <br/><br/>
        <div style="text-align: center;">
            <h1>Camera Settings:</h1>
            <label for="username"><p style="font-size: 21px;">username:</p></label>
            <input type="Text" style="font-size: 21px;" id="username" name="username" />
            <br/>
            <label for="password_u"><p style="font-size: 21px;">password:</p></label>
            <input type="Text" style="font-size: 21px;" id="password_u" name="password_u" />
        </div>
        <br/><br/>
        <div style="text-align: center;">
            <h1>Static IP:</h1>
            <label for="ip_1"><p style="font-size: 21px;">ip:</p></label>
            <input type="Text" style="font-size: 21px; width:50px;" id="ip_1" name="ip_1" />
            <input type="Text" style="font-size: 21px; width:50px;" id="ip_2" name="ip_2" />
            <input type="Text" style="font-size: 21px; width:50px;" id="ip_3" name="ip_3" />
            <input type="Text" style="font-size: 21px; width:50px;" id="ip_4" name="ip_4" />
            <br/>
            <label for="gateway_1"><p style="font-size: 21px;">gateway:</p></label>
            <input type="Text" style="font-size: 21px; width:50px;" id="gateway_1" name="gateway_1" />
            <input type="Text" style="font-size: 21px; width:50px;" id="gateway_2" name="gateway_2" />
            <input type="Text" style="font-size: 21px; width:50px;" id="gateway_3" name="gateway_3" />
            <input type="Text" style="font-size: 21px; width:50px;" id="gateway_4" name="gateway_4" />
            <br/>
            <label for="subnet_1"><p style="font-size: 21px;">subnet:</p></label>
            <input type="Text" style="font-size: 21px; width:50px;" id="subnet_1" name="subnet_1" />
            <input type="Text" style="font-size: 21px; width:50px;" id="subnet_2" name="subnet_2" />
            <input type="Text" style="font-size: 21px; width:50px;" id="subnet_3" name="subnet_3" />
            <input type="Text" style="font-size: 21px; width:50px;" id="subnet_4" name="subnet_4" />
            <!--
            <br/>
            <label for="port"><p style="font-size: 21px;">port:</p></label>
            <input type="Text" style="font-size: 21px; width:50px;" id="port" name="port" />
            -->
        </div>
        <br/><br/>
        <br/>
        <div style="text-align: center;">
            <button id="save_button" style="font-size: 21px; border-radius: 15%;" name="save" onclick="SaveRequest()">save</button>
            <button id="stream" style="font-size: 21px; border-radius: 15%;" name="stream" onclick="startStream()">stream</button>
            <button id="stream" style="font-size: 21px; border-radius: 15%;" name="cancel" onclick="Close()">cansel</button>
        </div>
        <br/><br/>
        <br/>
    </body>
    <script>
      function SaveRequest() {
        let ssid = document.getElementById("ssid").value;
        let password = document.getElementById("password").value;
        let username = document.getElementById("username").value;
        let password_u = document.getElementById("password_u").value;
        let ip_1 = document.getElementById("ip_1").value;
        let ip_2 = document.getElementById("ip_2").value;
        let ip_3 = document.getElementById("ip_3").value;
        let ip_4 = document.getElementById("ip_4").value;
        let gateway_1 = document.getElementById("gateway_1").value;
        let gateway_2 = document.getElementById("gateway_2").value;
        let gateway_3 = document.getElementById("gateway_3").value;
        let gateway_4 = document.getElementById("gateway_4").value;
        let subnet_1 = document.getElementById("subnet_1").value;
        let subnet_2 = document.getElementById("subnet_2").value;
        let subnet_3 = document.getElementById("subnet_3").value;
        let subnet_4 = document.getElementById("subnet_4").value;
        //let port = document.getElementById("port").value;
        let url = "/save?ssid="+ssid+"&password="+password+"&username="+username+"&password_u="+password_u+
            "&ip_1="+ip_1+"&ip_2="+ip_2+"&ip_3="+ip_3+"&ip_4="+ip_4+
            "&gateway_1="+gateway_1+"&gateway_2="+gateway_2+"&gateway_3="+gateway_3+"&gateway_4="+gateway_4+
            "&subnet_1="+subnet_1+"&subnet_2="+subnet_2+"&subnet_3="+subnet_3+"&subnet_4="+subnet_4;//+"&port="+port;
        var client = new XMLHttpRequest();
        client.open("GET", url, true);
        client.send(null); 
      }
      function startStream() {
        let url = "/hostpotstream";
        var client = new XMLHttpRequest();
        client.open("GET", url, true);
        client.send(null);
        //setTimeout(() => {window.location = "http://192.168.4.1:81/stream"}, 1000);
        setTimeout(window.open("http://192.168.4.1:81/stream", "_blank"), 2000);
        //window.open("http://192.168.4.1:81/stream", "_blank");
      }
      function Close() {
        let url = "/hostpotstop";
        var client = new XMLHttpRequest();
        client.open("GET", url, true);
        client.send(null);
      }
  </script>
</html>
)rawliteral";







// Start The Server
void start_hostpot() {
  if (WiFi.status() != WL_CONNECTED) {
    // Remove the password parameter, if you want the AP (Access Point) to be open
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid_h, password_h);
    delay(100);
    // Seting Fixed IP
    WiFi.softAPConfig(Ip_h, Ip_h, NMask_h);
    // Fixed IP
    IPAddress IP_h = WiFi.softAPIP();
    Serial.print("HotSpot IP address: ");
    Serial.println(IP_h);
  }
  // Send web page with input fields to client
  serverh.on("/main", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", MAIN_page);
  });
  
  // When Click Save Button
  serverh.on("/save", HTTP_GET,[](AsyncWebServerRequest *request) {
    int paramsNr = request->params();
    String ssi_d = "";
    String pas_s = "";
    String username = "";
    String pas_u = "";
    String ip_1 = "", ip_2 = "", ip_3 = "", ip_4 = "";
    String gateway_1 = "", gateway_2 = "", gateway_3 = "", gateway_4 = "";
    String subnet_1 = "", subnet_2 = "", subnet_3 = "", subnet_4 = "";
    //String port = "";
    for ( int i = 0; i < paramsNr; i++ ) {
      AsyncWebParameter* p = request->getParam(i);
      if ( p->name() == "ssid" ) {
        ssi_d = p->value();
      }
      if ( p->name() == "password" ) {
        pas_s = p->value();
      }
      if ( p->name() == "username" ) {
        username = p->value();
      }
      if ( p->name() == "password_u" ) {
        pas_u = p->value();
      }
      if ( p->name() == "ip_1" ) {
        ip_1 = p->value();
      }
      if ( p->name() == "ip_2" ) {
        ip_2 = p->value();
      }
      if ( p->name() == "ip_3" ) {
        ip_3 = p->value();
      }
      if ( p->name() == "ip_4" ) {
        ip_4 = p->value();
      }
      if ( p->name() == "gateway_1" ) {
        gateway_1 = p->value();
      }
      if ( p->name() == "gateway_2" ) {
        gateway_2 = p->value();
      }
      if ( p->name() == "gateway_3" ) {
        gateway_3 = p->value();
      }
      if ( p->name() == "gateway_4" ) {
        gateway_4 = p->value();
      }
      if ( p->name() == "subnet_1" ) {
        subnet_1 = p->value();
      }
      if ( p->name() == "subnet_2" ) {
        subnet_2 = p->value();
      }
      if ( p->name() == "subnet_3" ) {
        subnet_3 = p->value();
      }
      if ( p->name() == "subnet_4" ) {
        subnet_4 = p->value();
      }
      /*
      if ( p->name() == "port" ) {
        port = p->value();
      }
      */
    }
    Serial.println("Saving And Start The Server.");
    serverh.end();
    delay(1000);
    deleteFile(SD_MMC, "/ssid.txt");
    deleteFile(SD_MMC, "/password.txt");
    deleteFile(SD_MMC, "/username.txt");
    deleteFile(SD_MMC, "/password_c.txt");
    deleteFile(SD_MMC, "/ip.txt");
    deleteFile(SD_MMC, "/gateway.txt");
    deleteFile(SD_MMC, "/subnet.txt");
    //deleteFile(SD_MMC, "/port.txt");
    writeFile(SD_MMC, "/ssid.txt", ssi_d.c_str());
    writeFile(SD_MMC, "/password.txt", pas_s.c_str());
    writeFile(SD_MMC, "/username.txt", username.c_str());
    writeFile(SD_MMC, "/password_u.txt", pas_u.c_str());
    writeFile(SD_MMC, "/ip.txt", String(ip_1+"&"+ip_2+"&"+ip_3+"&"+ip_4).c_str());
    writeFile(SD_MMC, "/gateway.txt", String(gateway_1+"&"+gateway_2+"&"+gateway_3+"&"+gateway_4).c_str());
    writeFile(SD_MMC, "/subnet.txt", String(subnet_1+"&"+subnet_2+"&"+subnet_3+"&"+subnet_4).c_str());
    //writeFile(SD_MMC, "/port.txt", port.c_str());
    deleteFile(SD_MMC, "/run.txt");
    writeFile(SD_MMC, "/run.txt", "true");
    // Restart ESP
    ESP.restart();
  });
  
  // When Click Hostpot
  serverh.on("/hostpotstream", HTTP_GET, [](AsyncWebServerRequest *request) {
    if(!camRuns) {
      startCameraServer();
      camRuns = true;
    }
  });
  
  // When Click Hostpot Stop
  serverh.on("/hostpotstop", HTTP_GET, [](AsyncWebServerRequest *request) {
    deleteFile(SD_MMC, "/run.txt");
    writeFile(SD_MMC, "/run.txt", "true");
    ESP.restart();
  });
  
  // On Error
  serverh.onNotFound([] (AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
  });
  
  // Start
  serverh.begin();
}






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
















// Setup Cameras Pins
void setupCamera() {
  // Setup Cameras Pins
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
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  #if defined(CAMERA_MODEL_ESP_EYE)
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
  #endif
  // Inti Camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  // Setup Cameras Settings
  sensor_t * s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);       //flip it back
    s->set_brightness(s, 1);  //up the blightness just a bit
    s->set_saturation(s, -2); //lower the saturation
  }
  //drop down frame size for higher initial frame rate
  //s->set_framesize(s, FRAMESIZE_QVGA);
  s->set_framesize(s, (framesize_t)7);
  s->set_vflip(s, 1);
  #if defined(CAMERA_MODEL_M5STACK_WIDE)
    s->set_vflip(s, 1);
    s->set_hmirror(s, 1);
  #endif
}




// Split String Function
String* split(String &str) {
  String * my_data = new String[4];
  int pointer = 0;
  for (int i = 0; i < str.length(); i++) {
    if (str[i] != '&') {
      my_data[pointer] += str[i];
    } else {
      pointer++;
    }
  }
  return my_data;
}




void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  // Setup SD
  setup_SD();
  // Setup Camera
  setupCamera();
  /*
  deleteFile(SD_MMC, "/ssid.txt");
  deleteFile(SD_MMC, "/password.txt");
  deleteFile(SD_MMC, "/username.txt");
  deleteFile(SD_MMC, "/password_c.txt");
  deleteFile(SD_MMC, "/run.txt");
  deleteFile(SD_MMC, "/ip.txt");
  deleteFile(SD_MMC, "/gateway.txt");
  deleteFile(SD_MMC, "/subnet.txt");
  deleteFile(SD_MMC, "/port.txt");
  */
  boolean runserver = (readFile(SD_MMC, "/run.txt") == "true")? true : false;
  if (!runserver) {
    // Start HotSpot
    start_hostpot();
  } else {
    // Setup WIFI
    String ssid = readFile(SD_MMC, "/ssid.txt");
    String password = readFile(SD_MMC, "/password.txt");
    // Get Static Ip GateWay and Subnet From Files
    String b_ip = readFile(SD_MMC, "/ip.txt");
    String b_gateway = readFile(SD_MMC, "/gateway.txt");
    String b_subnet = readFile(SD_MMC, "/subnet.txt");
    String * ipp = split(b_ip);
    String * gatewayy = split(b_gateway);
    String * subnett = split(b_subnet);
    IPAddress ip(ipp[0].toInt(), ipp[1].toInt(), ipp[2].toInt(), ipp[3].toInt());
    IPAddress gateway(gatewayy[0].toInt(), gatewayy[1].toInt(), gatewayy[2].toInt(), gatewayy[3].toInt());
    IPAddress subnet(subnett[0].toInt(), subnett[1].toInt(), subnett[2].toInt(), subnett[3].toInt());
    WiFi.config(ip, gateway, subnet);
    Serial.println("SSID: " + ssid + "Pass: " + password + "IP: " + b_ip + "Gateway: " + b_gateway + "Subnet: " + b_subnet);
    WiFi.begin(ssid.c_str(), password.c_str());
    int counter = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      counter++;
      if (counter == 14) {
        runserver = false;
        break;
      }
    }
    if (!runserver) {
      // Start HotSpot
      start_hostpot();
    } else {
      Serial.println("");
      Serial.println("WiFi connected global");
      if(!camRuns) {
        startCameraServer();
        camRuns = true;
      }
      Serial.print("Camera Ready! Use 'http://");
      Serial.print(WiFi.localIP());
      Serial.println("' to connect");
    }
  }
}








void loop() {
  delay(10000);
}
