#include <IRremote.h>
#include <EEPROM.h>

#define IR_RECEIVE_PIN 2
#define IR_LEARN_SW 1
#define IR_LEARN_LED 4

#define OUT_PIN 3

byte outState = LOW;

int addr = 0;
bool learn = false;
int ir_code = -1;

void setup() {
  //Serial.begin(115200);

  pinMode(IR_LEARN_SW,  INPUT_PULLUP);
  pinMode(IR_LEARN_LED, OUTPUT);  
  pinMode(OUT_PIN,      OUTPUT); 
  digitalWrite(IR_LEARN_LED, LOW);
  ledBlink(1, 100, LOW);
  
  if(digitalRead(IR_LEARN_SW)==LOW){
    learn = true;  
    digitalWrite(IR_LEARN_LED, HIGH);
    //Serial.println("attivo learn mode!");
    IrReceiver.begin(IR_RECEIVE_PIN);
  }else{
    learn = false;
    ir_code = EEPROM.read(addr);
    //Serial.print("leggo ir_code: ");
    //Serial.println(ir_code);
    IrReceiver.begin(IR_RECEIVE_PIN);
  }
  

}

void loop() {
  if (IrReceiver.decode()) {
    int command = IrReceiver.decodedIRData.command;
    //Serial.println(command);
    
    if(learn){
      EEPROM.write(addr, command);
      ledBlink(5, 100, HIGH);
      //Serial.print("scrivo ir_code: ");
      //Serial.println(command);
    }else if(command == ir_code){
      outState = (outState == LOW) ? HIGH : LOW;
      digitalWrite(OUT_PIN, outState);
      ledBlink(1, 100, LOW);
      //Serial.print("setto pin uscita: ");
      //Serial.println(outState);
      delay(1000);
    }            
    IrReceiver.resume();        
  }
}

void ledBlink(int i, int d, int final_state){
  for(int k=0; k<i; ++k){
        delay(d);
        digitalWrite(IR_LEARN_LED, LOW==final_state);
        delay(d);
        digitalWrite(IR_LEARN_LED, HIGH==final_state);
      }
}
