## Toelichting bij sketch
In deze Arduino sketch kun je vinden hoe je met je Arduino verbinding kunt maken met je GitPod server.
Elke seconde wordt (als nietszeggend voorbeeld) de baanstatus een nieuw random getal tussen 0 en 10.
Elke 30 seconden stuurt de Arduino de huidige baanstatus naar de server.

### Dit MOET je aanpassen om de sketch werkend te krijgen:
- de naam van je WiFi-netwerk
- het bijbehorende wachtwoord
- de naam van je GitPod-server. Als je de server start en in de browser je widget opent, zie je deze domeinnaam. Het is iets als 3000-ef8492d9............gitpod.io

### Uitleg functies:
Naast de `setup` en `loop` functies bevat deze sketch functies om de code onder te verdelen
- `stuurUpdate()`: stuur een HTTP-verzoek met daarin een nieuwe cijfer voor de baanstatus.
- `checkHTTPResponse()`: controleert of er nieuwe data via WiFi is aangekomen. Wordt aangeroepen in de `loop` indien er een verbinding met een server is.
- `checkWiFiModule()`: controleert of de wifi-module het doet.
- `verbindMetWiFiNetwerk()`: maakt de verbinding met het wifi-netwerk. Wordt aangeroepen in de `setup`
- `printWiFiStatus()`: wordt aangeroepen als je aangemeld bent bij je wifi-netwerk. Geeft wat extra info.
- `setWiFiLED(r, g, b)`: de Arduino WiFi Rev2 bevat een kleurenled. Die wordt gebruikt om daarmee gemakkelijk te laten zien wat de status van de wifi en netwerkcommunicatie is
