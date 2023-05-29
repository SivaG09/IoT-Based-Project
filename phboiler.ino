#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2 // DS18B20 on arduino pin2 corresponds to D4 on physical board "D4 pin on the ndoemcu Module"
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
float temp;
float Fahrenheit = 0;

const char *ssid =  "embedded";
const char *pass =  "12345678";


unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10], temp;

WiFiClient client;

long myChannelNumber = 2075575;
const char myWriteAPIKey[] = "QX356RK15A08K9ZA";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);


  // dht.begin();

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print("..");
  }
  Serial.println();
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());
  // dht.begin();
  ThingSpeak.begin(client);
  //*/

  pinMode(D1, INPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(D0, OUTPUT);



  relay();
}

void loop() {


  for (int i = 0; i < 10; i++) //Get 10 sample value from the sensor for smooth the value
  {
    buf[i] = Serial.available(SensorPin);
    delay(10);
  }
  for (int i = 0; i < 9; i++) //sort the analog from small to large
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buf[i] > buf[j])
      {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  avgValue = 0;
  for (int i = 2; i < 8; i++)               //take the average value of 6 center sample
    avgValue += buf[i];
  float phValue = (float)avgValue * 5.0 / 1024 / 6; //convert the analog into millivolt
  phValue = 3.5 * phValue;                  //convert the millivolt into pH value
  Serial.print("    pH:");
  Serial.print(phValue, 2);
  Serial.println(" ");
  waterLevel();
  float w =  analogRead(A0);
  int gas =  digitalRead(D1);

  w = 1023 - w;

  Serial.println("Temperature: " + (String) temp);

  Serial.println("water level: " + (String) w);
  Serial.println("gas level: " + (String) gas);

  if (temp > 45)
  {
    digitalWrite(D6, 1);
    delay(500);


  } else
  {
    digitalWrite(D6, 0);
    delay(500);

  }

  if (w > 450)
  {
    digitalWrite(D7, 1);
    delay(500);


  } else
  {
    digitalWrite(D7, 0);
    delay(500);

  }


  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {

    String postStr = apiKey;

    postStr += "&field1=";
    postStr += String(temp););
    postStr += "&field2=";
    postStr += String(phValue);
    postStr += "&field3=";
    postStr += String(w);
    postStr += "&field4=";
    postStr += String(gas);
    postStr += "&field5=";
    postStr += String(az);
    postStr += "\r\n\r\n";


    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" degrees Celcius, Humidity: ");
    Serial.print(h);
    Serial.println("%. Send to Thingspeak.");
  }
  client.stop();




  delay(15000);
}

//  https://api.thingspeak.com/update?api_key=QX356RK15A08K9ZA&field1=32&field2=68&field3=0&field4=0


void waterLevel()
{


  DS18B20.requestTemperatures();
  temp = DS18B20.getTempCByIndex(0); // Celcius
  Fahrenheit = DS18B20.toFahrenheit(temp); // Fahrenheit
  Serial.println(temp);
  // Serial.println(Fahrenheit);
  // Blynk.virtualWrite(V1, temp); //virtual pin V3
  //Blynk.virtualWrite(V4, Fahrenheit); //virtual pin V4


}

void relay()
{

  delay(500);
  digitalWrite(D5, 1);
  delay(500);
  digitalWrite(D5, 0);
  delay(500);


  digitalWrite(D6, 1);
  delay(500);
  digitalWrite(D6, 0);
  delay(500);

  digitalWrite(D7, 1);
  delay(500);
  digitalWrite(D7, 0);
  delay(500);

  digitalWrite(D8, 1);
  delay(500);
  digitalWrite(D8, 0);
  delay(500);


  digitalWrite(D0, 1);
  delay(500);
  digitalWrite(D0, 0);
  delay(500);

  digitalWrite(D0, 1);
  delay(500);
  digitalWrite(D0, 0);
  delay(500);


  digitalWrite(D5, 0);
  digitalWrite(D6, 0);
  digitalWrite(D7, 0);
  digitalWrite(D8, 0);
  delay(500);



}