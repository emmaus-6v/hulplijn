/* 
  IR Breakbeam sensor demo!
  overgenomen van Adafruit, aangepast: https://learn.adafruit.com/ir-breakbeam-sensors/arduino

  aanpassingen door CAM
*/

const int ledPin 13
const int sensorPin 4

// variables will change:
int sensorStatus = LOW;    // de MEEST RECENTE waarde van de sensor
int vorigeStatus = LOW;      // de VORIGE waarde van de sensor

void setup() {
  // stel de ledPin in als OUTPUT
  pinMode(ledPin, OUTPUT);      
  
  // stel de sensorPin in als INPUT
  pinMode(sensorPin, INPUT);

  /* lijkt raar, maar hiermee staat de sensor
     standaard hoog, tenzij deze door onderbreking
     op laag schiet. Dit had je ook kunnen doen door
     deze regel weg te halen en de sensorpin via een
     weerstand van 10KΩ met 5V te verbinden */
  digitalWrite(sensorPin, HIGH);

  // Seriële monitor opstarten om berichten door te geven
  Serial.begin(9600);
}

void loop(){
  // lees de waarde van de sensor
  // LOW  -> onderbroken
  // HIGH -> geen onderbreking
  sensorStatus = digitalRead(SENSORPIN);

  // als de straal onderbroken is: LEDje aan
  // anders uit
  if (sensorStatus == LOW) {
    digitalWrite(LEDPIN, HIGH);  
  } 
  else {
    digitalWrite(LEDPIN, LOW); 
  }

  /* met behulp van de VORIGE sensorstatus
     kun je controleren of de straal misschien
     zojuist is onderbroken of dat de onderbreking
     zojuist is opgeheven. M.a.w: of de sensor van 
     LOW naar HIGH is gegaan -> straal komt weer aan bij sensor
     of van HIGH naar LOW -> straal komt nu niet meer aan
     */
  if (sensorStatus == HIGH && vorigeStatus == LOW) {
    Serial.println("De straal is zojuist onderbroken");
  } 
  if (sensorStatus == LOW && vorigeStatus == HIGH) {
    Serial.println("De versperring is zojuist verwijderd");
  }


  /* voordat we de loop zo opnieuw doorlopen, kopieren
     we de waarde sensorStatus in vorigeStatus zodat
     we die keer opnieuw kunnen controleren of
     de status is veranderd */
  vorigeStatus = sensorStatus;
}
