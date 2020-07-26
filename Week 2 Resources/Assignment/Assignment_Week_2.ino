#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>

const char* ssid     = "Aniket";
const char* password = "123456712";

int REDLED = 21;
int GREENLED = 32;
long ptime, currtime;
int acases, deaths, pacases, pdeaths;

void setup()
{

  pinMode(REDLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  digitalWrite(REDLED, LOW);
  digitalWrite(GREENLED, LOW);
  Serial.begin(9600);

  WiFi.enableSTA(true);

  delay(2000);

  connectToWifi();
  currtime = 0;
  ptime = currtime;

  initializePcases();

}

void loop() {
  currtime = millis() / 1000;
  if (WiFi.status() == WL_CONNECTED) {
    if (currtime - ptime >= 86400) {
      ptime = currtime;
      doTheTask();
    }
  }
  else {
    Serial.println("WiFi not connected. Please Turn it on.");
    delay(1000);
    connectToWifi();
  }
}

void connectToWifi()
{

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void initializePcases()
{
  HTTPClient http_active;   // making a variable for interacting to any webpage
  http_active.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key=ZH55P18AI0P1DFJU");  // connecting with this webpage - Active
  int httpCode = http_active.GET();
  if (httpCode > 0) { // condition written is if connected
    int Active_Cases = http_active.getString().toInt();   // storing active
    String confirmedCases = http_active.getString();   // storing active
    Serial.print("Confirmed Active Cases - ");
    Serial.println(confirmedCases); // printing on serial monitor
  }

  HTTPClient http_deaths;   // making a variable for interacting to any webpage
  http_deaths.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key=ZXWQEXSX9BJ18QPH");  // connecting with this webpage - Deaths
  httpCode = http_deaths.GET();
  if (httpCode > 0) { // condition written is if connected
    int Death_Cases = http_deaths.getString().toInt();   // storing deaths
    String deathCases = http_deaths.getString();   // storing deaths
    Serial.print("Confirmed Deaths - ");
    Serial.println(deathCases); // printing on serial monitor
  }
}

void doTheTask() {

  int Active_Cases_new;
  int Death_Cases_new;
  int Active_Cases = 0;
  int Death_Cases = 0;

  HTTPClient http_active;   // making a variable for interacting to any webpage
  http_active.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key=ZH55P18AI0P1DFJU");  // connecting with this webpage - Active
  int httpCode = http_active.GET();
  if (httpCode > 0) { // condition written is if connected
    Active_Cases_new = http_active.getString().toInt();   // storing active
  }

  HTTPClient http_deaths;   // making a variable for interacting to any webpage
  http_deaths.begin("https://api.thingspeak.com/apps/thinghttp/send_request?api_key=ZXWQEXSX9BJ18QPH");  // connecting with this webpage - Deaths
  httpCode = http_deaths.GET();
  if (httpCode > 0) { // condition written is if connected
    Death_Cases_new = http_deaths.getString().toInt();   // storing deaths
  }

  // If any of the data is above 1 percent increase glow REDLED or else glow GREENLED
  if (Death_Cases_new > 1.1 * Death_Cases or Active_Cases_new > 1.1 * Active_Cases) {
    digitalWrite(REDLED, HIGH);
    digitalWrite(GREENLED, LOW);
  }
  else {
    digitalWrite(REDLED, LOW);
    digitalWrite(GREENLED, HIGH);
  }

  Death_Cases = Death_Cases_new;
  Active_Cases = Active_Cases_new;

  // Also remember to turn off other LED
  // Then change previous data to current so to check again from this next day
}
