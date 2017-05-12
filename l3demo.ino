#include "beta-cube-library.h"
#include "HttpClient.h"

#include <math.h>

Cube cube = Cube();
unsigned int nextTime = 0;    // Next time to contact the server
unsigned int READ_INTERVAL = 3000;
HttpClient http;

// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    { "X-Exosite-CIK", "b233de12fab2203721d53de63c9a7882a48058da" },
    { "Content-Type", "application/x-www-form-urlencoded; charset=utf-8" },
    { "Accept" , "application/x-www-form-urlencoded; charset=utf-8" },
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

http_request_t request;
http_response_t response;

void setup()
{
    Serial.begin(9600);
	cube.begin();
	// Register functions for controlling the cube
	// functions can be max 12 char long
	//             "123456789012"
	//cube.background(black);  //clear the cube for each frame
	//cube.sphere(4,4,4,3,orange);
	cube.show();  //send the data to the cube
}

void loop()
{
	String voxel;
	if (nextTime > millis()) {
		delay(10);
		return;
	}
    Serial.println();
    Serial.println("Application>\tStart of Loop.");
    // Request path and body can be set at runtime or at setup.
    request.hostname = "r6tgd79vme80vn29.m2.exosite-dev.io";
    request.port = 80;
    request.path = "/onep:v1/stack/alias?voxel";

    // The library also supports sending a body with your request:
    //request.body = "{\"key\":\"value\"}";

    // Get request
    http.get(request, response, headers);
    Serial.print("Application>\tHTTP GET response status: ");
    Serial.println(response.status);

    Serial.print("Application>\tHTTP GET response Body: ");
    Serial.println(response.body);

	if (response.status == 200) {
		// Write a voxel 
	 	
		// 6 -- remove "voxel="
		voxel = urldecode(response.body.substring(6));
		Serial.println("Decoded voxel to:" + voxel);
	    setVoxel(voxel);
	} else {
		// cube.background(red);
	}

	// Write accelerometer data and received voxel value back
    request.path = "/onep:v1/stack/alias";
	// get latest accelerometer values
	cube.updateAccelerometer();
	request.body = String(response.body + "&accelerometer=" + 
		urlencode(
			String(cube.accelerometerX, DEC) + "," + 
			String(cube.accelerometerY, DEC) + "," + 
			String(cube.accelerometerZ, DEC)));
	http.post(request, response, headers);
    Serial.print("Application>\tHTTP POST response status: ");
    Serial.println(response.status);

    nextTime = millis() + READ_INTERVAL;
}


/*******************************************************************************
 * Function Name  : setVoxel 
 * Description    : draws a point in cube space
 * Input          : Point in x,y,z,r,g,b format
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int setVoxel(String command)
{
  const char* commandChar = command.c_str();
  char* myCopy = strtok(strdup(commandChar), ",");
  int value[6];
  for (int i = 0; i < 6; i++)
  {
    value[i]= atoi(myCopy);
    myCopy = strtok(NULL, ","); 
  }  
  Color color = Color(value[3], value[4], value[5]);
  cube.setVoxel(value[0], value[1], value[2], color);
  cube.show();
  return 1;
}


String urldecode(String str)
{
	// https://github.com/zenmanenergy/ESP8266-Arduino-Examples/blob/master/helloWorld_urlencoded/urlencode.ino    
    String encodedString="";
    char c;
    char code0;
    char code1;
    for (int i =0; i < str.length(); i++){
        c=str.charAt(i);
      if (c == '+'){
        encodedString+=' ';  
      }else if (c == '%') {
        i++;
        code0=str.charAt(i);
        i++;
        code1=str.charAt(i);
        c = (h2int(code0) << 4) | h2int(code1);
        encodedString+=c;
      } else{
        
        encodedString+=c;  
      }
      
      yield();
    }
    
   return encodedString;
}

String urlencode(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
    
}

unsigned char h2int(char c)
{
    if (c >= '0' && c <='9'){
        return((unsigned char)c - '0');
    }
    if (c >= 'a' && c <='f'){
        return((unsigned char)c - 'a' + 10);
    }
    if (c >= 'A' && c <='F'){
        return((unsigned char)c - 'A' + 10);
    }
    return(0);
}
