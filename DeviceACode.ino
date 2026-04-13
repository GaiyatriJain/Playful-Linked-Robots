#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <driver/i2s.h>
#include <math.h>

// wifi vars
const char* ssid = "bell034";
const char* password = "*****insert********"; //i had my password here but i just took it out

// database stuff via supabase
String SUPABASE_URL = "https://*******.supabase.co"; // i put my id where the stars are but i dont think i should put that  online
String API_KEY = "YOUR_ANON_KEY";

// device's id (ie what it registers in the database)
String deviceID = "deviceA"; 

//PCF8575 
#define PCF_ADDR 0x20  // default address?

uint16_t lastState = 0xFFFF; 

// 6 button pins on mcp (available are 1-15)
int buttons[6] = {0, 1, 2, 3, 4, 5};

// messages 
String messages[6] = {
  "😀",
  "💛",
  "👍",
  "😭",
  "hi",
  "miss you!"
};

//state
String lastMsg = "";

#define I2S_BCLK 2
#define I2S_LRC  1
#define I2S_DOUT 3

void setupI2S() {
  i2s_config_t config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 22050,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 4,
    .dma_buf_len = 256,
    .use_apll = false
  };

  i2s_pin_config_t pins = {
    .bck_io_num = I2S_BCLK,
    .ws_io_num = I2S_LRC,
    .data_out_num = I2S_DOUT,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pins);
}

void setup() {
  Serial.begin(115200);

  Wire.begin();

  // getting the pcf ready
  Wire.beginTransmission(PCF_ADDR);
  Wire.write(0xFF); 
  Wire.write(0xFF); 
  Wire.endTransmission();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("connecting...");
  }
  
  Serial.println("Connected!");

  setupI2S();


}

void loop() {
  checkButtons();
  checkMessages();
  delay(200);
}

uint16_t readPCF() {
  Wire.requestFrom(PCF_ADDR, 2);

  if (Wire.available() == 2) {
    uint8_t low = Wire.read();
    uint8_t high = Wire.read();
    return (high << 8) | low;
  }

  return 0xFFFF;
}

//wooshy sound
void playWhoosh() {
  for (int i = 0; i < 4000; i++) {
    float t = i / 22050.0;
    float freq = 200 + 800 * (i / 4000.0); // wooshh
    int16_t sample = 10000 * sin(2 * PI * freq * t);

    size_t bytesWritten;
    i2s_write(I2S_NUM_0, &sample, sizeof(sample), &bytesWritten, portMAX_DELAY);
  }
}

//ding sound
void playDing() {
  for (int i = 0; i < 3000; i++) {
    float t = i / 22050.0;
    float envelope = exp(-3 * t); // goes down
    int16_t sample = 12000 * sin(2 * PI * 800 * t) * envelope;

    size_t bytesWritten;
    i2s_write(I2S_NUM_0, &sample, sizeof(sample), &bytesWritten, portMAX_DELAY);
  }
}
//sending messages
void sendMessage(String msg) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = SUPABASE_URL + "/rest/v1/messages";

    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("apikey", API_KEY);
    http.addHeader("Authorization", "Bearer " + API_KEY);
    http.addHeader("Prefer", "return=minimal");

    String body = "{\"device\":\"" + deviceID + "\",\"msg\":\"" + msg + "\"}";

    int code = http.POST(body);

    Serial.println("Sent: " + msg);
    Serial.println(code);

    http.end();
    
    playWhoosh();
  }
}

//getting messges
void checkMessages() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String otherDevice = (deviceID == "deviceA") ? "deviceB" : "deviceA";

    String url = SUPABASE_URL +
      "/rest/v1/messages?device=eq." + otherDevice +
      "&order=id.desc&limit=1";

    http.begin(url);
    http.addHeader("apikey", API_KEY);
    http.addHeader("Authorization", "Bearer " + API_KEY);

    int code = http.GET();

    if (code > 0) {
      String payload = http.getString();
      Serial.println("Received: " + payload);

      if (payload != lastMsg) {
        lastMsg = payload;
        Serial.println("NEW MESSAGE!");
        Serial.println(payload);

      }
    }

    http.end();
    playDing();
  }
}

