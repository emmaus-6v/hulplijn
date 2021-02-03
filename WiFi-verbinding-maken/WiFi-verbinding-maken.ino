#include <SPI.h>
#include <WiFiNINA.h>
#include <utility/wifi_drv.h>

char ssid[] = "naam-van-je-wifi-netwerk";    // naam van het WiFi-netwerk. Dat heet SSID
char pass[] = "wachtw00rd";                  // WiFi-wachtwoord

int status = WL_IDLE_STATUS;

// Vul hieronder de domeinnaam van de gitpod server in.
// Deze vind je ook als je de widget in een browser opent.
// Later richten we het verzoek naar deze domeinnaam + "/api/......"
char server[] = "3000-ef8492d9-bla-bla-bla-bla.gitpod.io";

// We maken een client-object aan dat berichten naar de server kan sturen
// en antwoord terug kan krijgen.
WiFiSSLClient client;

long statusTimerLaatsteActivering = 0;
int  statusTimerWachtTijd = 1000;  // elke seconde 'tellen' we een knikker

long wifiTimerLaatsteActivering = 0;
int  wifiTimerWachtTijd = 30000; // elke 30 seconden sturen we een update naar de server

int baanStatus = 0;


void setup() {
  // De Arduino UNO WiFi Rev2 heeft een kleurenled. Die gebruiken we voor de WiFi-status:
  // Zet LED op rood:
  setWiFiLED(255, 0, 0);
  
  // Zet de seriele poort klaar zodat je berichten naar je
  // computer kunt sturen:
  Serial.begin(9600);
  while (!Serial) {
    ; // we wachten hier net zolang tot de seriele poort verbinding heeft
  }

  // verbind met de WiFi
  checkWiFiModule();
  verbindMetWiFiNetwerk();

  // als je hier bent, werkt de WiFi, zet led op groen:
  setWiFiLED(0, 255, 0);

  // initialiseer hier je timers:
  // zet de start, t=0, van de timers op NU,
  // zodat die 1 en 30 seconden vanaf NU voor het eerst 'afgaan'.
  statusTimerLaatsteActivering = millis();
  wifiTimerLaatsteActivering=  millis();

  // Zet hieronder alle andere dingen die in de setup moeten,
  // zoals pinMode etc etc.







}


void loop() {
  // controleer of de telTimer 'af moet gaan'
  if (millis() > statusTimerLaatsteActivering + statusTimerWachtTijd) {
    // er zijn blijkbaar meer van 1000 milliseconden verstreken
    // sinds de laatste activering van de telTimer
    // Verhoog knikkerTeller met 1.
    // update de tijd van de laatste activering naar NU
    statusTimerLaatsteActivering = millis();
    
    // geef de baanstatus een random waarde tussen 0 en 10:
    baanStatus = random(10);
  }

  // controleer of de wifiTimer 'af moet gaan'
  if (millis() > wifiTimerLaatsteActivering + wifiTimerWachtTijd) {
    // er zijn blijkbaar meer van 30000 milliseconden verstreken
    // sinds de laatste activering van de wifiTimer
    
    // update de tijd van de laatste activering naar NU
    wifiTimerLaatsteActivering = millis();

    // tijdens communicatie zetten we de LED even op blauw
    setWiFiLED(0, 0, 255);
    
    stuurUpdate();
  }


  // Je wilt in ieder geval kunnen lezen wat de
  // server als antwoord terugstuurt.
  if (client.connected()) {
    checkHTTPResponse();

    // na antwoord zetten we LED weer op groen:
    setWiFiLED(0, 255, 0);
  }
  


  // hele korte delay om de Arduino niet helemaal gek te maken
  delay(50);
}



void stuurUpdate() {
  Serial.println("\nStart verbinding met server");

  // hier maken we gebruik van het client-object om
  // de verbinding te maken. Die geeft true of false
  // terug om aan te geven of het is gelukt.
  // Het kan even duren voor de verbinding is gemaakt.
  if (client.connect(server, 443)) {
    Serial.println("Verbonden met de server. HTTP verzoek wordt verstuurd.");
    
    // We sturen nu m.b.v. het client-object de tekst van een HTTP verzoek:
    // 1e regel
    client.print("GET /api/setKnikkerbaanStatus/");
    client.print(baanStatus);
    client.println(" HTTP/1.1");

    // 2e regel
    client.print("Host: ");
    client.println(server);

    // 3e regel
    client.println("Connection: close");

    // 4e regel -- moet leeg zijn
    client.println();

    /* Dat verzoek ziet er dus zo uit als baanstatus de waarde 8 heeft:
     GET /api/setKnikkerbaanStatus/8 HTTP/1.1
     Host: 3000-blablabla.gitpod.io
     Connection: close
     
    */
  }
  else {
    Serial.println("verbinding maken niet gelukt");
  }
  
}



void checkHTTPResponse() {
  // controleer of de client data beschikbaar heeft
  // lees deze uit en print ze, net zolang tot er
  // geen nieuwe data meer beschikbaar is
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // mocht de server na het antwoord toch zelf niet
  // de verbinding hebben verbroken, doe het dan maar zelf
  if (!client.connected()) {
    Serial.println("");
    Serial.println("---------EINDE ANTWOORD VAN DE SERVER----------");
    Serial.println("De verbinding met de server is verbroken");
    client.stop();
  }
}



void checkWiFiModule() {
  // controleer of er een WiFi module is
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // controleer de versie van de firmware
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }
}



void verbindMetWiFiNetwerk() {
  // probeer contact te maken met het opgegeven WiFi-netwerk:
  // dit gaat met een while-loop, zodat je niet verder kunt zolang
  // je niet verbonden bent.
  while (status != WL_CONNECTED) {
    Serial.print("Probeert te verbinden met SSID: ");
    Serial.println(ssid);
    
    // verbind met WiFi.begin
    status = WiFi.begin(ssid, pass);

    // wacht 4 seconden voor de verbinding:
    // mocht het verbinden niet lekker gaan, probeer er dan
    // 10000 milliseconden van te maken (10 seconden)
    delay(4000);
  }
  // als je hier komt, betekent dat dat je verbonden bent.  
  Serial.println("Verbonden met WiFi");

  // print informatie over het netwerk waarmee je verbonden bent
  printWiFiStatus();
}



void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


// stel de kleur van de RGB led in
void setWiFiLED(uint8_t rood, uint8_t groen, uint8_t blauw) {
  WiFiDrv::pinMode(25, OUTPUT);  //GREEN
  WiFiDrv::pinMode(26, OUTPUT);  //RED
  WiFiDrv::pinMode(27, OUTPUT);  //BLUE
  WiFiDrv::analogWrite(27, blauw);
  WiFiDrv::analogWrite(26, groen);
  WiFiDrv::analogWrite(25, rood);
}
