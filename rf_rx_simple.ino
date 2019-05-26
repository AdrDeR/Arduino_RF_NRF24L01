// SimpleRx - the slave or the receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10

const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN);

char dataReceived[10]; // this must match dataToSend in the TX
bool newData = false;
int humidity, temperature;

//===========

void setup() {

    Serial.begin(9600);

    Serial.println("SimpleRx Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();
}

//=============

void loop() {
    getData();
    showData();
}

//==============

void getData() {
    if ( radio.available() ) {
        radio.read( &dataReceived, sizeof(dataReceived) );
        newData = true;
    }
}

void showData() {
    int tmp_temperature = 0;
    int tmp_humidity    = 0;
    
    tmp_temperature =(dataReceived[6]<<24|dataReceived[7]<<16|dataReceived[8]<<8|dataReceived[9]);
    tmp_humidity =(dataReceived[2]<<24|dataReceived[3]<<16|dataReceived[4]<<8|dataReceived[5]);
    
    if (newData == true) {
        Serial.print("Temperature: ");                
        Serial.print(tmp_temperature);        
        Serial.println("°C");

        Serial.print("Humidity: ");                
        Serial.print(tmp_humidity);        
        Serial.println("%");
                
        newData = false;
    }
}
