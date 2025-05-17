// #include <ESP8266WiFi.h>
// #include <ESP8266WebServer.h>
// #include <WiFiUdp.h>
// #include <ArduinoOTA.h>
// #include <Servo.h>

// #include <IRremoteESP8266.h>
// #include <IRrecv.h>
// #include <IRutils.h>

// const char* ssid = "Seco";
// const char* password = "secosanpq1";

// ESP8266WebServer server(80);

// Servo myServo;   // Servo 1
// Servo myServo2;  // Servo 2
// Servo myServo3;  // Servo 3

// bool isAttached = false;
// bool isAttached2 = false;

// // L298N pin definitions
// const int IN1 = D2;
// const int IN2 = D3;
// const int ENA = D4;

// const int IN3 = D6;
// const int IN4 = D7;
// const int ENB = 3;

// // IR receiver pe pinul TX (GPIO1)
// const uint16_t RECV_PIN = D5;  // TX = GPIO1
// IRrecv irrecv(RECV_PIN);
// decode_results results;
// unsigned long irCode = 0;

// String lastIRCode = "";

// // Coduri IR cunoscute (le poți obține mai jos)
// const unsigned long CODE_1 = 0xFFA25D;
// const unsigned long CODE_2 = 0xFF629D;


// // Pentru servo2 oscilatie
// int servo2Pos = 90;
// bool servo2Increasing = true;
// bool servo2Oscillating = false;
// int servo2OscDelay = 300;
// int servo2Step = 2;
// int servo2Min = 60;
// int servo2Max = 100;


// unsigned long lastOscTime = 0;
    

// // --- FUNCTII UTILE ---

// void setServo2Angle(int targetAngle) {
//   myServo2.attach(D8, 500, 2400);
//   int currentAngle = servo2Pos;

//   if (targetAngle > currentAngle) {
//     for (int angle = currentAngle; angle <= targetAngle; angle++) {
//       myServo2.write(angle);
//       delay(10);
//     }
//   } else {
//     for (int angle = currentAngle; angle >= targetAngle; angle--) {
//       myServo2.write(angle);
//       delay(10);
//     }
//   }

//   servo2Pos = targetAngle;
//   // myServo2.detach(); // Optional, dacă vrei să detasezi servo-ul
// }

// // --- HANDLERE ---

// void handleRoot() {
//   String html = R"rawliteral(
//     <html>
//     <body>
//      <h2>Cod IR primit:</h2>
//   <p id="irDisplay">N/A</p>

//       <h2>Control Servo Continuu</h2>
//       <button onclick="fetch('/start0')">Porneste 92°</button>
//       <button onclick="fetch('/start180')">Porneste 180°</button>
//       <button onclick="fetch('/stop')">Opreste Servo</button>
//       <br><br>
//       <input type="number" id="val" min="0" max="180" placeholder="Viteza servo">
//       <button onclick="setSpeed()">Seteaza viteza</button>

//       <h2>Control Motor DC Colector</h2>
//       <button onclick="fetch('/motorFWD')">Inainte 150</button>
//       <button onclick="fetch('/motorBWD')">Inapoi</button>
//       <button onclick="fetch('/motorStop')">Opreste Motor</button>
//       <br><br>
//       <input type="number" id="dcSpeed" min="0" max="1023" placeholder="Viteza motor">
//       <button onclick="setMotorSpeed()">Seteaza viteza</button>

//       <h2>Control Motor DC Tun</h2>
//       <button onclick="fetch('/motor2FWD')">Inainte 150</button>
//       <button onclick="fetch('/motor2BWD')">Inapoi</button>
//       <button onclick="fetch('/motor2Stop')">Opreste Motor</button>
//       <br><br>
//       <input type="number" id="dcSpeed2" min="0" max="1023" placeholder="Viteza motor 2">
//       <button onclick="setMotor2Speed()">Seteaza viteza</button>

//       <h2>Oscilatie fixa</h2>
//       <button onclick="fetch('/servo2_100')">Pozitie 100°</button>
//       <button onclick="fetch('/servo2_60')">Pozitie 60°</button>
//       <button onclick="fetch('/servo2_stop')">Opreste Servo 2</button>
//       <br><br>
//       <input type="number" id="servo2val" min="0" max="180" placeholder="Unghi custom">
//       <button onclick="setServo2()">Seteaza unghi</button>

