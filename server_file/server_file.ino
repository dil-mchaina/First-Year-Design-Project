#include <AcceleroMMA7361.h>
#include <Ethernet.h>
#include <SPI.h>
void stdDelay(EthernetClient a);
void warningPage(double minMag, double maxMag, EthernetClient a);
void earthquakeState(int x, int y, int z, int lowestAccel, int highestAccel, double minMag, double maxMag);

//accelerometer variables
AcceleroMMA7361 accelero;
int xAccel, yAccel, zAccel;
int xMax(0), yMax(0), zMax(0);
int printCounter(0);

//ethernet shield variables
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(10, 0, 1, 23);
EthernetServer server(80);
EthernetClient androidApp;

void setup()
{
 
 //accelerometer setup
 Serial.begin(9600);
 accelero.begin(2, 3, 4, 5, A3, A4, A5);
 accelero.setARefVoltage(5);                   //sets the AREF voltage to 3.3V
 accelero.setSensitivity(LOW);                   //sets the sensitivity to +/-6G
 accelero.calibrate();
 
 //ethernet shield setup
 Ethernet.begin(mac, ip);
 server.begin();
 Serial.print("server is at ");
 Serial.println(Ethernet.localIP());
}

void loop()
{
  xAccel = accelero.getXAccel();
  yAccel = accelero.getYAccel() - 2;
  zAccel = accelero.getZAccel() - 102;
  
  if (xAccel<0)
 {
   int xHolder = (-1)*xAccel;
   
   if (xHolder>xMax)
   {
     xMax = xHolder;
   }
 }
 
 else if (xAccel>0 && xAccel>xMax)
 {
 xMax = xAccel;
 }
 
 
 
  if (yAccel<0)
 {
   int yHolder = (-1)*yAccel;
   
   if (yHolder>yMax)
   {
     yMax = yHolder;
   }
 }
 
 else if (yAccel>0 && yAccel>yMax)
 {
 yMax = yAccel;
 }
 
  if (zAccel<0)
 {
   int zHolder = (-1)*zAccel;
   
   if (zHolder>zMax)
   {
     zMax = zHolder;
   }
 }
 
 else if (zAccel>0 && zAccel>zMax)
 {
 zMax = zAccel;
 }
 
 if (printCounter%300==0)
 {
 Serial.print("\n Max Acceleration in the X-Axis: ");
 Serial.print(xMax);
 Serial.print(" \t Max Acceleration in the Y-Axis: ");
 Serial.print(yMax);
 Serial.print(" \t Max Acceleration in the Z-Axis: ");
 Serial.print(zMax);
 }
 
 
 printCounter++;
                                    //make it readable
    
    // listen for incoming clients
    EthernetClient androidApp = server.available();
    if (androidApp) //if the app is connected to the server, the proceeding will happen
    
    {
    Serial.println("InstAlert server active!");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (androidApp.connected()) {
    if (androidApp.available()) {
    char c = androidApp.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          androidApp.println("HTTP/1.1 200 OK");
          androidApp.println("Content-Type: text/html");
          androidApp.println("Connection: close");  // the connection will be closed after completion of the response
          androidApp.println("Refresh: 0.001");  // refresh the page automatically every 5 sec
          androidApp.println();
          androidApp.println("<!DOCTYPE HTML>");
          androidApp.println("<html>");
          // output the value of each analog input pin
           {
            
            earthquakeState(xAccel, yAccel, zAccel, 19, 40, 3.0, 3.5, androidApp);
            earthquakeState(xAccel, yAccel, zAccel, 40, 121, 3.5, 4.0, androidApp);            
            earthquakeState(xAccel, yAccel, zAccel, 121, 388, 4.0, 4.5, androidApp);
            earthquakeState(xAccel, yAccel, zAccel, 388, 561, 4.5, 5.0, androidApp);
            earthquakeState(xAccel, yAccel, zAccel, 561, 700, 5.0, 5.5, androidApp);
            earthquakeState(xAccel, yAccel, zAccel, 700, 100000, 5.5, 6.0, androidApp);
            
            while (xAccel<=18 && yAccel<=18 && zAccel<=18)
            {
            androidApp.print("<html style=background-color:black><h1 style=background-color:black><a><p style=color:yellow;font-size:16px;font-family:Verdana>");
            androidApp.print("InstAlert Prototype");
            androidApp.println("<br />");
            androidApp.println("<br />");
            androidApp.print("X-Axis Acceleration: ");
            androidApp.print(xAccel);
            androidApp.println("<br />");
            androidApp.print("Y-Axis Acceleration: ");
            androidApp.print(yAccel);
            androidApp.println("<br />");
            androidApp.print("Z-Axis Acceleration: ");
            androidApp.print(zAccel);
            androidApp.println("<br />");
            androidApp.println("<br />");
            androidApp.print("Max X-Axis Acceleration ");
            androidApp.print(xMax);
            androidApp.println("<br />");
            androidApp.print("Max Y-Axis Acceleration ");
            androidApp.print(yMax);
            androidApp.println("<br />");
            androidApp.print("Max Z-Axis Acceleration ");
            androidApp.print(zMax);

            break;
            }

          }
          androidApp.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(200);
    // close the connection:
    androidApp.stop();
    Serial.println("client disconnected");
  }
        
   Serial.print("\nx: ");
   Serial.print(xAccel);
   Serial.print("\ty: ");
   Serial.print(yAccel);
   Serial.print("\tz: ");
   Serial.print(zAccel);
   delay(10); //(make it readable)
}

void stdDelay(EthernetClient a)
{
  delay(3);
  a.println("<br />"); 
}  

void warningPage(double minMag, double maxMag, EthernetClient a)
{
  a.print("<html style=background-color:yellow><h1 style=background-color:yellow><a><p style=color:blue;font-size:40px;font-family:Verdana>WARNING ");
  a.print(minMag);
  a.print("-");
  a.print(maxMag);
  a.print(" MAGNITUDE EARTHQUAKE IMMINENT!!</p><audio preload autoplay loop>  <source src=http://www.accesscontrolsales.com/Ingram_Products/mp3/pb628dus-x.mp3 type=audio/mp3 >  </audio> </a></h1></html>");
         
}

void earthquakeState(int x, int y, int z, int lowestAccel, int highestAccel, double minMag, double maxMag, EthernetClient a)
{
  while (((x>=lowestAccel && x<highestAccel) && y<highestAccel && z<highestAccel) || ((y>=lowestAccel && y<highestAccel) && x<highestAccel && z<highestAccel) || ((z>=lowestAccel && z<highestAccel) && y<highestAccel && xAccel<highestAccel))
  {
    warningPage(minMag, maxMag, a);
    stdDelay(a);
    break;
  }
}

