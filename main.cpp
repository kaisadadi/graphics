#include<iostream>
#include<cstdio>
#include<cstdlib>
#include "Element.h"

using namespace std;

int main(){
	paramt Kd=paramt(0.7,0.7,0.7);   //漫反射
	paramt Ks=paramt(0.3,0.3,0.3);   //镜面反射
	paramt Ka=paramt(0.6,0.6,0.6);  //环境光
	node N1=node(50,50,0);
	node N2=node(150,50,0);
	node N3=node(150,150,0);
	node N4=node(50,150,0);
	node N5=node(50,50,50);
	node N6=node(150,50,50);
	node N7=node(150,150,50);
	node N8=node(50,150,50);
	vector <triangle> temp;
	temp.push_back(triangle(N1,N2,N5,Kd,Ks,Ka));   //左侧面
	temp.push_back(triangle(N5,N6,N2,Kd,Ks,Ka));
	temp.push_back(triangle(N2,N3,N6,Kd,Ks,Ka));   //正面
	temp.push_back(triangle(N6,N7,N3,Kd,Ks,Ka));
	temp.push_back(triangle(N3,N4,N7,Kd,Ks,Ka));  //右侧面
	temp.push_back(triangle(N7,N8,N4,Kd,Ks,Ka));
	temp.push_back(triangle(N5,N6,N7,Kd,Ks,Ka));   //上面
	temp.push_back(triangle(N5,N7,N8,Kd,Ks,Ka));
	temp.push_back(triangle(N1,N4,N5,Kd,Ks,Ka));    //背面
	temp.push_back(triangle(N4,N5,N8,Kd,Ks,Ka));
	lightmatter fangkuai=lightmatter();
	for(int i=0;i<temp.size();i++){        //物体设置
		fangkuai.add_one(temp[i],i);
	}
	envir.push_back(fangkuai);
	node V1=node(0,0,0);
	node V2=node(200,0,0);
	node V3=node(200,200,0);
	node V4=node(0,200,0);
	node V5=node(0,0,200);
	node V6=node(200,0,200);
	node V7=node(200,200,200);
	node V8=node(0,200,200);
	lightmatter zcq=lightmatter();    //红色
	lightmatter ycq=lightmatter();    //绿色
	lightmatter zmq=lightmatter();    //土黄
	lightmatter smq=lightmatter();     //土黄
	lightmatter dmq=lightmatter();      //地面
	zcq.add_one(triangle(V1,V2,V5,paramt(0.6,0,0),paramt(0.3,0,0),paramt(0.5,0,0)),0);
	zcq.add_one(triangle(V2,V5,V6,paramt(0.6,0,0),paramt(0.3,0,0),paramt(0.5,0,0)),1);
	ycq.add_one(triangle(V3,V4,V8,paramt(0,0.6,0),paramt(0,0.6,0),paramt(0,0.5,0)),0);
	ycq.add_one(triangle(V3,V7,V8,paramt(0,0.6,0),paramt(0,0.6,0),paramt(0,0.5,0)),1);
	zmq.add_one(triangle(V1,V4,V5,paramt(0.75,0.75,0.75),paramt(0.3,0.3,0.3),paramt(0.5,0.5,0.5)),0);
	zmq.add_one(triangle(V4,V5,V8,paramt(0.75,0.75,0.75),paramt(0.3,0.3,0.3),paramt(0.5,0.5,0.5)),1);
	smq.add_one(triangle(V5,V6,V8,paramt(0.75,0.75,0.75),paramt(0.3,0.3,0.3),paramt(0.5,0.5,0.5)),0);
	smq.add_one(triangle(V6,V8,V7,paramt(0.75,0.75,0.75),paramt(0.3,0.3,0.3),paramt(0.5,0.5,0.5)),1);
	dmq.add_one(triangle(V1,V2,V3,paramt(0.75,0.75,0.75),paramt(0.3,0.3,0.3),paramt(0.5,0.5,0.5)),0);
	dmq.add_one(triangle(V1,V3,V4,paramt(0.75,0.75,0.75),paramt(0.3,0.3,0.3),paramt(0.5,0.5,0.5)),1);
	envir.push_back(zcq);
	envir.push_back(ycq);
	envir.push_back(zmq);
	envir.push_back(smq);
	envir.push_back(dmq);
	camera mycamera=camera(node(500,100,100));  //相机位置
	screen myscreen=screen(80,80,node(200,0,200),2.5); 
//	source mysource=source(70,130,70,130,200);  //得到面光源位置
	mycamera.getpicture(myscreen);
//	findcolor(node(150,100,100),node(-1,0,-1),0,paramt(1,1,1)).show();
//	printf("%d\n",count);
	int TIM=80;
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