//       <h2>Oscilatie</h2>
//       <label for="oscSpeed">Viteza (ms):</label>
//       <input type="number" id="oscSpeed" value="30" min="5" max="2000" step="50"><br>
//       <label for="oscStep">Pas (grade):</label>
//       <input type="number" id="oscStep" value="2" min="1" max="30" step="1"><br>
//       <label for="oscMin">Minim (°):</label>
//       <input type="number" id="oscMin" value="60" min="0" max="180"><br>
//       <label for="oscMax">Maxim (°):</label>
//       <input type="number" id="oscMax" value="100" min="0" max="180"><br><br>
//       <button onclick="startOscillation()">Porneste oscilatie</button>
//       <button onclick="fetch('/servo2_osc_stop')">Opreste oscilatie</button>

//       <h2>Inaltime</h2>
//       <button onclick="fetch('/servo3_100')">Pozitie 100°</button>
//       <button onclick="fetch('/servo3_75')">Pozitie 75°</button>
//       <button onclick="fetch('/servo3_stop')">Opreste Servo 3</button>
//       <br><br>
//       <input type="number" id="servo3val" min="0" max="180" placeholder="Unghi custom">
//       <button onclick="setServo3()">Seteaza unghi</button>

//       <script>
//        setInterval(() => {
//       fetch('/getir')
//         .then(response => response.text())
//         .then(data => {
//           document.getElementById('irDisplay').innerText = data;
//         });
//     }, 500); // actualizează la fiecare 500ms

//         function setSpeed() {
//           let val = document.getElementById('val').value;
//           fetch("/set?val=" + val);
//         }
//         function setMotorSpeed() {
//           let val = document.getElementById('dcSpeed').value;
//           fetch("/motorSet?val=" + val);
//         }
//         function setServo2() {
//           let val = document.getElementById('servo2val').value;
//           fetch("/servo2_set?val=" + val);
//         }
//         function startOscillation() {
//           const speed = document.getElementById('oscSpeed').value;
//           const step = document.getElementById('oscStep').value;
//           const min = document.getElementById('oscMin').value;
//           const max = document.getElementById('oscMax').value;
//           fetch(`/servo2_osc_start?speed=${speed}&step=${step}&min=${min}&max=${max}`);
//         }
//         function setServo3() {
//           let val = document.getElementById('servo3val').value;
//           fetch("/servo3_set?val=" + val);
//         }
//         function setMotor2Speed() {
//           let val = document.getElementById('dcSpeed2').value;
//           fetch("/motor2Set?val=" + val);
//         }
//       </script>
//     </body>
//     </html>
//   )rawliteral";
//   server.send(200, "text/html", html);
// }

// // Servo 1 handlers
// void handleStart0() {
//   if (!isAttached) {
//     myServo.attach(D1);
//     isAttached = true;
//   }
//   myServo.write(92);
//   server.send(200, "text/plain", "Pornit la 92°");
// }

// void handleStart180() {
//   if (!isAttached) {
//     myServo.attach(D1);
//     isAttached = true;
//   }
//   myServo.write(180);
//   server.send(200, "text/plain", "Pornit la 180°");
// }

// void handleStop() {
//   if (isAttached) {
//     myServo.detach();
//     isAttached = false;
//   }
//   server.send(200, "text/plain", "Servo oprit");
// }

// void handleSet() {
//   if (server.hasArg("val")) {
//     int val = constrain(server.arg("val").toInt(), 0, 180);
//     if (!isAttached) {
//       myServo.attach(D1);
//       isAttached = true;
//     }
//     myServo.write(val);
//   }
//   server.send(200, "text/plain", "Viteza servo setata");
// }

// // Motor DC handlers
// void handleMotorFWD() {
//   digitalWrite(IN1, HIGH);
//   digitalWrite(IN2, LOW);
//   analogWrite(ENA, 150);
//   server.send(200, "text/plain", "Motor inainte");
// }

// void handleMotorBWD() {
//   digitalWrite(IN1, LOW);
//   digitalWrite(IN2, HIGH);
//   analogWrite(ENA, 135);
//   server.send(200, "text/plain", "Motor inapoi");
// }

// void handleMotorStop() {
//   digitalWrite(IN1, LOW);
//   digitalWrite(IN2, LOW);
//   analogWrite(ENA, 0);
//   server.send(200, "text/plain", "Motor oprit");
// }

// void handleMotorSetSpeed() {
//   if (server.hasArg("val")) {
//     int pwm = constrain(server.arg("val").toInt(), 0, 1023);
//     analogWrite(ENA, pwm);
//     digitalWrite(IN1, HIGH);
//     digitalWrite(IN2, LOW);
//   }
//   server.send(200, "text/plain", "Viteza motor setata");
// }

// // Motor DC 2 handlers
// void handleMotor2FWD() {
//   digitalWrite(IN3, HIGH);
//   digitalWrite(IN4, LOW);
//   analogWrite(ENB, 150);
//   server.send(200, "text/plain", "Motor 2 inainte");
// }

