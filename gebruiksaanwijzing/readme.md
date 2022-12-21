# Gebruiksaanwijzing

### opladen / vervangen batterijen
Je hebt nodig 2 18650 li-ion batterijen en een geschikte oplader voor dit type. Je kan de oplader aan je pc, AC adapter,... opladen.

### draadloze communicatie
#### verbinding maken
Om draadloos te kunnen communiceren met je line follower maakt je gebruik van een bluetooth module. De bluetooth modulde die ik gebruik in mijn project is een HC-05 module. Plug de module in de voorziene pinnen op je pcb bord. Zorg er voor dat je arduino niet meer geconecteerd is met jouw computer via de usb kabel. Vervolgens kan je op jouw gsm een serial terminal downloaden. Deze zal je gebruiken voor de verschillende commando's. Eens dit allemaal gebeurt is connecteer je jouw gsm/computer met de HC-05 module. Nu kan je commando's doorsturen naar je arduino via de serial terminal.

#### commando's
debug [on/off]  => Opvragen van je ingestelde parameters.
run  => Starten van de line follower.
stop  => Stoppen van de line follower.
set cycle [µs]  => Instellen van de cycle time. Deze moet hoger zijn dan jouw calculation time.
set power [0..255]  => Instellen van hoeveel power je jouw line follower wilt geven. Hoe hoger, hoe sneller de line follower.
set diff [0..1]  => Differentieel bepaalt hoeveel je afremt in de bochten.
set kp [0..]  => Hoeveel ja line follower zichtzelf corrigeert om de line perect in het midden te krijgen. Is deze te hoog zal jouw line follower een schokkende beweging geven van links naar rechts.
set ki [0..]  => Integererende correctie. Hoe langer de error duurt hoe sterker de line follower zal bijregelen.
set kd [0..]  => Dit is de foutverandering. Kijkt naar de fout in de vorige cyclus aan gaat hier de fout gaan bijregelen. Was de fout groot dan wordt er meer gecorigeerd.
calibrate black  => Commando voor het calibreren van de zwart waarden.
calibrate white  => Commando voor het calibreren van de wit waarden.

### kalibratie
Wanneer men de QTR-8A wil kalibreren moet je 2 dingen doen. Plaats je line follower met zijn sensoren op een zwarte lijn van je parcour. Zorg dat alle sensoren over het zwarte deel hangen. Hierna geeft je het commando calibrate black in op gsm. Als je de melding krijgt calibrate black done kan je het zelfde doen voor de kleur wit.
Let op!! Wanneer je op een andere parcour wil rijden doe je deze stappen best opnieuw om een optimaale prestatie te krijgen.

### settings
De robot rijdt stabiel met volgende parameters: 
cyle time = 2100,
power = 120,
diff = 0.20,
kp = 10.00,
ki = 0.10,
kd = 0.35,


