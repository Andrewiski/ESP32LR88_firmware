
enum { NONE=0, INDEX, XML };

void handleMenuHtml(void){
   Server.send(200, "text/html", AUTOCONNECT_LINK(BAR_32)); 
}
 
void handleRelayPageJson(void)
 {            

            String strRly = Server.arg("rly");
            String strAction = Server.arg("a");
            
            if(strRly != NULL && strAction != NULL){
                Serial.println("gpio?rly=" + strRly + "&action=" + strAction );
                int rly = strRly.toInt();
                int action = strAction.toInt();
    //            if (isDigit(strRly)){
    //              rly = strRly.toInt(); 
    //            }
    //            if (isDigit(strAction)){
    //              action = strAction.toInt(); 
    //            }
                switch(rly) {
                  case 1:
                    switch(action) {
                      case 0: digitalWrite(Rly1, 0); break;
                      case 1: digitalWrite(Rly1, 1); break;
                      case 2: digitalWrite(Rly1, !digitalRead(Rly1)); break;
                    }
                    break;
                  case 2:
                    switch(action) {
                      case 0: digitalWrite(Rly2, 0); break;
                      case 1: digitalWrite(Rly2, 1); break;
                      case 2: digitalWrite(Rly2, !digitalRead(Rly2)); break;
                    }
                    break;
                  case 3:
                    switch(action) {
                      case 0: digitalWrite(Rly3, 0); break;
                      case 1: digitalWrite(Rly3, 1); break;
                      case 2: digitalWrite(Rly3, !digitalRead(Rly3)); break;
                    }
                    break;
                  case 4:
                    switch(action) {
                      case 0: digitalWrite(Rly4, 0); break;
                      case 1: digitalWrite(Rly4, 1); break;
                      case 2: digitalWrite(Rly4, !digitalRead(Rly4)); break;
                    }
                    break;
                  case 5:
                    switch(action) {
                      case 0: digitalWrite(Rly5, 0); break;
                      case 1: digitalWrite(Rly5, 1); break;
                      case 2: digitalWrite(Rly5, !digitalRead(Rly5)); break;
                    }
                    break;
                  case 6:
                    switch(action) {
                      case 0: digitalWrite(Rly6, 0); break;
                      case 1: digitalWrite(Rly6, 1); break;
                      case 2: digitalWrite(Rly6, !digitalRead(Rly6)); break;
                    }
                    break;
                  case 7:
                    switch(action) {
                      case 0: digitalWrite(Rly7, 0); break;
                      case 1: digitalWrite(Rly7, 1); break;
                      case 2: digitalWrite(Rly7, !digitalRead(Rly7)); break;
                    }
                    break;
                  case 8:
                    switch(action) {
                      case 0: digitalWrite(Rly8, 0); break;
                      case 1: digitalWrite(Rly8, 1); break;
                      case 2: digitalWrite(Rly8, !digitalRead(Rly8)); break;
                    }
                    break;                
                }
            }
              String json = "{\"relays\":[ " + String(digitalRead(Rly1)) + "," + String(digitalRead(Rly2)) + "," 
                + String(digitalRead(Rly3)) + ","  + String(digitalRead(Rly4)) + "," + String(digitalRead(Rly5)) + "," 
                + String(digitalRead(Rly6)) + "," + String(digitalRead(Rly7)) + ","  + String(digitalRead(Rly8)) + "],"
                "\"inputs\":[ " + String(digitalRead(Inp1)) + "," + String(digitalRead(Inp2)) + "," + String(digitalRead(Inp3)) + ","
              + String(digitalRead(Inp4)) + "," + String(digitalRead(Inp5)) + "," + String(digitalRead(Inp6)) + ","
              + String(digitalRead(Inp7)) + "," + String(digitalRead(Inp8))+ "]}";
              Server.send(200, "text/json", json);
              //Serial.println(json);
              json = String();
 }           



void setupHttpServer(){
  //SERVER INIT
  //get all GPIO statuses in one json call
  Server.on("/gpio", HTTP_GET, handleRelayPageJson);
  Server.on("/autoconnectMenu", HTTP_GET, handleMenuHtml);
  
  //list directory
  Server.on("/list", HTTP_GET, handleFileList);
  //load editor
  Server.on("/edit", HTTP_GET, []() {
    if (!handleFileRead("/edit.htm")) {
      Server.send(404, "text/plain", "FileNotFound");
    }
  });
  //create file
  Server.on("/edit", HTTP_PUT, handleFileCreate);
  //delete file
  Server.on("/edit", HTTP_DELETE, handleFileDelete);
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  Server.on("/edit", HTTP_POST, []() {
    Server.send(200, "text/plain", "");
  }, handleFileUpload);
  
  //called when the url is not defined here
  //use it to load content from FILESYSTEM
  Server.onNotFound([]() {
    if (!handleFileRead(Server.uri())) {
      Server.send(404, "text/plain", "FileNotFound");
    }
  });

  
}