// void handleMotor2BWD() {
//   digitalWrite(IN3, LOW);
//   digitalWrite(IN4, HIGH);
//   analogWrite(ENB, 135);
//   server.send(200, "text/plain", "Motor 2 inapoi");
// }

// void handleMotor2Stop() {
//   digitalWrite(IN3, LOW);
//   digitalWrite(IN4, LOW);
//   analogWrite(ENB, 0);
//   server.send(200, "text/plain", "Motor 2 oprit");
// }

// void handleMotor2SetSpeed() {
//   if (server.hasArg("val")) {
//     int pwm = constrain(server.arg("val").toInt(), 0, 1023);
//     analogWrite(ENB, pwm);
//     digitalWrite(IN3, HIGH);
//     digitalWrite(IN4, LOW);
//   }
//   server.send(200, "text/plain", "Viteza motor 2 setata");
// }

// // Servo 2 fixed positions
// void handleServo2_100() {
//   servo2Oscillating = false;
//   setServo2Angle(100);
//   server.send(200, "text/plain", "Servo 2 la 100°");
// }

// void handleServo2_60() {
//   servo2Oscillating = false;
//   setServo2Angle(60);
//   server.send(200, "text/plain", "Servo 2 la 60°");
// }

// void handleServo2_stop() {
//   servo2Oscillating = false;
//   if (isAttached2) {
//     myServo2.detach();
//     isAttached2 = false;
//   }
//   server.send(200, "text/plain", "Servo 2 oprit");
// }

// void handleServo2_set() {
//   if (server.hasArg("val")) {
//     int val = constrain(server.arg("val").toInt(), 0, 180);
//     servo2Oscillating = false;
//     setServo2Angle(val);
//     server.send(200, "text/plain", "Servo 2 setat unghi");
//     return;
//   }
//   server.send(400, "text/plain", "Parametru val lipsa");
// }


// void handleServo2_osc_start() {
//   if (server.hasArg("speed") && server.hasArg("step") && server.hasArg("min") && server.hasArg("max")) {
//     servo2OscDelay = constrain(server.arg("speed").toInt(), 50, 2000);
//     servo2Step = constrain(server.arg("step").toInt(), 1, 30);
//     servo2Min = constrain(server.arg("min").toInt(), 0, 180);
//     servo2Max = constrain(server.arg("max").toInt(), 0, 180);

//     if (servo2Min >= servo2Max) {
//       server.send(400, "text/plain", "Min trebuie mai mic decat max");
//       return;
//     }

//     if (!isAttached2) {
//       myServo2.attach(D8, 500, 2400);
//       isAttached2 = true;
//     }

//     // Rampa lină până la servo2Min
//     int currentPos = myServo2.read();  // Ultimul unghi scris
//     if (currentPos < servo2Min) {
//       for (int p = currentPos; p <= servo2Min; p++) {
//         myServo2.write(p);
//         delay(10);  // Delay mic pentru tranziție lină
//       }
//     } else if (currentPos > servo2Min) {
//       for (int p = currentPos; p >= servo2Min; p--) {
//         myServo2.write(p);
//         delay(10);
//       }
//     }

//     // Inițializează oscilația
//     servo2Pos = servo2Min;
//     servo2Increasing = true;
//     servo2Oscillating = true;

//     server.send(200, "text/plain", "Oscilatie pornita");
//   } else {
//     server.send(400, "text/plain", "Parametri lipsa");
//   }
// }



// void handleServo2_osc_stop() {
//   servo2Oscillating = false;
//   if (isAttached2) {
//     myServo2.detach();
//     isAttached2 = false;
//   }
//   server.send(200, "text/plain", "Oscilatie oprita");
// }

// // Servo 3 handlers
// Servo myServo3Instance;
// bool isAttached3 = false;

// void handleServo3_100() {
//   if (!isAttached3) {
//     myServo3.attach(D0);
//     isAttached3 = true;
//   }
//   myServo3.write(100);
//   server.send(200, "text/plain", "Servo 3 la 100°");
// }

// void handleServo3_75() {
//   if (!isAttached3) {
//     myServo3.attach(D0);
//     isAttached3 = true;
//   }
//   myServo3.write(75);
//   server.send(200, "text/plain", "Servo 3 la 75°");
// }

// void handleServo3_stop() {
//   if (isAttached3) {
//     myServo3.detach();
//     isAttached3 = false;
//   }
//   server.send(200, "text/plain", "Servo 3 oprit");
// }

// void handleServo3_set() {
//   if (server.hasArg("val")) {
//     int val = constrain(server.arg("val").toInt(), 0, 180);
//     if (!isAttached3) {
//       myServo3.attach(D0);
//       isAttached3 = true;
//     }
//     myServo3.write(val);
//     server.send(200, "text/plain", "Servo 3 setat unghi");
//     return;
//   }
//   server.send(400, "text/plain", "Parametru val lipsa");
// }

