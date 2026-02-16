# Heisprosjekt Gruppe 57 Jakob og Torkel 2026

## Dokumentasjon
Hver modul i V-diagrammet våres tilsvarer en modul i C (altså en .h og .c fil)
V-diagram: https://lucid.app/lucidchart/af6554d9-918d-4caa-bd1b-52f9e21cc7bc/edit?viewport_loc=-1516%2C1298%2C1150%2C1301%2C0_0&invitationId=inv_f5284a52-c125-4b88-92a1-a448fc3036dc 

For dokumentasjon, skriv
```
doxygen Doxyfile
cd docs/html && python3 -m http.server 6767
```
fra roten i prosjektet og åpne ```http://localhost:6767``` i nettleseren.

### Hvordan bruke simulator
Kjør følgende kommandoer i terminal
``` chmod +x SimElevatorServer``` første gang
```./SimElevatorServer```
Åpne en annen terminal og skriv
```make && ./elevator```


**Bruk:**
- Heisknapp (opp) : qwe
- Heisknapp (ned) : sdf
- Heisknapp (inne) : zxcv
- Obstruksjonsknapp :-
- Stoppknapp : p


# Notater til heisprosjekt




## C programmering
Ligner på C++. 

**Header**: Hver "modul" har en .h og en .c fil. I .h (header) deklarerer man og sier da hva andre moduler som importerer denne kan se. I .c-filen definerer man funksjonen

**typedef enum**: Her defineres en type. navnet på typen er det som kommer etter } (feks. MotorDirection). MotorDirection blir da en type som gjør at man kan bruke de definerte konstantene feks DIRN_DOWN som verdi. Eksempel:


**Header guards**:
```
#ifndef ELEVATOR_CONTROLLER_H  // If not defined
#define ELEVATOR_CONTROLLER_H  // Define it
// ... your declarations ...
#endif
```
eller 
```
#pragma once
```

**static** static int tall = 0;     Dette gjør at tall bare er definert innad i filen.





### Conventions
Definerte funksjoner i moduler bør ha navnet på følgende form: modulnavn_funksjonsnavn. Feks doorController_close() istedenfor bare close().











## git
```
git add .
git commit -m "message"
git push
```

### Conventions
Commite når man gjør en endring. 
Commits bør se ut som følgende:
```
type: short description
[optional longer explanation]
````

type kan være 
- feat: - New feature
- fix: - Bug fix
- docs: - Documentation only
- refactor: - Code restructure (no behavior change)
- test: - Adding/fixing tests
- style: - Formatting, missing semicolons, etc.
- wip: - Work in progress (use sparingly)