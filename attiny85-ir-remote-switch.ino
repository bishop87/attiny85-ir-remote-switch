#include <IRremote.h>
#include <EEPROM.h>

#define IR_RECEIVE_PIN 2
#define IR_LEARN_PIN 3

#define OUT_PIN 9

byte outState = LOW;

int addr = 0;
bool learn = false;
int ir_code = -1;

void setup() {
  Serial.begin(115200);

  pinMode(IR_LEARN_PIN, INPUT_PULLUP);
  pinMode(OUT_PIN,      OUTPUT);  
  
  if(digitalRead(IR_LEARN_PIN)==LOW){
    learn = true;  
    Serial.println("attivo learn mode!");
    IrReceiver.begin(IR_RECEIVE_PIN);
  }else{
    ir_code = EEPROM.read(addr);
    Serial.print("leggo ir_code: ");
    Serial.println(ir_code);
    IrReceiver.begin(IR_RECEIVE_PIN);
  }
  

}

void loop() {
  if (IrReceiver.decode()) {
    int command = IrReceiver.decodedIRData.command;
    Serial.println(command);
    
    if(learn){
      EEPROM.write(addr, command);
      Serial.print("scrivo ir_code: ");
      Serial.println(command);
    }else if(command == ir_code){
      outState = (outState == LOW) ? HIGH : LOW;
      digitalWrite(OUT_PIN, outState);
      Serial.print("setto pin uscita: ");
      Serial.println(outState);
      delay(1000);
    }            
    IrReceiver.resume();        
  }
}
