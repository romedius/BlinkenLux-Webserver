#include <Adafruit_NeoPixel.h>
#include <string.h>

/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>

const char* ssid = "it-syndikat";
const char* password = "spaceInvasion16";
//const char* ssid = "Heimnetz";
//const char* password = "Hallo321123A";

#define PIN 14
Adafruit_NeoPixel strip = Adafruit_NeoPixel(9, PIN, NEO_GRB + NEO_KHZ800);

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

enum STATE {
  HOME,
  INVALID,
  BLINK,
  LED,
  LEDS, 
  COLOR_R,
  COLOR_G,
  COLOR_B
};

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


void setup() {
  Serial.begin(115200);
  delay(10);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  strip.begin();
  strip.setPixelColor(0, strip.Color(0,  20,  0));
  strip.show();
  delay(300);
  strip.setPixelColor(0, strip.Color(0,  0,  0));
  strip.show();
  delay(300);
  strip.setPixelColor(0, strip.Color(0,  20,  0));
  strip.show();
  delay(300);
  strip.setPixelColor(0, strip.Color(0,  0,  0));
  strip.show();
}

void loop() {
  STATE state=INVALID;
  char *token;
  char str[50];
  int index = 0;
  static uint8_t color_r = 0;
  static uint8_t color_g = 0;
  static uint8_t color_b = 0;
  int led_index = 0;
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  req.toCharArray(str,50);
  token = strtok(str, "/");
  while( token != NULL )
  {
      //Serial.println(token);
      if(index == 1 && strcmp(token,"LED") == 0) {
        state = LED;
      } else if(index == 1 && strcmp(token,"LEDS") == 0) {
         state = LEDS;
      } else if(index == 1 && strcmp(token,"BLINK") == 0) {
        state = BLINK;
        theaterChaseRainbow(40);
        break;
      } else if (state == LEDS && index == 2) {
        led_index = String(token).toInt();
      } else if (state == LEDS && index == 3) {
        color_r = String(token).toInt();
      } else if (state == LEDS && index == 4) {
        color_g = String(token).toInt();
      } else if (state == LEDS && index == 5) {
        color_b = String(token).toInt();
        strip.setPixelColor(led_index, strip.Color(color_r , color_g, color_b));
        break;
      } else if (state == LED && index == 2) {
        color_r = String(token).toInt();
      } else if (state == LED && index == 3) {
        color_g = String(token).toInt();
      } else if (state == LED && index == 4) {
        color_b = String(token).toInt();
        for (int i=0;i<9;i++)
          strip.setPixelColor(i, strip.Color(color_r , color_g, color_b));
        break;
      } else {
        state = INVALID;
      }
      token = strtok(NULL, "/");
      index++;
  }

  if (state == INVALID && index > 3) {
    Serial.println("invalid request");
    client.stop();
    return;
  } else {
    strip.show();
  }
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
/*  s += "<!DOCTYPE HTML>\r\n<html>\r\nR = ";
  s += String(color_r);
  s += " B =";
  s += String(color_b);
  s += " G =";
  s += String(color_g);
  s += " </br> <a href=\"/LED/100/0/0\">Red</a> </br>"; 
  s += " <a href=\"/LED/0/100/0\">Green</a> </br>";
  s += " <a href=\"/LED/0/0/100\">Blue</a> </br>";
  s += " <a href=\"/LED/100/100/100\">White</a> </br>";
  s += " <a href=\"/LED/0/0/0\">OFF</a> </html>\n";
*/
  s += "<!DOCTYPEhtml> <html> <body>  <tableid=\"tableled\"style=\"width:100%\">  </table>  <script> function buildtable(){ varnode=document.getElementById(\"tableled\"); var row=\"\"; for(i=0;i<3;i++){ row+=\"<tr>\"; for(j=0;j<3;j++){ row+=\"<td>\"; row+=\"<table>\"+\"LED\"+(i*3+(j+1)); for(k=0;k<3;k++){ row+=\"<tr><td>\"; var array=[\"R\",\"G\",\"B\"]; var arraycolor=[\"red\",\"green\",\"blue\"] row+='<inputtype=\"range\"id=\"'+(i*3+(j+1))+array[k]+'\"value=\"0\"style=\"background:'+arraycolor[k]+';\">' row+=\"</td></tr>\"; } row+=\"</table>\"; row+=\"</td>\"; } row+=\"</tr>\"; } console.log(row); node.innerHTML=row; }  function sendleds(event){ var id=parseInt(event.id,10); var red=Math.floor(document.getElementById(id+\"R\").value*255/100); var green=Math.floor(document.getElementById(id+\"G\").value*255/100); var blue=Math.floor(document.getElementById(id+\"B\").value*255/100); var url=\"/LEDS/\"+(id-1)+\"/\"+red+\"/\"+green+\"/\"+blue; console.log(url); var ajax=newXMLHttpRequest(); ajax.open(\"GET\",url); ajax.send(null); }  buildtable(); var nodes=document.getElementsByTagName(\"input\"); for(i=0;i<nodes.length;i++){ nodes[i].addEventListener(\"change\",function(){sendleds(this);}); }  </script>  </body> </html> ":

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