// // --- SETUP ---

// void setup() {
//   Serial.begin(115200);
//   delay(100);

//   pinMode(IN1, OUTPUT);
//   pinMode(IN2, OUTPUT);
//   pinMode(ENA, OUTPUT);

//   pinMode(IN3, OUTPUT);
//   pinMode(IN4, OUTPUT);
//   pinMode(ENB, OUTPUT);

//   digitalWrite(IN1, LOW);
//   digitalWrite(IN2, LOW);
//   analogWrite(ENA, 0);

//   digitalWrite(IN3, LOW);
//   digitalWrite(IN4, LOW);
//   analogWrite(ENB, 0);

//   irrecv.enableIRIn();  // Pornește receptorul IR

//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);

//   Serial.print("Connecting to WiFi");
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

//   ArduinoOTA.setHostname("esp8266-Servo");
//   ArduinoOTA.begin();

//   server.on("/", handleRoot);

// //ir
//   server.on("/getir", []() {
//     server.send(200, "text/plain", lastIRCode);
//   });

  
//   // Servo 1
//   server.on("/start0", handleStart0);
//   server.on("/start180", handleStart180);
//   server.on("/stop", handleStop);
//   server.on("/set", handleSet);

//   // Motor DC 1
//   server.on("/motorFWD", handleMotorFWD);
//   server.on("/motorBWD", handleMotorBWD);
//   server.on("/motorStop", handleMotorStop);
//   server.on("/motorSet", handleMotorSetSpeed);

//   // Motor DC 2
//   server.on("/motor2FWD", handleMotor2FWD);
//   server.on("/motor2BWD", handleMotor2BWD);
//   server.on("/motor2Stop", handleMotor2Stop);
//   server.on("/motor2Set", handleMotor2SetSpeed);

//   // Servo 2 fixed
//   server.on("/servo2_100", handleServo2_100);
//   server.on("/servo2_60", handleServo2_60);
//   server.on("/servo2_stop", handleServo2_stop);
//   server.on("/servo2_set", handleServo2_set);

//   // Servo 2 oscillation
//   server.on("/servo2_osc_start", handleServo2_osc_start);
//   server.on("/servo2_osc_stop", handleServo2_osc_stop);

//   // Servo 3
//   server.on("/servo3_100", handleServo3_100);
//   server.on("/servo3_75", handleServo3_75);
//   server.on("/servo3_stop", handleServo3_stop);
//   server.on("/servo3_set", handleServo3_set);

//   server.begin();
//   Serial.println("HTTP server started");
// }

// // --- LOOP ---
// // e cat de cat ok cu viteza 30 si pas 2
// void loop() {
//   ArduinoOTA.handle();
//   server.handleClient();

//   // Oscilatie servo 2 lină
//   if (servo2Oscillating && millis() - lastOscTime >= servo2OscDelay) {
//     lastOscTime = millis();

//     // Mișcare incrementală
//     if (servo2Increasing) {
//       if (servo2Pos + servo2Step <= servo2Max) {
//         servo2Pos += servo2Step;
//       } else {
//         servo2Pos = servo2Max;
//         servo2Increasing = false;
//       }
//     } else {
//       if (servo2Pos - servo2Step >= servo2Min) {
//         servo2Pos -= servo2Step;
//       } else {
//         servo2Pos = servo2Min;
//         servo2Increasing = true;
//       }
//     }

//     myServo2.write(servo2Pos);
//   }




//   // // IR check
//   // if (irrecv.decode(&results)) {
//   //   // Serial.println(resultToHexidecimal(&results));  // Vezi codul în monitorul serial
//   //   irCode = results.value;  // salvăm codul pentru HTML

//   //   // String code = resultToHexidecimal(&results);
//   //   if (irCode == 0xc01aeeff) {  // Exemplu: butonul POWER de pe telecomanda NEC
//   //     handleMotor2BWD();  // Apelează direct funcția
//   //   }  else if (irCode == 0xc9db0ced) {
//   //     handleMotor2Stop();  // Apelează direct funcția
//   //   } 

//   //   irrecv.resume();  // Pregătește următoarea citire
//   // }

//   // if (irrecv.decode(&results)) {
//   //   lastIRCode = "0x" + String(results.value, HEX);
//   //   Serial.println("Cod IR: " + lastIRCode);

//   //   // acțiuni
//   //   if (results.value == CODE_1) handleMotor2BWD();
//   //   if (results.value == CODE_2) handleMotor2Stop();

//   //   irrecv.resume();
//   // }

// }





#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Servo.h>

