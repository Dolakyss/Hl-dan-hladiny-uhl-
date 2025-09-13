Popis
Tento projekt slouží k mému problemu s kotlem který nemá žádne upozornění když dochazí uhlí v násypce. 

Senzor
funkce je zajištěna díky kapacitnímu senzoru který snimá uhlí v nýsypce. pokud uhlí klesne nad určitou hladinu, tak senzor se zapne jelikož před ním není žadne uhlí. Typ senzoru jsem zvolil NPN a NC (normal close) jelikož když vidí uhlí tak je otevřený a neposílá žadný signál.  

Funkce relé
když senzor nedetekuje žádné uhlí tak sepne piny které aktivují 12v relátko. Po aktivaci relítka propijí piny na digitalní výstupu na ESP32 proti zemi (v serii dejde rezistor o hodnotě 10K proti GND.)

ESP32


https://www.aliexpress.com/item/1005005231378875.html?spm=a2g0o.order_list.order_list_main.12.5aa71802q08wme
https://www.aliexpress.com/item/1005006422498371.html?spm=a2g0o.order_list.order_list_main.556.24761802kQDqn7
