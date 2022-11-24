#include "mbed.h"
#include "LCD.h"
#include "platform/mbed_thread.h"
#include <cstdio>

#define PAUSE 20

lcd myLcd;

int main()
{
    PortIn zeilen(PortB,0b00001111); // Spalten einlesen
    zeilen.mode(PullUp);
    PortInOut spalten(PortB,0b11110000); // Zeilen abfragen
    spalten.output();
    spalten.mode(OpenDrain);

    myLcd.clear();
    
    char zeichen;
    int eingabe;

    while(true) {
        // 0b11100000 Spalte 1
        // 0b11010000 Spalte 2
        // 0b10110000 Spalte 3
        // 0b01110000 Saplte 4

        // 0b00001110 Zeile 1
        // ob00001101 Zeile 2
        // 0b00001011 Zeile 3
        // ob00000111 Zeile 4

        spalten = 0b11100000; // erste Saplte wählen
        thread_sleep_for(PAUSE);
        eingabe = zeilen;
        if (eingabe == 0b1110) zeichen='1';
        if (eingabe == 0b1101) zeichen='4';
        if (eingabe == 0b1011) zeichen='7';
        if (eingabe == 0b0111) zeichen='*';

        spalten = 0b11010000; // erste Zeile wählen
        thread_sleep_for(PAUSE);
        eingabe = zeilen;
        if (eingabe == 0b1110) zeichen='2'; 
        if (eingabe == 0b1101) zeichen='5';
        if (eingabe == 0b1011) zeichen='8';
        if (eingabe == 0b0111) zeichen='0';

        spalten = 0b10110000; // erste Zeile wählen
        thread_sleep_for(PAUSE);
        eingabe = zeilen;
        if (eingabe == 0b1110) zeichen='3'; 
        if (eingabe == 0b1101) zeichen='6';
        if (eingabe == 0b1011) zeichen='9';
        if (eingabe == 0b0111) zeichen='#';

        spalten = 0b01110000; // erste Zeile wählen
        thread_sleep_for(PAUSE);
        eingabe = zeilen;
        if (eingabe == 0b1110) zeichen='A'; 
        if (eingabe == 0b1101) zeichen='B';
        if (eingabe == 0b1011) zeichen='C';
        if (eingabe == 0b0111) zeichen='D';


        if (zeichen != 'X') { // Wenn Zeichen gesetzt
            myLcd.cursorpos(0);
            myLcd.printf("%c", zeichen);
        } else {
            myLcd.clear();
        }
    }
}