#include <ESP8266HTTPClient.h>


#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>


#define IR_RECEIVE_PIN D5 // Pinul TX pe ESP8266, vezi dacă merge la tine
IRrecv irrecv(IR_RECEIVE_PIN);
decode_results results;

WiFiClient client;  // trebuie să ai un obiect WiFiClient

const char* ssid = "Seco";
const char* password = "secosanpq1";

ESP8266WebServer server(80);

Servo myServo;   // Servo 1
Servo myServo2;  // Servo 2
Servo myServo3;  // Servo 3

bool isAttached = false;
bool isAttached2 = false;

// L298N pin definitions
const int IN1 = D2;
const int IN2 = D3;
const int ENA = D4;

const int IN3 = D6;
const int IN4 = D7;
const int ENB = 3;


// Pentru servo2 oscilatie
int servo2Pos = 90;
bool servo2Increasing = true;
bool servo2Oscillating = false;
int servo2OscDelay = 30;
int servo2Step = 2;
int servo2Min = 60;
int servo2Max = 100;


unsigned long lastOscTime = 0;
    

// --- FUNCTII UTILE ---

void setServo2Angle(int targetAngle) {
  myServo2.attach(D8, 500, 2400);
  int currentAngle = servo2Pos;

  if (targetAngle > currentAngle) {
    for (int angle = currentAngle; angle <= targetAngle; angle++) {
      myServo2.write(angle);
      delay(10);
    }
  } else {
    for (int angle = currentAngle; angle >= targetAngle; angle--) {
      myServo2.write(angle);
      delay(10);
    }
  }

  servo2Pos = targetAngle;
  // myServo2.detach(); // Optional, dacă vrei să detasezi servo-ul
}

// --- HANDLERE ---

void handleRoot() {
  String html = R"rawliteral(
    <html>
    <body>
     <h2>Cod IR primit:</h2>
  <p id="irDisplay">N/A</p>

      <h2>Control Servo Continuu</h2>
      <button onclick="fetch('/start0')">Porneste 92°</button>
      <button onclick="fetch('/start180')">Porneste 180°</button>
      <button onclick="fetch('/stop')">Opreste Servo</button>
      <br><br>
      <input type="number" id="val" min="0" max="180" placeholder="Viteza servo">
      <button onclick="setSpeed()">Seteaza viteza</button>

      <h2>Control Motor DC Colector</h2>
      <button onclick="fetch('/motorFWD')">Inainte 150</button>
      <button onclick="fetch('/motorBWD')">Inapoi</button>
      <button onclick="fetch('/motorStop')">Opreste Motor</button>
      <br><br>
      <input type="number" id="dcSpeed" min="0" max="1023" placeholder="Viteza motor">
      <button onclick="setMotorSpeed()">Seteaza viteza</button>

      <h2>Control Motor DC Tun</h2>
      <button onclick="fetch('/motor2FWD')">Inainte 150</button>
      <button onclick="fetch('/motor2BWD')">Inapoi</button>
      <button onclick="fetch('/motor2Stop')">Opreste Motor</button>
      <br><br>
      <input type="number" id="dcSpeed2" min="0" max="1023" placeholder="Viteza motor 2">
      <button onclick="setMotor2Speed()">Seteaza viteza</button>

      <h2>Oscilatie fixa</h2>
      <button onclick="fetch('/servo2_100')">Pozitie 100°</button>
      <button onclick="fetch('/servo2_60')">Pozitie 60°</button>
      <button onclick="fetch('/servo2_stop')">Opreste Servo 2</button>
      <br><br>
      <input type="number" id="servo2val" min="0" max="180" placeholder="Unghi custom">
      <button onclick="setServo2()">Seteaza unghi</button>

      <h2>Oscilatie</h2>
      <label for="oscSpeed">Viteza (ms):</label>
      <input type="number" id="oscSpeed" value="30" min="5" max="2000" step="50"><br>
      <label for="oscStep">Pas (grade):</label>
      <input type="number" id="oscStep" value="2" min="1" max="30" step="1"><br>
      <label for="oscMin">Minim (°):</label>
      <input type="number" id="oscMin" value="60" min="0" max="180"><br>
      <label for="oscMax">Maxim (°):</label>
      <input type="number" id="oscMax" value="100" min="0" max="180"><br><br>
      <button onclick="startOscillation()">Porneste oscilatie</button>
      <button onclick="fetch('/servo2_osc_stop')">Opreste oscilatie</button>

      <h2>Inaltime</h2>
      <button onclick="fetch('/servo3_100')">Pozitie 100°</button>
      <button onclick="fetch('/servo3_75')">Pozitie 75°</button>
      <button onclick="fetch('/servo3_stop')">Opreste Servo 3</button>
      <br><br>
      <input type="number" id="servo3val" min="0" max="180" placeholder="Unghi custom">
      <button onclick="setServo3()">Seteaza unghi</button>

      <script>
       setInterval(() => {
      fetch('/getir')
        .then(response => response.text())
        .then(data => {
          document.getElementById('irDisplay').innerText = data;
        });
    }, 500); // actualizează la fiecare 500ms

        function setSpeed() {
          let val = document.getElementById('val').value;
          fetch("/set?val=" + val);
        }
        function setMotorSpeed() {
          let val = document.getElementById('dcSpeed').value;
          fetch("/motorSet?val=" + val);
        }
        function setServo2() {
          let val = document.getElementById('servo2val').value;
          fetch("/servo2_set?val=" + val);
        }
        function startOscillation() {
          const speed = document.getElementById('oscSpeed').value;
          const step = document.getElementById('oscStep').value;
          const min = document.getElementById('oscMin').value;
          const max = document.getElementById('oscMax').value;
          fetch(`/servo2_osc_start?speed=${speed}&step=${step}&min=${min}&max=${max}`);
        }
        function setServo3() {
          let val = document.getElementById('servo3val').value;
          fetch("/servo3_set?val=" + val);
        }
        function setMotor2Speed() {
          let val = document.getElementById('dcSpeed2').value;
          fetch("/motor2Set?val=" + val);
        }
      </script>
    </body>
    </html>
  )rawliteral";
  server.send(200, "text/html", html);
}

