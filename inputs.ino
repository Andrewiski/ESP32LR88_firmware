/*
 * Assumes a momentary Input is connected that when pressed closes input 
 * 
 * 
 */
const boolean inputsDebug = false;
const int debounceCount = 15;
const int longPressCount = 1000;
const int betweenPressCount = 250;
const int relayCount = 8;
const int inputCount = 8;
const int relays[relayCount] = {Rly1, Rly2, Rly3, Rly4, Rly5, Rly6, Rly7, Rly8};
const int inputs[inputCount] = {Inp1, Inp2, Inp3, Inp4, Inp5, Inp6, Inp7, Inp8};
boolean inputState[inputCount] = {0,0,0,0,0,0,0,0};
int inputPressCount[inputCount] = {0,0,0,0,0,0,0,0};
long inputPressTime[inputCount] = {0,0,0,0,0,0,0,0};

void toggleRelay(int relayIndex){
  digitalWrite(relays[relayIndex], !digitalRead(relays[relayIndex]));
}

void openRelay(int relayIndex){
  digitalWrite(relays[relayIndex], LOW);
}

void closeRelay(int relayIndex){
  digitalWrite(relays[relayIndex], HIGH);
}

void singlePressInput(int inputIndex){
  if(inputsDebug) Serial.println("Input " + String(inputIndex) + " Single Press ");
  toggleRelay(inputIndex);
}
void doublePressInput(int inputIndex){
  if(inputsDebug) Serial.println("Input " + String(inputIndex) + " Double Press ");
  openRelay(inputIndex);
}
void tripplePressInput(int inputIndex){
  if(inputsDebug) Serial.println("Input " + String(inputIndex) + " Tripple Press ");
  closeRelay(inputIndex);
}

void longPressInput(int inputIndex){

  if(inputsDebug) Serial.println("Input " + String(inputIndex) + " Long Press ");
   for(int i = 0; i < inputCount; i ++) {
    openRelay(i);
  }
}

void checkInput(int inputIndex){
  int tmpInput = digitalRead(inputs[inputIndex]);
  long tmpTime = millis();
  
  if(inputState[inputIndex] == 0 && tmpInput == 0 && inputPressCount[inputIndex] == 0){
    //This is a first press
    inputState[inputIndex] = 1;
    inputPressTime[inputIndex] = millis();
    inputPressCount[inputIndex] = 0;
    if(inputsDebug)  Serial.println("Input " + String(inputIndex) + " First Press " + String(inputPressTime[inputIndex]));


  }else if(inputState[inputIndex] == 0 && tmpInput == 0 && inputPressCount[inputIndex] >= 1 && (tmpTime - inputPressTime[inputIndex]) <= betweenPressCount){
    if(inputsDebug) Serial.println("Input " + String(inputIndex) + " Press Multiple  " + String(inputPressTime[inputIndex]));
    inputState[inputIndex] = 1;
    inputPressTime[inputIndex] = millis();

  }else if (inputState[inputIndex] == 1 && tmpInput == 1 &&  (tmpTime - inputPressTime[inputIndex]) >= debounceCount){
   
        
    if((tmpTime - inputPressTime[inputIndex]) > longPressCount){
      // This is a Release after a press of duration debounce count 
      if(inputsDebug) Serial.println("Input " + String(inputIndex) + " Released Long Press " + String((tmpTime - inputPressTime[inputIndex])) + " (" + String(inputState[inputIndex]) + ", " + String(tmpTime) +", " + String(inputPressTime[inputIndex]) + ")"  );
      inputState[inputIndex] = 0;
      inputPressTime[inputIndex] = 0;
      inputPressCount[inputIndex] = 0;
      longPressInput(inputIndex);
    }else{
      if(inputsDebug) Serial.println("Input " + String(inputIndex) + " Released Short Press Count " + String(inputPressCount[inputIndex])  + String((tmpTime - inputPressTime[inputIndex])) + " (" + String(inputState[inputIndex]) + ", " + String(tmpTime) +", " + String(inputPressTime[inputIndex]) + ")"  );
      inputState[inputIndex] = 0;
      inputPressTime[inputIndex] = millis();
      inputPressCount[inputIndex]++;  
      
    }
    
    
  }else if(inputPressTime[inputIndex] > 0 && inputState[inputIndex] == 0 && tmpInput == 1 && (tmpTime - inputPressTime[inputIndex]) >= betweenPressCount ){
    if(inputsDebug) Serial.println("Input " + String(inputIndex) + " Released plus Multiple Press Timeout Expired Press Count " + String(inputPressCount[inputIndex]) + " Timeout " + String((tmpTime - inputPressTime[inputIndex])));
    //This was a short press of bounce
    switch(inputPressCount[inputIndex]){
      case 1:
        singlePressInput(inputIndex);  
        break;
      case 2:
        doublePressInput(inputIndex);  
        break;
      default:
        tripplePressInput(inputIndex);
        break;
    }
    
    inputState[inputIndex] = 0;
    inputPressTime[inputIndex] = 0;
    inputPressCount[inputIndex] = 0;
  }else if(inputState[inputIndex] == 1 && tmpInput == 1 && (tmpTime - inputPressTime[inputIndex]) <= debounceCount ){
    if(inputsDebug) Serial.println("Input " + String(inputIndex) + " Bounce " + String((tmpTime - inputPressTime[inputIndex])));
    //This was a short press or bounce as  it was pressed
    inputState[inputIndex] = 0;
    inputPressTime[inputIndex] = 0;
    inputPressCount[inputIndex] = 0;
  }
  
}




void modeInput(){
  for(int i = 0; i < inputCount; i ++) {
    checkInput(i);
  }
}
