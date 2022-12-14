/* 
 * mbed Library to use a software master i2c interface on any GPIO pins
 * Copyright (c) 2012 Christopher Pepper
 * Released under the MIT License: http://mbed.org/license/mit
 */

#ifndef _SOFTWARE_I2C_H_
#define _SOFTWARE_I2C_H_

#include "mbed.h"

/**
  * @brief SoftwareI2C class
  */

class SoftwareI2C {
public:
    SoftwareI2C(PinName sda, PinName scl);
    ~SoftwareI2C();

    void read(uint8_t device_address, uint8_t* data, uint8_t data_bytes);
    void write(uint8_t device_address, uint8_t* data,  uint8_t data_bytes);
    void write(uint8_t device_address, uint8_t byte);
    void randomRead(uint8_t device_address, uint8_t start_address, uint8_t* data, uint8_t data_bytes);
    void randomWrite(uint8_t device_address, uint8_t start_address, uint8_t* data, uint8_t data_bytes);
    void randomWrite(uint8_t device_address, uint8_t start_address, uint8_t byte);
    
    uint8_t read8(uint8_t device_address, uint8_t start_address);
    uint16_t read16(uint8_t device_address, uint8_t start_address);
    uint32_t read24(uint8_t device_address, uint8_t start_address);
    uint32_t read32(uint8_t device_address, uint8_t start_address);
    
    void setDeviceAddress(uint8_t address){
        _device_address = address;
    }
    
    void setFrequency(uint32_t frequency){
        _frequency_delay = 1000000 / frequency;
    }

    inline void initialise() {
        //masse=0;
        _scl.output();
        //GPIOA->OSPEEDR=0xFF3FFFFF;
        _sda.output();
        
        _sda = 1;
        _scl = 0;
        warte(_frequency_delay);

        for ( int n = 0; n <= 3; ++n ) {
            stop();
        }
    }

private:
    void warte(int zeit)
    {
     for (int i=0;i<zeit*5;i++)
     {
      asm("nop\n\t");   
     }   
    }
    inline void start() {
        _sda.output();
        warte(_frequency_delay);
        _scl = 1;
        _sda = 1;
        warte(_frequency_delay);
        _sda = 0;
        warte(_frequency_delay);
        _scl = 0;
        warte(_frequency_delay);
    }

    inline void stop() {
        _sda.output();
        warte(_frequency_delay);
        _sda = 0;
        warte(_frequency_delay);
        _scl = 1;
        warte(_frequency_delay);
        _sda = 1;
    }

    inline void putByte(uint8_t byte) {
        _sda.output();
        for ( int n = 8; n > 0; --n) {
            warte(_frequency_delay);
            _sda = byte & (1 << (n-1));
            warte(_frequency_delay); //2.12.21
            _scl = 1;
            warte(_frequency_delay);
            _scl = 0;
        }
        _sda = 1;
    }

    inline uint8_t getByte() {
        uint8_t byte = 0;

        _sda.input();          //release the data line
        _sda.mode(OpenDrain);
        
        warte(_frequency_delay);

        for ( int n = 8; n > 0; --n ) {
            _scl=1;            //set clock high
            warte(_frequency_delay);
            byte |= _sda << (n-1); //read the bit
            warte(_frequency_delay);
            _scl=0;            //set clock low
            warte(_frequency_delay);
        }

        _sda.output();         //take data line back

        return byte;
    }

    inline void giveAck() {
        _sda.output();
        warte(_frequency_delay);
        _sda = 0;
        warte(_frequency_delay); //2.12.21
        _scl = 1;
        warte(_frequency_delay);
        _scl = 0;
        _sda = 1;

    }

    inline bool getAck() {
        _sda.output();
        _sda = 1;
        _scl = 1;
        _sda.input();
        _sda.mode(OpenDrain);
        warte(_frequency_delay);
        _scl = 0;

        if(_sda != 0){return false;}

        warte(_frequency_delay);
        return true;
    }

    DigitalInOut _sda;
    DigitalInOut _scl;
    //DigitalOut masse(PA_10);
    //DigitalOut _scl;
    
    uint8_t _device_address;
    uint32_t _frequency_delay;
};

#endif