// Servo 1 handlers
void handleStart0() {
  if (!isAttached) {
    myServo.attach(D1);
    isAttached = true;
  }
  myServo.write(92);
  server.send(200, "text/plain", "Pornit la 92°");
}

void handleStart180() {
  if (!isAttached) {
    myServo.attach(D1);
    isAttached = true;
  }
  myServo.write(180);
  server.send(200, "text/plain", "Pornit la 180°");
}

void handleStop() {
  if (isAttached) {
    myServo.detach();
    isAttached = false;
  }
  server.send(200, "text/plain", "Servo oprit");
}

void handleSet() {
  if (server.hasArg("val")) {
    int val = constrain(server.arg("val").toInt(), 0, 180);
    if (!isAttached) {
      myServo.attach(D1);
      isAttached = true;
    }
    myServo.write(val);
  }
  server.send(200, "text/plain", "Viteza servo setata");
}

// Motor DC handlers
void handleMotorFWD() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 150);
  server.send(200, "text/plain", "Motor inainte");
}

void handleMotorBWD() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 135);
  server.send(200, "text/plain", "Motor inapoi");
}

void handleMotorStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
  server.send(200, "text/plain", "Motor oprit");
}

void handleMotorSetSpeed() {
  if (server.hasArg("val")) {
    int pwm = constrain(server.arg("val").toInt(), 0, 1023);
    analogWrite(ENA, pwm);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  server.send(200, "text/plain", "Viteza motor setata");
}

// Motor DC 2 handlers
void handleMotor2FWD() {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 150);
  server.send(200, "text/plain", "Motor 2 inainte");
}

void handleMotor2BWD() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 135);
  server.send(200, "text/plain", "Motor 2 inapoi");
}

void handleMotor2Stop() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
  server.send(200, "text/plain", "Motor 2 oprit");
}

