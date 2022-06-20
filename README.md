# BSY-Portfolio
von Diyar und Faruk

## Aufgabe:

Schreiben Sie ein C/C++-Programm das folgende Anforderungen erfüllt:

Es werden drei Threads erzeugt, die in Endlosschleife laufen und folgende Funktionen ausführen:

- Thread1 nutzt den Lichtstärkesensor BH1750 zur Messung der Umgebungshelligkeit.
- Thread2 nutzt den Luftdrucksensor BMP280 zur Messung der Temperatur.
- Thread3 steuert die Siebensegmentanzeige über den Controller MAX7219.

Der Ablauf soll in folgender Art und Weise erfolgen:

Die Umgebungshelligkeit soll alle 10 Sekunden gemessen werden. Der Wert soll in der
Konsole und mit der Siebensegmentanzeige ausgegeben werden. Die Temperatur wird alle
15 Sekunden gemessen und gleichfalls mit der Siebensegmentanzeige und der Konsole
ausgegeben. Zur besseren Nachvollziehbarkeit sollen die Werte in der Konsole mit dem
Zusatz, von welchem Sensor der Messwert stammt, ausgegeben werden. Stellen Sie sicher,
dass die Ausgaben jeweils vollständig hintereinander erfolgen.

Sobald die Messwerte erfasst sind, soll die Ausgabe mit der Siebensegmentanzeige erfolgen.
Um zu kennzeichnen, welche Messwerte gerade ausgewertet werden, wird für den Wert der
Helligkeit ein „H“ und für die Temperatur ein „C“ an die Zahl angehängt.

Nutzen Sie geeignete Methoden zur Synchronisation der einzelnen Threads. Beenden Sie das
Programm zunächst nach einer geeigneten Anzahl von Minuten.

Wenn Variablen in mehreren Threads benötigt werden, sollen sie geteilt und mittels Call-byReferenz an die Threadfunktionen übergeben werden.
