
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "bevelcube.h"

void drawbevelcube(void)
{
GLfloat bcubevertices[][3] = {{0.475000, -0.500000, -0.475000}, 
			      {0.475000, -0.500000, 0.475000}, 
			      {-0.475000, -0.500000, 0.475000}, 
			      {-0.475000, -0.500000, -0.475000}, 
			      {0.475000, 0.500000, -0.475000}, 
			      {-0.475000, 0.500000, -0.475000}, 
			      {-0.475000, 0.500000, 0.475000}, 
			      {0.475000, 0.500000, 0.475000}, 
			      {0.500000, -0.475000, -0.475000}, 
			      {0.500000, 0.475000, -0.475000}, 
			      {0.499999, 0.475000, 0.475000}, 
			      {0.500000, -0.475000, 0.475000}, 
			      {0.475000, -0.475000, 0.500000}, 
			      {0.475000, 0.475000, 0.500000}, 
			      {-0.475000, 0.475000, 0.500000}, 
			      {-0.475000, -0.475000, 0.500000}, 
			      {-0.500000, -0.475000, 0.475000}, 
			      {-0.500000, 0.475000, 0.475000}, 
			      {-0.500000, 0.475000, -0.475000}, 
			      {-0.500000, -0.475000, -0.475000}, 
			      {0.475000, 0.475000, -0.500000}, 
			      {0.475000, -0.475000, -0.500000}, 
			      {-0.475000, -0.475000, -0.500000}, 
			      {-0.475000, 0.475000, -0.500000}};

GLfloat bcubenormals[][3] = {{-0.000000, -1.000000, 0.000000}, 
			     {0.000000, 1.000000, -0.000000}, 
			     {1.000000, 0.000000, 0.000000}, 
			     {-0.000000, -0.000000, 1.000000}, 
			     {-1.000000, -0.000000, -0.000000}, 
			     {0.000000, 0.000000, -1.000000}, 
			     {0.707107, -0.707107, 0.000000}, 
			     {-0.000000, -0.707107, 0.707107}, 
			     {0.000000, -0.707107, -0.707106}, 
			     {-0.707107, -0.707107, -0.000000}, 
			     {0.000000, 0.707107, -0.707107}, 
			     {-0.707107, 0.707107, -0.000000}, 
			     {0.707107, 0.707107, 0.000000}, 
			     {-0.000000, 0.707107, 0.707106}, 
			     {0.707107, -0.000000, -0.707107}, 
			     {0.707107, 0.000000, 0.707107}, 
			     {-0.707107, 0.000000, 0.707107}, 
			     {-0.707107, 0.000000, -0.707107}, 
			     {0.577350, -0.577351, -0.577350}, 
			     {0.577350, -0.577350, 0.577351}, 
			     {-0.577350, -0.577350, 0.577350}, 
			     {-0.577350, -0.577351, -0.577350}, 
			     {0.577350, 0.577350, -0.577350}, 
			     {0.577350, 0.577351, 0.577350}, 
			     {-0.577350, 0.577350, 0.577350}, 
			     {-0.577350, 0.577350, -0.577351}};



  glBegin(GL_QUADS);
  glNormal3fv(bcubenormals[0]);
  glVertex3fv(bcubevertices[0]);
  glVertex3fv(bcubevertices[1]);
  glVertex3fv(bcubevertices[2]);
  glVertex3fv(bcubevertices[3]);
  glNormal3fv(bcubenormals[1]);
  glVertex3fv(bcubevertices[6]);
  glVertex3fv(bcubevertices[7]);
  glVertex3fv(bcubevertices[4]);
  glVertex3fv(bcubevertices[5]);
  glNormal3fv(bcubenormals[2]);
  glVertex3fv(bcubevertices[10]);
  glVertex3fv(bcubevertices[11]);
  glVertex3fv(bcubevertices[8]);
  glVertex3fv(bcubevertices[9]);
  glNormal3fv(bcubenormals[3]);
  glVertex3fv(bcubevertices[14]);
  glVertex3fv(bcubevertices[15]);
  glVertex3fv(bcubevertices[12]);
  glVertex3fv(bcubevertices[13]);
  glNormal3fv(bcubenormals[4]);
  glVertex3fv(bcubevertices[18]);
  glVertex3fv(bcubevertices[19]);
  glVertex3fv(bcubevertices[16]);
  glVertex3fv(bcubevertices[17]);
  glNormal3fv(bcubenormals[5]);
  glVertex3fv(bcubevertices[22]);
  glVertex3fv(bcubevertices[23]);
  glVertex3fv(bcubevertices[20]);
  glVertex3fv(bcubevertices[21]);
  glNormal3fv(bcubenormals[6]);
  glVertex3fv(bcubevertices[0]);
  glVertex3fv(bcubevertices[8]);
  glVertex3fv(bcubevertices[11]);
  glVertex3fv(bcubevertices[1]);
  glNormal3fv(bcubenormals[7]);
  glVertex3fv(bcubevertices[15]);
  glVertex3fv(bcubevertices[2]);
  glVertex3fv(bcubevertices[1]);
  glVertex3fv(bcubevertices[12]);
  glNormal3fv(bcubenormals[8]);
  glVertex3fv(bcubevertices[21]);
  glVertex3fv(bcubevertices[0]);
  glVertex3fv(bcubevertices[3]);
  glVertex3fv(bcubevertices[22]);
  glNormal3fv(bcubenormals[9]);
  glVertex3fv(bcubevertices[19]);
  glVertex3fv(bcubevertices[3]);
  glVertex3fv(bcubevertices[2]);
  glVertex3fv(bcubevertices[16]);
  glNormal3fv(bcubenormals[10]);
  glVertex3fv(bcubevertices[23]);
  glVertex3fv(bcubevertices[5]);
  glVertex3fv(bcubevertices[4]);
  glVertex3fv(bcubevertices[20]);
  glNormal3fv(bcubenormals[11]);
  glVertex3fv(bcubevertices[5]);
  glVertex3fv(bcubevertices[18]);
  glVertex3fv(bcubevertices[17]);
  glVertex3fv(bcubevertices[6]);
  glNormal3fv(bcubenormals[12]);
  glVertex3fv(bcubevertices[7]);
  glVertex3fv(bcubevertices[10]);
  glVertex3fv(bcubevertices[9]);
  glVertex3fv(bcubevertices[4]);
  glNormal3fv(bcubenormals[13]);
  glVertex3fv(bcubevertices[6]);
  glVertex3fv(bcubevertices[14]);
  glVertex3fv(bcubevertices[13]);
  glVertex3fv(bcubevertices[7]);
  glNormal3fv(bcubenormals[14]);
  glVertex3fv(bcubevertices[20]);
  glVertex3fv(bcubevertices[9]);
  glVertex3fv(bcubevertices[8]);
  glVertex3fv(bcubevertices[21]);
  glNormal3fv(bcubenormals[15]);
  glVertex3fv(bcubevertices[10]);
  glVertex3fv(bcubevertices[13]);
  glVertex3fv(bcubevertices[12]);
  glVertex3fv(bcubevertices[11]);
  glNormal3fv(bcubenormals[16]);
  glVertex3fv(bcubevertices[14]);
  glVertex3fv(bcubevertices[17]);
  glVertex3fv(bcubevertices[16]);
  glVertex3fv(bcubevertices[15]);
  glNormal3fv(bcubenormals[17]);
  glVertex3fv(bcubevertices[18]);
  glVertex3fv(bcubevertices[23]);
  glVertex3fv(bcubevertices[22]);
  glVertex3fv(bcubevertices[19]);
  glEnd();

  glBegin(GL_TRIANGLES);
  glNormal3fv(bcubenormals[18]);
  glVertex3fv(bcubevertices[0]);
  glVertex3fv(bcubevertices[21]);
  glVertex3fv(bcubevertices[8]);
  glNormal3fv(bcubenormals[19]);
  glVertex3fv(bcubevertices[1]);
  glVertex3fv(bcubevertices[11]);
  glVertex3fv(bcubevertices[12]);
  glNormal3fv(bcubenormals[20]);
  glVertex3fv(bcubevertices[2]);
  glVertex3fv(bcubevertices[15]);
  glVertex3fv(bcubevertices[16]);
  glNormal3fv(bcubenormals[21]);
  glVertex3fv(bcubevertices[3]);
  glVertex3fv(bcubevertices[19]);
  glVertex3fv(bcubevertices[22]);
  glNormal3fv(bcubenormals[22]);
  glVertex3fv(bcubevertices[4]);
  glVertex3fv(bcubevertices[9]);
  glVertex3fv(bcubevertices[20]);
  glNormal3fv(bcubenormals[23]);
  glVertex3fv(bcubevertices[7]);
  glVertex3fv(bcubevertices[13]);
  glVertex3fv(bcubevertices[10]);
  glNormal3fv(bcubenormals[24]);
  glVertex3fv(bcubevertices[6]);
  glVertex3fv(bcubevertices[17]);
  glVertex3fv(bcubevertices[14]);
  glNormal3fv(bcubenormals[25]);
  glVertex3fv(bcubevertices[5]);
  glVertex3fv(bcubevertices[23]);
  glVertex3fv(bcubevertices[18]);
  glEnd();
}

