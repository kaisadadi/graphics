#include<iostream>
#include<cstdio>
#include<cstdlib>
#include "Element.h"

using namespace std;

int main(){
	readmtl();
	readobj();
	camera mycamera=camera(node(250,250,-500));  //���λ��
	screen myscreen=screen(100,100,node(500,500,0),5); 
//	mycamera.getpicture(myscreen);
	int TIM=500;
	FILE *fp;
	fp=fopen("ans.ppm","wb");
	fprintf(fp,"P6\n%d %d\n255\n",TIM,TIM);
	for(int i=0;i<TIM;i++){
		for(int j=0;j<TIM;j++){
			static unsigned char color[3];
			color[0]=mymin(paintcolor[i][j][0],255);
			color[1]=mymin(paintcolor[i][j][1],255);
			color[2]=mymin(paintcolor[i][j][1],255);
			fwrite(color,1,3,fp);
		}
	}
	fclose(fp);
	return 0;
}
