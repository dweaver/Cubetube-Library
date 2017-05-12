#include "beta-cube-library.h"
#include <math.h>

Cube cube = Cube();

void setup()
{
  cube.begin();
  // Register functions for controlling the cube
  // functions can be max 12 char long
  //             "123456789012"
  Spark.function("setVoxel", setVoxel);
  Spark.function("background", background);
  cube.background(black);  //clear the cube for each frame
}

void loop()
{
  //cube.sphere(4,4,4,3,orange);
  //cube.show();  //send the data to the cube
}

/*******************************************************************************
 * Function Name  : background 
 * Description    : fills the cube with a single color
 * Input          : Point in x,y,z,r,g,b format
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int background(String command)
{
  const char* commandChar = command.c_str();
  char* myCopy = strtok(strdup(commandChar), ",");
  int value[3];
  for (int i = 0; i < 3; i++)
  {
    value[i]= atoi(myCopy);
    myCopy = strtok(NULL, ","); 
  }  
  Color color = Color(value[0], value[1], value[2]);
  cube.background(color);
  cube.show();
  return 1;
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

