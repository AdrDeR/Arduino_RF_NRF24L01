// SimpleTx - the master or the transmitter

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SimpleDHT.h> // DHT temperature sensor

//#define CE_PIN   9 // Arduino Uno
//#define CSN_PIN 10 // Arduino Uno
#define CE_PIN   6 // Arduino Due
#define CSN_PIN 7 // Arduino Due

// Declarations for DHT11
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = 2;
SimpleDHT11 dht11;
byte temperature = 0;
byte humidity = 0;
  
// Declarations for RF transmitter 
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
char dataToSend[10] = "Message 0";
char txNum = '0';
const byte slaveAddress[5] = {'R','x','A','A','A'};

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000; // send once per second


void setup() {

    Serial.begin(9600);

    Serial.println("SimpleTx Starting");

    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.setRetries(3,5); // delay, count
    radio.openWritingPipe(slaveAddress);
}

//====================

void loop() {
    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis) {
        send();
        prevMillis = millis();
    }

    if (dht11.read(pinDHT11, &temperature, &humidity, NULL)) {
     //Serial.println("Read DHT11 failed.");   
     delay(1000);
    }
    else
    {
     Serial.print((int)temperature); Serial.print(" °C, "); 
     Serial.print((int)humidity); Serial.println(" %");
    }
   
}

//====================

void send() {

    bool rslt;
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2
    //rslt = radio.write( &humidity, sizeof(humidity) );
    Serial.print("Data Sent ");
    Serial.print(dataToSend);
    if (rslt) {
        Serial.println("  Acknowledge received");
        updateMessage();
    }
    else {
        Serial.println("  Tx failed");
    }
}

//================

void updateMessage() {
  
    int tmp_temperature=0;
    int tmp_humidity=0;
    
    // so you can see that new data is being sent
    txNum += 1;
    if (txNum > '9') {
        txNum = '0';
    }
    
    tmp_temperature = (int)temperature;
    tmp_humidity    = (int)humidity;
    

    dataToSend[2] = tmp_humidity>>23; // Shift right by 23 bit w/o the sign bit (=MSB)
    dataToSend[3] = tmp_humidity>>15;
    dataToSend[4] = tmp_humidity>>7;
    dataToSend[5] = tmp_humidity;
    
    dataToSend[6] = tmp_temperature>>23; // Shift right by 23 bit w/o the sign bit (=MSB)
    dataToSend[7] = tmp_temperature>>15;
    dataToSend[8] = tmp_temperature>>7;
    dataToSend[9] = tmp_temperature;
    
    /*
    dataToSend[6] = temp>>24;
    dataToSend[7] = temp>>16;
    dataToSend[8] = temp>>8;
    dataToSend[9] = temp;
    */
 
}
