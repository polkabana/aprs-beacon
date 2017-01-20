#include <Arduino.h>
#include <ArduinoQAPRS.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define TX_PIN  2
#define TX_DISABLE_PIN  3
#define GPS_TX_PIN  7
#define GPS_RX_PIN  6
#define GPS_TX_PIN  7
#define GPS_BAUD    9600
#define ONEWIRE_BUS 4    // DS18B20 GPIO
#define LED_PIN     13

char packet_buffer[128];
char from_addr[] = "xx0xxx";  // your callsign
char dest_addr[] = "APDR12";
char relays[] = "WIDE1 1";
char char_table = '/';
char aprs_icon = '>';

TinyGPSPlus gps;
SoftwareSerial ss(GPS_RX_PIN, GPS_TX_PIN);
OneWire ds(ONEWIRE_BUS);
DallasTemperature sensors(&ds);

void setup() {
  Serial.begin(115200);
  ss.begin(GPS_BAUD);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  QAPRS.init(TX_DISABLE_PIN, TX_PIN);
}

void loop() {
  sensors.requestTemperatures();
  delay(1000);

  while (ss.available() > 0)
    gps.encode(ss.read());

  if (gps.location.isUpdated() || gps.time.isUpdated() || gps.date.isUpdated()) {
    double lat = gps.location.lat();
    double lon = gps.location.lng();
    double lat_min = (lat - (int)lat) * 60;
    double lon_min = (lon - (int)lon) * 60;
    double lat_sec = (lat_min - (int)lat_min) * 60;
    double lon_sec = (lon_min - (int)lon_min) * 60;
    int alt = gps.altitude.feet();
    int course = gps.course.deg();
    int speed = gps.speed.knots();
    int speed_km = gps.speed.kmph();
    int sats = gps.satellites.value();
    int temp = round(sensors.getTempCByIndex(0));

    if (lat && lon && sats) {
      sprintf(packet_buffer, "=%02d%02d.%02d%c%c%03d%02d.%02d%c%c%03d/%03d/A=%06d Speed=%dkm/h, Temp=%d'C, Sats=%d", (int)lat, (int)lat_min, (int)lat_sec, (lat < 0?'S':'N'), char_table, (int)lon, (int)lon_min, (int)lon_sec, (lon < 0?'W':'E'), aprs_icon, course, speed, alt, speed_km, temp, sats);
      Serial.println(packet_buffer);
      digitalWrite(LED_PIN, HIGH);
      QAPRS.send(from_addr, '9', dest_addr, '0', relays, packet_buffer);
      digitalWrite(LED_PIN, LOW);
    }
  }
  
  delay(60000);	// wait 60 secs
}