//checking buttons
void checkButtons() {
  uint16_t currentState = readPCF();
  for (int i = 0; i < 6; i++) {
    int pin = buttonPins[i];
    bool last = (lastState >> pin) & 1;
    bool current = (currentState >> pin) & 1;

    // detect press 
    if (last == 1 && current == 0) {
      sendMessage(messages[i]);
    }
  }
  lastState = currentState;
}

int getPressedButton() {
  uint16_t currentState = readPCF();

  for (int i = 0; i < 6; i++) {
    int pin = buttons[i];
    bool last = (lastState >> pin) & 1;
    bool current = (currentState >> pin) & 1;

    if (last == 1 && current == 0) {
      lastState = currentState;
      return i;
    }
  }

  lastState = currentState;
  return -1;
}

unsigned long pressStartTime[6] = {0};
bool pressActive[6] = {false};

int getLongPressedButton(unsigned long thresholdMs) {
  uint16_t currentState = readPCF();

  for (int i = 0; i < 6; i++) {
    int pin = buttons[i];
    bool current = (currentState >> pin) & 1;

    if (!current && !pressActive[i]) {
      pressActive[i] = true;
      pressStartTime[i] = millis();
    }

    if (pressActive[i] && !current) {
      if (millis() - pressStartTime[i] > thresholdMs) {
        pressActive[i] = false;
        return i;
      }
    }

    if (current) {
      pressActive[i] = false;
    }
  }

  return -1;
}


struct FaceState {
  float eyeOpen;
  float eyeYOffset;
  float mouthCurve;
  float mouthOpen;
  float energy;
};

FaceState faceNow  = {0.9, 0.0, 0.0, 0.15, 0.6};
FaceState faceGoal = faceNow;

unsigned long nextBlinkTime = 0;
bool blinking = false;

float approach(float current, float target, float amt) {
  return current + (target - current) * amt;
}

void setIdleFace() {
  faceGoal.eyeOpen = 0.9;
  faceGoal.eyeYOffset = sin(millis() * 0.001) * 2.0;
  faceGoal.mouthCurve = 0.0;
  faceGoal.mouthOpen = 0.15;
  faceGoal.energy = 0.6;
}

void reactPlayfulFace() {
  faceGoal.eyeOpen = 1.1;
  faceGoal.mouthCurve = 0.5;
  faceGoal.mouthOpen = 0.3;
  faceGoal.energy = 1.0;
}

void updateBlink() {
  unsigned long now = millis();

  if (!blinking && now > nextBlinkTime) {
    blinking = true;
    faceGoal.eyeOpen = 0.1;
    nextBlinkTime = now + random(1200, 3000);
  }

  if (blinking && faceNow.eyeOpen < 0.2) {
    blinking = false;
  }
}

void updateFace() {
  updateBlink();
  float s = 0.08 * faceGoal.energy;

  faceNow.eyeOpen = approach(faceNow.eyeOpen, faceGoal.eyeOpen, s);
  faceNow.eyeYOffset = approach(faceNow.eyeYOffset, faceGoal.eyeYOffset, s);
  faceNow.mouthCurve = approach(faceNow.mouthCurve, faceGoal.mouthCurve, s);
  faceNow.mouthOpen = approach(faceNow.mouthOpen, faceGoal.mouthOpen, s);
}

void reactToIncomingMessage(String msg) {
  if (msg.indexOf("😭") != -1) {
    faceGoal.eyeOpen = 0.6;
    faceGoal.mouthCurve = -0.5;
    faceGoal.energy = 0.4;
  } else {
    faceGoal.eyeOpen = 1.0;
    faceGoal.mouthCurve = 0.4;
    faceGoal.energy = 0.9;
  }
}
void addMicroMovement() {
  faceGoal.eyeYOffset += sin(millis() * 0.004) * 0.3;
  faceGoal.mouthCurve += sin(millis() * 0.002
