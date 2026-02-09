# Notater til heisprosjekt

## Dokumentasjon
Hver modul i V-diagrammet våres tilsvarer en modul i C (altså en .h og .c fil)


### driver








## C programmering
Ligner på C++. 

**Header**: Hver "modul" har en .h og en .c fil. I .h (header) deklarerer man og sier da hva andre moduler som importerer denne kan se. I .c-filen definerer man funksjonen

**typedef enum**: Her defineres en type. navnet på typen er det som kommer etter } (feks. MotorDirection). 

**Header guards**:
''' 
#ifndef ELEVATOR_CONTROLLER_H  // If not defined
#define ELEVATOR_CONTROLLER_H  // Define it
// ... your declarations ...
#endif
'''

**static** static int tall = 0;     Dette gjør at tall bare er definert innad i filen.

### Conventions
Definerte funksjoner i moduler bør ha navnet på følgende form: modulnavn_funksjonsnavn. Feks doorController_close() istedenfor bare close().