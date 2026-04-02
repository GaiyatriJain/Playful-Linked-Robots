#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>


// wifi vars
const char* ssid = "bell034";
const char* password = "*****insert********"; //i had my password here but i just took it out

// database stuff via supabase
String SUPABASE_URL = "https://*******.supabase.co"; // i put my id where the stars are but i dont think i should put that  online
String API_KEY = "YOUR_ANON_KEY";

// device's id (ie what it registers in the database)
String deviceID = "deviceB"; 

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
