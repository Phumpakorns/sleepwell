#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Poompark - 2.4 GHz";
const char* password = "0817688811";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "24b8e67d-e0ab-4a4a-91bb-7f9ffd21ee84";
const char* mqtt_username = "i4QoCR5HV2oxvfJ4jewHfJnB4Mehb2sp";
const char* mqtt_password = "iuD1RRR8gIe4V34~k0cvnW*YSYkoM4dE";
const int LED = 2;
const int FromSTM = 4;
const int ToSTM = 13;
WiFiClient espClient;
PubSubClient client(espClient);

int lighting = 0;
char msg[50];
long lastMsg = 0;
bool stmFlag = false;

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("Connected");
      client.subscribe("@msg/togstatus");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("Try again in 5 seconds...");
      delay(5000);
    }
  }
}

void onoff(int lighting) {
  if (lighting == 1){
    Serial.println("Turn on the light!");
    digitalWrite(ToSTM,HIGH);
    delay(200);
    digitalWrite(ToSTM,LOW);
    client.publish("@shadow/data/update", "{\"data\" : {\"togstatus\" : \"on\"}}");
  } 
  else if (lighting == 0) {
    Serial.println("Turn off the light!");
    digitalWrite(ToSTM,HIGH);
    delay(200);
    digitalWrite(ToSTM,LOW);
    client.publish("@shadow/data/update", "{\"data\" : {\"togstatus\" : \"off\"}}");
  }  
}

void callback(char* topic,byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  String msg;
  for (int i = 0; i < length; i++) {
    msg = msg + (char)payload[i];
  }
   Serial.println(msg);
  if (String(topic) == "@msg/togstatus") {
    if (msg == "on"){
      lighting = 1;
      onoff(lighting); 
    } 
    else if (msg == "off") {
      lighting = 0;
      onoff(lighting);
    }
    else if (msg == "tog") {
      if(lighting){
        lighting = 0;
      }else{
        lighting = 1;
      }
      onoff(lighting);
    }
  }
}

void IRAM_ATTR stmISR() {
  stmFlag = true;
}

void tog() {
  if (lighting){
    lighting = 0;
  }else{
    lighting = 1;
  }
  onoff(lighting);
}

void bling() {
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW);
}

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(FromSTM, INPUT_PULLUP);
  pinMode(ToSTM, OUTPUT);
  attachInterrupt(FromSTM, stmISR, FALLING);
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  Serial.println("HELLO :)");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
  digitalWrite(ToSTM,LOW);
  bling();
}

void loop() {
  if(stmFlag){
    tog();
    bling();
    stmFlag = false;
  }
  if(!client.connected()){
    Serial.println("mqtt loss");
    reconnect();
  }else{
    Serial.println("mqtt ok");
    delay(2000);
  }
  client.loop();
}