void handleMotor2SetSpeed() {
  if (server.hasArg("val")) {
    int pwm = constrain(server.arg("val").toInt(), 0, 1023);
    analogWrite(ENB, pwm);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  server.send(200, "text/plain", "Viteza motor 2 setata");
}

// Servo 2 fixed positions
void handleServo2_100() {
  servo2Oscillating = false;
  setServo2Angle(100);
  server.send(200, "text/plain", "Servo 2 la 100°");
}

void handleServo2_80() {
  servo2Oscillating = false;
  setServo2Angle(80);
  server.send(200, "text/plain", "Servo 2 la 80°");
}

void handleServo2_60() {
  servo2Oscillating = false;
  setServo2Angle(60);
  server.send(200, "text/plain", "Servo 2 la 60°");
}

void handleServo2_stop() {
  servo2Oscillating = false;
  if (isAttached2) {
    myServo2.detach();
    isAttached2 = false;
  }
  server.send(200, "text/plain", "Servo 2 oprit");
}

void handleServo2_set() {
  if (server.hasArg("val")) {
    int val = constrain(server.arg("val").toInt(), 0, 180);
    servo2Oscillating = false;
    setServo2Angle(val);
    server.send(200, "text/plain", "Servo 2 setat unghi");
    return;
  }
  server.send(400, "text/plain", "Parametru val lipsa");
}

// //e bun dar il modific ca sa mearga si ir remote pentru ca nu poate lua valorile standard din html
// void handleServo2_osc_start() {
//   if (server.hasArg("speed") && server.hasArg("step") && server.hasArg("min") && server.hasArg("max")) {
//     servo2OscDelay = constrain(server.arg("speed").toInt(), 50, 2000);
//     servo2Step = constrain(server.arg("step").toInt(), 1, 30);
//     servo2Min = constrain(server.arg("min").toInt(), 0, 180);
//     servo2Max = constrain(server.arg("max").toInt(), 0, 180);

//     if (servo2Min >= servo2Max) {
//       server.send(400, "text/plain", "Min trebuie mai mic decat max");
//       return;
//     }

//     if (!isAttached2) {
//       myServo2.attach(D8, 500, 2400);
//       isAttached2 = true;
//     }

//     // Rampa lină până la servo2Min
//     int currentPos = myServo2.read();  // Ultimul unghi scris
//     if (currentPos < servo2Min) {
//       for (int p = currentPos; p <= servo2Min; p++) {
//         myServo2.write(p);
//         delay(10);  // Delay mic pentru tranziție lină
//       }
//     } else if (currentPos > servo2Min) {
//       for (int p = currentPos; p >= servo2Min; p--) {
//         myServo2.write(p);
//         delay(10);
//       }
//     }

//     // Inițializează oscilația
//     servo2Pos = servo2Min;
//     servo2Increasing = true;
//     servo2Oscillating = true;

//     server.send(200, "text/plain", "Oscilatie pornita");
//   } else {
//     server.send(400, "text/plain", "Parametri lipsa");
//   }
// }
void handleServo2_osc_start() {
  // Dacă există toți parametrii, actualizează valorile
  if (server.hasArg("speed") && server.hasArg("step") && server.hasArg("min") && server.hasArg("max")) {
    int tempDelay = constrain(server.arg("speed").toInt(), 50, 2000);
    int tempStep = constrain(server.arg("step").toInt(), 1, 30);
    int tempMin = constrain(server.arg("min").toInt(), 0, 180);
    int tempMax = constrain(server.arg("max").toInt(), 0, 180);

    if (tempMin >= tempMax) {
      server.send(400, "text/plain", "Min trebuie mai mic decat max");
      return;
    }

    // Aplică doar dacă totul e valid
    servo2OscDelay = tempDelay;
    servo2Step = tempStep;
    servo2Min = tempMin;
    servo2Max = tempMax;
  }

  // Atașează servo dacă nu e deja
  if (!isAttached2) {
    myServo2.attach(D8, 500, 2400);
    isAttached2 = true;
  }

  // Mută servo la poziția minimă inițială
  int currentPos = myServo2.read();  // Ultimul unghi scris
  if (currentPos < servo2Min) {
    for (int p = currentPos; p <= servo2Min; p++) {
      myServo2.write(p);
      delay(10);
    }
  } else if (currentPos > servo2Min) {
    for (int p = currentPos; p >= servo2Min; p--) {
      myServo2.write(p);
      delay(10);
    }
  }

  servo2Pos = servo2Min;
  servo2Increasing = true;
  servo2Oscillating = true;

  server.send(200, "text/plain", "Oscilatie servo2 pornita.");
}



void handleServo2_osc_stop() {
  servo2Oscillating = false;
  if (isAttached2) {
    myServo2.detach();
    isAttached2 = false;
  }
  server.send(200, "text/plain", "Oscilatie oprita");
}

// Servo 3 handlers
Servo myServo3Instance;
bool isAttached3 = false;

void handleServo3_100() {
  if (!isAttached3) {
    myServo3.attach(D0);
    isAttached3 = true;
  }
  myServo3.write(100);
  server.send(200, "text/plain", "Servo 3 la 100°");
}

void handleServo3_50() {
  if (!isAttached3) {
    myServo3.attach(D0);
    isAttached3 = true;
  }
  myServo3.write(50);
  server.send(200, "text/plain", "Servo 3 la 50°");
}

void handleServo3_stop() {
  if (isAttached3) {
    myServo3.detach();
    isAttached3 = false;
  }
  server.send(200, "text/plain", "Servo 3 oprit");
}

void handleServo3_set() {
  if (server.hasArg("val")) {
    int val = constrain(server.arg("val").toInt(), 0, 180);
    if (!isAttached3) {
      myServo3.attach(D0);
      isAttached3 = true;
    }
    myServo3.write(val);
    server.send(200, "text/plain", "Servo 3 setat unghi");
    return;
  }
  server.send(400, "text/plain", "Parametru val lipsa");
}

// Function prototype for sendHttp
void sendHttp(String url);

// --- SETUP ---
void sendHttp(String url) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(client, url);   // Aici folosești clientul explicit
    int httpCode = http.GET();
    Serial.printf("GET %s -> %d\n", url.c_str(), httpCode);
    http.end();
  } else {
    Serial.println("WiFi deconectat!");
  }
}

