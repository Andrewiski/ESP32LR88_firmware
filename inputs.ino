/*
 * Assumes a momentary Input is connected that when pressed closes input 
 * 
 * 
 */
const boolean inputsDebug = false;
const int debounceCount = 25;
const int longPressCount = 1000;
const int betweenPressCount = 250;
const int relayCount = 8;
const int inputCount = 8;
//This is the bitwise (so you can or and trip two at a time) assignment of what relay the input should trigger
// it is initialy populated via nvram
uint8_t inputRelays[inputCount] = {64,128,32,16,8,4,2,1};

const int relays[relayCount] = {Rly1, Rly2, Rly3, Rly4, Rly5, Rly6, Rly7, Rly8};
const int inputs[inputCount] = {Inp1, Inp2, Inp3, Inp4, Inp5, Inp6, Inp7, Inp8};
boolean inputState[inputCount] = {0,0,0,0,0,0,0,0};
long inputPressTime[inputCount] = {0,0,0,0,0,0,0,0};



boolean loadInputData(){

  inputRelays[0] = nvm.getUChar("NR1", 1);
  inputRelays[1] = nvm.getUChar("NR2", 2);
  inputRelays[2] = nvm.getUChar("NR3", 4);
  inputRelays[3] = nvm.getUChar("NR4", 8);
  inputRelays[4] = nvm.getUChar("NR5", 16);
  inputRelays[5] = nvm.getUChar("NR6", 32);
  inputRelays[6] = nvm.getUChar("NR7", 64);
  inputRelays[7] = nvm.getUChar("NR8", 128);
  return true; 
//   //Init the Config Data
//   for( int i = 0; i < inputCount; i ++){
//      inputInfo[0].name = "Input " + String(i);
//      inputInfo[0].relayToggle = 1 << i;
//    }
//    
//    for( int i = 0; i < relayCount; i ++){
//      relayInfo[0].name = "Relay " + String(i);
//    }
//
//  
//   const char* configFileName = "/config.json";
//   Serial.println("loadConfigData: " + String(configFileName));
//   
//   String fileJson = readFileAsString(configFileName);
//   if (fileJson == ""){
//     Serial.println("loadConfigData: Error loading Config " + String(configFileName));
//     return false;
//   }
//   
//  // Deserialize the JSON document
//  StaticJsonDocument<1024> doc;
//  DeserializationError error = deserializeJson(doc, fileJson);
//
//  // Test if parsing succeeds.
//  if (error) {
//    Serial.print(F("deserializeJson() failed: "));
//    Serial.println(error.c_str());
//     return false;
//  }else{
//    JsonArray inputsArray = doc["inputs"].as<JsonArray>();
//    for (int i = 0; i < inputsArray.size(); i ++) {
//      inputInfo[i].name = inputsArray[i]["name"].as<String>();
//      inputInfo[i].relayToggle = inputsArray[i]["relayToggle"].as<int>();    
//    }
//    JsonArray relaysArray = doc["relays"].as<JsonArray>();
//    for (int i = 0; i < relaysArray.size(); i ++) {
//      relayInfo[i].name = relaysArray[i]["name"].as<String>();   
//    }
//    return true;  
//  }
   
}

void handleInputRelayPageJson(){

  String strInput = Server.arg("input");
  String strRelay = Server.arg("relay");
  String json = "{";
  if(strInput != NULL && strRelay != NULL){
      Serial.println("setInputRelay input=" + strInput + "&strRelay=" + strRelay );
      int input = strInput.toInt();
      int inputRelayValue = strRelay.toInt();
      if(input >=1 && input <=inputCount && inputRelayValue >=0 && inputRelayValue <=255){
        inputRelays[input-1] = lowByte(inputRelayValue);
        char nvmName[4] = "NR0";
        nvmName[2] = String(input).charAt(0);
        nvm.putUChar(nvmName, lowByte(inputRelayValue)); 
      }else{
        json = json + "\"error\":\"Invalid input\",";
      }
  }
  json = json + "\"inputRelays\":[ "; 
  for (int i = 0; i < inputCount; i++){
    json = json + String(inputRelays[i]);
    if(i != inputCount-1){
      json = json + ",";
    }
  }
  json = json + "]}";
  Server.send(200, "text/json", json);
  //Serial.println(json);
  json = String();
}

void toggleInputRelays(int inputIndex){
  int relaybits = inputRelays[inputIndex];
  //Serial.println("toggleInputRelays " + String(inputIndex) + ", " + String(relaybits))
  if( 1 == (1 & relaybits)) {
    digitalWrite(relays[0], !digitalRead(relays[0]));  
  }
  if( 2 == (2 & relaybits)) {
    digitalWrite(relays[1], !digitalRead(relays[1]));  
  }
  if( 4 == (4 & relaybits)) {
    digitalWrite(relays[2], !digitalRead(relays[2]));  
  }
  if( 8 == (8 & relaybits)) {
    digitalWrite(relays[3], !digitalRead(relays[3]));  
  }
  if( 16 == (16 & relaybits)) {
    digitalWrite(relays[4], !digitalRead(relays[4]));  
  }
  if( 32 == (32 & relaybits)) {
    digitalWrite(relays[5], !digitalRead(relays[5]));  
  }
  if( 64 == (64 & relaybits)) {
    digitalWrite(relays[6], !digitalRead(relays[6]));  
  }
  if( 128 == (128 & relaybits)) {
    digitalWrite(relays[7], !digitalRead(relays[7]));  
  }
  
  
}

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
  toggleInputRelays(inputIndex);
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
  
  if(inputState[inputIndex] == 0 && tmpInput == 0 ){
    //This is a first press
    inputState[inputIndex] = 1;
    inputPressTime[inputIndex] = millis();
    if(inputsDebug)  Serial.println("Input " + String(inputIndex) + " First Press " + String(inputPressTime[inputIndex]));
  }else if (inputState[inputIndex] == 1 && tmpInput == 1 &&  (tmpTime - inputPressTime[inputIndex]) >= debounceCount){    
    if((tmpTime - inputPressTime[inputIndex]) > longPressCount){
      // This is a Release after a press of duration debounce count 
      if(inputsDebug) Serial.println("Input " + String(inputIndex) + " Released Long Press " + String((tmpTime - inputPressTime[inputIndex])) + " (" + String(inputState[inputIndex]) + ", " + String(tmpTime) +", " + String(inputPressTime[inputIndex]) + ")"  );
      inputState[inputIndex] = 0;
      inputPressTime[inputIndex] = 0;
      longPressInput(inputIndex);
    }else{
      if(inputsDebug) Serial.println("Input " + String(inputIndex) + " Released Short Press "  + String((tmpTime - inputPressTime[inputIndex])) + " (" + String(inputState[inputIndex]) + ", " + String(tmpTime) +", " + String(inputPressTime[inputIndex]) + ")"  );
      inputState[inputIndex] = 0;
      inputPressTime[inputIndex] = 0;
      singlePressInput(inputIndex);
    }
  }else if(inputState[inputIndex] == 1 && tmpInput == 1 && (tmpTime - inputPressTime[inputIndex]) <= debounceCount ){
    if(inputsDebug) Serial.println("Input " + String(inputIndex) + " Bounce " + String((tmpTime - inputPressTime[inputIndex])));
    //This was a short press or bounce as  it was pressed
    inputState[inputIndex] = 0;
    inputPressTime[inputIndex] = 0;
  }
  
}




void modeInput(){
  for(int i = 0; i < inputCount; i ++) {
    checkInput(i);
  }
}
