#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <AsyncTCP.h>
#include "MS5837.h"
#include <Wire.h>
MS5837 sensor;
int floo = 7;
float chuan = 3.0;
float h = 0; // do cao

float t; int k = 0;
float fist[2];
const char* ssid = "SSA_LAB";
const char* password = "12345687990";


AsyncWebServer server(80);

  const char index_html[] PROGMEM = R"rawliteral( 
 <!DOCTYPE html> <html>
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="../style/style.css">
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css">
    <title>Wings</title>
    <style>
 body{
    margin: 0 !important;
    padding: 0 !important;
 }
 .background{
    height: 1265px;
    background-color: rgb(237, 238, 241);
    width:100%;
    background-size: cover;
    background-repeat: no-repeat;
    background-position: center;
    overflow-x:hidden;
  }
.text{
    text-align: center;
    color: #0e0d0d;
    padding: 25px;
    font-size: 25px;
    font-weight: 600;
}
.btn-start{
    text-align: center;
}
.btn-sensor{
    text-align:  center;
    padding-top: 10px;
    color:#0e0d0d;
    font-size: 20px;
}
button{
    background-color: rgb(80, 176, 181);
    border-radius:20px;
    padding: 15px;
    font-size: 25px;
    color: rgb(56, 76, 74); padding: 15px;
    cursor: pointer;
}
.content-center {
  margin-top: 50px;
  margin-left: 362px;
  size: auto;  

}
.footer{
    float: left;
    width: 100%;
    color: white;
    min-height: 150px;
    padding-top: 200px;
}
.row{
    float: left;
    width: 100%;
    margin-bottom: 20pt;
}
.boxcenter{
    width: 50%;
    margin: 0 auto; 
}
.boxfooter1{
    width: 55%;
    float: left;
 
}
.boxfooter2{
    width: 45%;
    float: left;
}
.textcenter{
    text-align: center;
    color: #1a1515;
}
.mr{
    margin-top: 10px;
    color:rgb(79, 84, 83);
}
.add{
    color:rgb(67, 63, 63);
    text-align: left;
}
.textlogo{
    text-align: center;
    font-size: 100px;
    color: red;
}
.color{
    color: #0e0d0d;
    text-align: left;
}
.icon{
    margin-left: 34%;
    font-size: 60px;
}
</style>
</head>
<script src="../js/basic.js" ></script>
<body> 
  <div class = "background">
    <div class = "textlogo"> FIREFIGHTING </div>
    <div class = "textlogo" > ASSISTANT </div>
    <div class="icon "> <i class="fas fa-wifi"></i>
        <i class="fas fa-fire"></i>
        <i class="fas fa-building"></i>
        <i class="fas fa-user-shield"></i>
        <i class="fas fa-user-circle"></i>
        <i class="fas fa-bell"></i>
        <i class="fas fa-tv"></i>
        <i class="fas fa-arrows-alt-v"></i>
    </div>
    <div class = "text">
        <span> Calculate altitude based on sensors </span>
    </div>
    <div class = "btn-start"> 
        <button>Result</button>  
    </div>
    <div class = "btn-sensor">
        <i class="fas fa-ruler-combined"></i>
        <span>Number of floors:</span> 
        <span id="floors">%FLOORS%</span>
    </div>
     
    <footer class = "footer">
        <div class="row">
            <div class="boxcenter">
                <div class="boxfooter1">
                    <h4 class="color">FOLLOW US</h4>
                    <div class="add mr"><a href="https://ssalab.phenikaa-uni.edu.vn/">https://ssalab.phenikaa-uni.edu.vn/</a></div>
                    <div class="add mr"><a href="https://www.facebook.com/wings0202">https://www.facebook.com/wings0202</a></div>
                    <div class="add mr"><a href="https://www.instagram.com/cah_c82/">https://www.instagram.com/cah_c82/</a></div>
                </div>
                <div class="boxfooter2">
                    <h4 class="color">CONTACT US</h4>
                    <div class="add mr"> Dong Vi, Dong La, Dong Hung, Thai Binh  </div>
                    <div class="add mr"> A4-704 SSA Lab Phenikaa University  </div>
                </div>
            </div>
        </div>
        <div class="row textcenter">
           By  Minh Lan and Canh Bui 
        </div>       
    </footer>>
  </div>
</body>
 <script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("floors").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/floors", true);
  xhttp.send();
}, 1000 ) ;
</script>
</html>)rawliteral";

int tmp;
int main(float fist, float second){
    tmp = fist - second;
    if(tmp == chuan){
      return 1;
    }
    if(tmp == -(chuan)){
      return -1;
    }
    return 0;
}
;
  String tranferdata(){
  float currentAltitude = (1-pow((sensor.pressure()/1013.25),.190284))*145366.45*.3048;
  if(k==0){
    fist[k] = currentAltitude;
  }
  k++;
  int floorChange = main(currentAltitude, fist[0]);
    floo += floorChange;
    if (floorChange != 0) 
    {
    fist[0] = currentAltitude;
    }
    return (String)floo;    
  }  
  // put your main code here, to run repeatedly:
 
String processor(const String& var){
  //Serial.println(var);
  if(var == "FLOORS"){
    return tranferdata();
  }
  return String();
}
void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
  // Connect to Wi-Fi
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());
   
  Wire.begin();
  sensor.init();
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  server.on("/floors", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", tranferdata().c_str());
  });
  // Start server
  server.begin(); 
}


void loop() {

}

  
