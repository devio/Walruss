# Walruss RSA Data Signing - Eine Sachspende

Freundliche Spende an den Hersteller von PC-Wahl 10.
Open Source.
So macht updaten Spaß!

## Hintergrund

Der Hersteller der Software **PC-Wahl 10.0** versuchte vor der Bundestagswahl im September 2017 hektisch und vergeblich, die Update-Funktion seiner Software abzusichern.
Dies geschah mehrfach hintereinander mit teils absurden Mechanismen.

Da sämtliche Versuche meist bereits nach wenigen Stunden vom Chaos Computer Club für unzureichend befunden wurden, stellen wir nun in einer Quellcode-Spende ein Verfahren zur Verfügung, welches ausreichend einfach im Rahmen des Ökosystems **PC-Wahl** zu implementieren ist.

## Updates

Update-Pakete können mit diesem Verfahren digital signiert werden. Hierzu muss ein RSA-Schlüsselpaar generiert werden, wie es die Dokumentation des RSACryptoServiceProvider beschreibt. Auf Seiten der Update-Routine kann der Public Key hart-kodiert werden. Dies kommt einem Certificate-Pinning gleich. Nach dem Herunterladen des Update-Paket wird automatisch geprüft, ob die RSA-PSS Signatur des Pakets mit dem hart-kodierten öffentlichen Schlüssel verifiziert werden kann.

### Disclaimer: Update best practices

Für den Update-Prozess empfehlen wir die Verwendung eines signierten Microsoft-Installers (MSI). Über einen solchen Installer wird das Betriebssystem in die Verifizierung eingebunden und die Hürden für einen Angreifer signifikant erhöht.
Der Hersteller scheint jedoch nicht über Willen, Flexibilität und inzwischen auch nicht mehr über die notwendige Zeit zu verfügen, einen solchen nutzen zu wollen.
Entsprechend passen wir uns mit diesem Vorschlag an die Vorlieben des Herstellers an und ergänzen sie um einen Hauch RSA.

## Übermittlung von Wahlergebnis-Daten

Die Wahlergebnisdaten müssen digital signiert werden, um einen Manipulationsschutz zu gewährleisten. Auf die Verschlüsselung der Daten kann verzichtet werden, da diese keinen Integritätschutz darstellt.
Der Versuch des Herstellers, dies mit GnuPG zu realisieren, resultierte in drei neuen Schwachstellen. Der hier veröffentlichte Beispielcode kann von PC-Wahl genutzt werden, um Daten zuverlässig digital zu signieren.

### Disclaimer: Signature best practices

Für das Signieren der Wahldaten empfehlen wir die Verwendung einer X.509-PKI. Über eine solche wird die zentrale Verifizierung erleichtert und die Provosionierung skalierbar.
Hersteller und Wahlleiter scheinen jedoch nicht über Willen, Flexibilität und inzwischen auch nicht mehr über die notwendige Zeit zu verfügen, eine solche PKI herstellerübergreifend zu implementieren.
Entsprechend passen wir uns mit diesem Vorschlag an die Vorlieben an und ergänzen sie um einen Hauch RSA.

## Beispiel

Das Projekt kann per Visual Studio 2017 übersetzt und sofort auf der Kommandozeile getestet werden:

```
bin\Debug\Walruss.exe -verify ..\example-data\input.txt
[d] RSA Verify - Result: True

bin\Debug\Walruss.exe -sign ..\example-data\regioIt-update.key  ..\example-data\input.txt
[d] RSA Sign - Result: ZVzDjDPy6+e2J8vcVtE9WrRduKH6V5DJZOi6ad52bX/AOS+tC3geoKC8/wyPfXLyLdXKOxd7wimM/1d8W+ejRsDlKA9nl+Qk1T0+gyUegTGG+VL9op9W8g7hssFaBKxoYah3yZwLUwHM4bXTdI+yd/LcVgkCMEot1adunHnCCBskVBOaPvoH7kHHvKG/GU7BkaEd6iA0Vlw9OUWeQ8V84Uzx7QJvWn0NfIx4FfOlf9v/40jej9Jt/mCGQP7N57D1g8FQKfqgrZp/eFWCoFNIER/FPxJBA5LOKIahSsnT1yZLw0eR50K3MeIBR0/xdzY4KcCOxN/OHzE79ZwvjARwLg==
```


## Author(s)

OpenSource Spende an den Hersteller von PC-Wahl 10 (c) 2017 Chaos Computer Club
by Thorsten (THS) Schroeder <ths [at] ccc [dot] de>