String lastIRCode = "N/A";  // valoare inițială


void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);

  // irrecv.enableIRIn();  // Pornește receptorul IR
  irrecv.enableIRIn();  // Pornește receptorul IR

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  ArduinoOTA.setHostname("esp8266-Servo");
  ArduinoOTA.begin();

  server.on("/", handleRoot);

//ir
  server.on("/getir", []() {
    server.send(200, "text/plain", lastIRCode);
  });
  
  Serial.println(F("IR control pornit. Folosește telecomanda."));

  // Servo 1
  server.on("/start0", handleStart0);
  server.on("/start180", handleStart180);
  server.on("/stop", handleStop);
  server.on("/set", handleSet);

  // Motor DC 1
  server.on("/motorFWD", handleMotorFWD);
  server.on("/motorBWD", handleMotorBWD);
  server.on("/motorStop", handleMotorStop);
  server.on("/motorSet", handleMotorSetSpeed);

  // Motor DC 2
  server.on("/motor2FWD", handleMotor2FWD);
  server.on("/motor2BWD", handleMotor2BWD);
  server.on("/motor2Stop", handleMotor2Stop);
  server.on("/motor2Set", handleMotor2SetSpeed);

  // Servo 2 fixed
  server.on("/servo2_100", handleServo2_100);
  server.on("/servo2_60", handleServo2_60);
  server.on("/servo2_stop", handleServo2_stop);
  server.on("/servo2_set", handleServo2_set);

  // Servo 2 oscillation
  server.on("/servo2_osc_start", handleServo2_osc_start);
  server.on("/servo2_osc_stop", handleServo2_osc_stop);

  // Servo 3
  server.on("/servo3_100", handleServo3_100);
  server.on("/servo3_75", handleServo3_50);
  server.on("/servo3_stop", handleServo3_stop);
  server.on("/servo3_set", handleServo3_set);

  server.begin();
  Serial.println("HTTP server started");
}


// --- LOOP ---
void loop() {
  ArduinoOTA.handle();
  server.handleClient();

  // Oscilatie servo 2 lină
  if (servo2Oscillating && millis() - lastOscTime >= servo2OscDelay) {
    lastOscTime = millis();

    // Mișcare incrementală
    if (servo2Increasing) {
      if (servo2Pos + servo2Step <= servo2Max) {
        servo2Pos += servo2Step;
      } else {
        servo2Pos = servo2Max;
        servo2Increasing = false;
      }
    } else {
      if (servo2Pos - servo2Step >= servo2Min) {
        servo2Pos -= servo2Step;
      } else {
        servo2Pos = servo2Min;
        servo2Increasing = true;
      }
    }

    myServo2.write(servo2Pos);
  }

  // IR Remote
  if (irrecv.decode(&results)) {
    uint32_t code = results.value;
    Serial.printf("Cod IR: 0x%X\n", code);
   
 // Generează string-ul exact ca în if-uri pentru html
 if (code != 0xFFFFFFFF) {
      char hexCode[11];
      sprintf(hexCode, "0x%08X", code);  // format HEX complet
      lastIRCode = String(hexCode);
 }
/////

    if (code == 0xFFA25D) {
      handleMotorBWD(); //Colector de mingi
    } else if (code == 0xFF629D) {
      handleMotorStop(); 
    } else if (code == 0xFFE21D) {
      handleStart0(); //Ball feeder
    }else if (code == 0xFF22DD) {
      handleStop();
    } else if (code == 0xFF02FD) {
      handleMotor2FWD(); //Tun
    }else if (code == 0xFFC23D) {
      handleMotor2Stop();
    } else if (code ==  0xFFE01F) {
      handleServo2_osc_start(); //Oscilatie
    }else if (code == 0xFFA857) {
      handleServo2_osc_stop();
    } else if (code ==  0xFF906F) {
      handleServo2_100(); //dreapta
    }else if (code == 0xFF6897) {
      handleServo2_80(); //mijloc
    } else if (code ==  0xFF9867) {
      handleServo2_60(); //stanga
    }else if (code == 0xFFB04F) {
      handleServo3_50(); //sus
    } else if (code ==   0xFF30CF) {
      handleServo3_100(); //Jos
    }else if (code == 0xFF18E7) {
      handleServo2_stop(); //Stop Servo oscilatie
    } else if (code ==  0xFF7A85) {
      handleServo3_stop(); //Stop servo inaltime
    }
    irrecv.resume();
  }
}



