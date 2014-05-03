#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <GLFW/glfw3.h>
#include <winix/time.h>
#include "sgen.h"
GLuint Stx;
void drawRect_(int x,int y,int w,int h,float tx,float ty,float tw,float th){
	glTexCoord2f(tx,ty);
	glVertex2i(x,y);
	glTexCoord2f(tx+tw,ty);
	glVertex2i(x+w,y);
	glTexCoord2f(tx+tw,ty+th);
	glVertex2i(x+w,y+h);
	glTexCoord2f(tx,ty+th);
	glVertex2i(x,y+h);
}
void drawRect(int x,int y,int w,int h,float tx,float ty,float tw,float th){
	glBegin(GL_QUADS);
	drawRect_(x,y,w,h,tx,ty,tw,th);
	glEnd();
}
int main(int argc,char**argv){
	glfwInit();
	GLFWwindow*wnd=glfwCreateWindow(Swid,Shei,"",0,0);
	glfwMakeContextCurrent(wnd);
	glOrtho(0,Swid,Shei,0,1,-1);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1,&Stx);
	glBindTexture(GL_TEXTURE_2D,Stx);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,Swid,Shei,0,GL_RGB,GL_UNSIGNED_BYTE,S);
	for(;;){
		drawRect(0,0,Swid,Shei,0,0,1,1);
		glfwSwapBuffers(wnd);
		endframe(10);
		glfwPollEvents();
		if(glfwGetKey(wnd,GLFW_KEY_ESCAPE)||glfwWindowShouldClose(wnd))return 0;
	}
}