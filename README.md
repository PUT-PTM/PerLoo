# PerLoo
##Założenia
Projekt zrealizowany na potrzeby przedmiotu Podstawy Techniki Mikroprocesorowej dziennych studiów inżynierskich na kierunku Informatyka uczelni Politechnika Poznańska w roku akademickim 2013/2014.

Głównym założeniem było pokazanie nabytych umiejętności w obrębie techniki mikroprocesorowej, a w szczególności oprogramowanie wybranego przez siebie do projektu mikrokontrolera. 

Kod źródłowy projektu zostaje opublikowany na zasadach licencji [MIT](http://opensource.org/licenses/MIT).
##Cele
Celem projektu było skonstruowanie przezroczystej nakładki na rośliny stwarzającej im z góry określony mikroklimat. Nakładka ta, dzięki zamontowanym czujnikom oraz mechanizmom, utrzymuje w jej wnętrzu stałe, najodpowiedniejsze dla danej rośliny warunki klimatyczne. 

Nadzorowane parametry klimatyczne:

* Temperatura
* Wilgotność
* Oświetlenie

Są to najważniejsze parametry istotne do prawidłowego rozwoju rośliny.

Nakładka została zaprojektowana dla pojedynczej rośliny nieprzekraczającej:

* 100cm wysokości(razem z doniczką), 
* 30cm średnicy przy maksymalnej wysokości  

##Realizacja
Podstawą konstrukcji nakładki jest arkusz przezroczystego tworzywa grubości 0.5 mm zwinięty w stożek. Wewnątrz niego umieszczone są czyjniki a na zwnątrz mikrokontroler oraz pozostałe urządzenia. Po wewnętrznej stronie stożka zamontowana została taśma z diodami LED zapalająca się w momęcie wykrycia na fotorezystorze zbyt niskiego natężenia światła. Do zewnętrznej ściany konstrukcji został zamontowany serwonapęd, który przez otwór w tworzywie reguluje przepływ wody z zasobnika umieszczonego na zewnątrz. Ze wzgędów bezpieczeństwa mikrokontroler oraz przekaźniki zostały zamontowane w szczelnym pudełku poza stożkiem. Na samym szczycie znajduje się grzałka, a nad nią bezgłośny wentylator wtłaczający powietrze do środka stożka. Powietrze przepływając przez grzałkę nagrzewa się i zwiększa temperaturę wokół rośliny. Wentylator wraz z grzałką jest uruchamiany w momęcie spadku temperatury powietrza poniżej wartości granicznej.

- - -
Projekt został w całości napisany w [CooCox IDE](http://www.coocox.org/index.html). 

###Użyte urządzenia:
* Mikrokontroler: [STM32F4DISCOVERY](http://www.st.com/web/catalog/tools/FM116/SC959/SS1532/PF252419)
* Czujnik temperatury oraz wilgotności: [DHT11](http://www.micro4you.com/files/sensor/DHT11.pdf)
* Serwonapęd: [Hextronik HXT900](http://www.servodatabase.com/servo/hextronik/hxt900)
* Przekaźnik: [Songle SRD-05VDC-SL-C](http://www.parallax.com/sites/default/files/downloads/27115-Single-Relay-Board-Datasheet.pdf)
* Wentylator: [AAB Cooling Super Silent Fan 12](http://www.komputronik.pl/product/87199/Sprzet_komputerowy_/Podzespoly/AAB_Cooling_Super_Silent_Fan_12.html)
* Oświetlenie: taśma LED SMD 12V
* Grzałka


## Skład zespołu:
* Bartosz Schiller
* Adrian Moczulski
* Rafał Kitta ([GitHub](https://github.com/rafalkitta))


