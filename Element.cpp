#include "Element.h"
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<random>
#include<ctime>
#include<vector>

int paintcolor[512][512][3];
std::vector <lightmatter> envir;
source mysource=source(80,120,80,120,200);  //得到面光源位置
int count=0;

double mymin(double a,double b){
	if(a>b) return b;
	return a;
}

double node::getlenth(){
	return sqrt(this->x*this->x+this->y*this->y+this->z*this->z);
}

node::node(){
	this->x=0;
	this->y=0;
	this->z=0;
}

node node::operator+(node that){
	node i;
	i.x=this->x+that.x;
	i.y=this->y+that.y;
	i.z=this->z+that.z;
	return i;
}

node node::operator-(node that){
	node i;
	i.x=this->x-that.x;
	i.y=this->y-that.y;
	i.z=this->z-that.z;
	return i;
}

node node::operator/(double that){
	node i;
	i.x=this->x/that;
	i.y=this->y/that;
	i.z=this->z/that;
	return i;
}

void node::show(){
	printf("%lf %lf %lf\n",this->x,this->y,this->z);
}


node::node(double x,double y,double z){
	this->x=x;
	this->y=y;
	this->z=z;
}

paramt::paramt(){
	this->r=0;
	this->g=0;
	this->b=0;
}

paramt::paramt(double r,double g,double b){
	this->r=r;
	this->g=g;
	this->b=b;
}

triangle::triangle(){
	this->n1=node(0,0,0);
	this->n2=node(0,0,0);
	this->n3=node(0,0,0);
	this->Ka=paramt(0,0,0);
	this->Kd=paramt(0,0,0);
	this->Ka=paramt(0,0,0);
}

triangle::triangle(node a,node b,node c,paramt Kd,paramt Ks,paramt Ka){
	this->n1=a;
	this->n2=b;
	this->n3=c;
	this->Kd=Kd;
	this->Ks=Ks;
	this->Ka=Ka;
}

node triangle::getfx(){
	node x1=this->n1-this->n2;
	node x2=this->n1-this->n3;
	node fx=node(x1.y*x2.z-x1.z*x2.y,x1.z*x2.x-x1.x*x2.z,x1.x*x2.y-x1.y*x2.x);
	return fx;
}

double triangle::getarea(){
	node x1=this->n1;
	node x2=this->n2;
	node x3=this->n3;
	node x4=x2-x1;
	node x5=x3-x1;
	node x6=x3-x2;
	double a=x4.getlenth();
	double b=x5.getlenth();
	double c=x6.getlenth();
	double p=(a+b+c)/2;
	return sqrt(p*(p-a)*(p-b)*(p-c));
}

node triangle::getjd(node R0,node Rd){
	node E1=this->n1-this->n2;
	node E2=this->n1-this->n3;
	node S=this->n1-R0;
	double t=detcalc(S,E1,E2)/detcalc(Rd,E1,E2);
	double beta=detcalc(Rd,S,E2)/detcalc(Rd,E1,E2);
	double gamma=detcalc(Rd,E1,S)/detcalc(Rd,E1,E2);
	node E;
	if(t>0 && beta>=0 && gamma>=0 && beta<=1 && gamma<=1 && beta+gamma<=1){
		E=node(R0.x+t*Rd.x,R0.y+t*Rd.y,R0.z+t*Rd.z);
	}
	else{
		E=node(-1,-1,-1);
	}
	return E;
}

node triangle::getfs(node R0,node Rd){
	node fx=this->getfx();
	if(dotans(fx,Rd)>0){
		fx=node(0,0,0)-fx;
	}
	Rd=Rd/Rd.getlenth();
	fx=fx/fx.getlenth();
	double cost=-dotans(fx,Rd)/(fx.getlenth()*Rd.getlenth());
	node Rf=node(2*cost*fx.x+Rd.x,2*cost*fx.y+Rd.y,2*cost*fx.z+Rd.z);
	return Rf;	
}

double dotans(node X,node Y){        //计算内积 
	return X.x*Y.x+X.y*Y.y+X.z*Y.z;
}
double detcalc(node X,node Y,node Z){   //计算行列式 
	return X.x*Y.y*Z.z+Y.x*Z.y*X.z+Z.x*X.y*Y.z-X.z*Y.y*Z.x-X.x*Z.y*Y.z-Y.x*X.y*Z.z;
}

lightmatter::lightmatter(){
	this->c.clear();
	this->num=0;
}

void lightmatter::add_one(triangle x,int index){    //添加元素,index从0开始
	if(this->num >= index){
		this->c.insert(this->c.begin()+index,x);
		this->num++;
	}
	else{
		printf("out of boundary!\n");
	}
}

void lightmatter::delete_one(int index){             //删除元素
	if(this->num>index){
		this->c.erase(this->c.begin()+index);
		this->num--;
	}
	else{
		printf("out of boundary!\n");
	}
}

triangle lightmatter::get_one(int index){    //取出元素
	return this->c[index];
}

sphere::sphere(node center, double r){       //球体定义
	this->center=center;
	this->r=r;
}

node sphere::get_jd(node R0,node Rd){          //求与球体交点
	double b=dotans(Rd,R0-(this->center));
	double c=dotans(R0-(this->center),R0-(this->center))-(this->r)*(this->r);
	double t1,t2;
	if(b*b<c){
		t1=-1;
		t2=-1;
	}
	else{
		t1=-b+sqrt(b*b-c);
		t2=-b-sqrt(b*b-c);
	}
//	printf("%lf %lf %lf %lf\n",t1,t2,b,c);
	node jd;
	if(t2>0){
		jd=node(R0.x+t2*Rd.x,R0.y+t2*Rd.y,R0.z+t2*Rd.z);
	}
	else{
		jd=node(-1,-1,-1);
	}
	return jd;
}

node sphere::get_fx(node R0){        //求法向
	node fx=R0-this->center;
	return fx;
}

node sphere::get_fs(node R0,node Rd){
	node fx=this->get_fx(this->get_jd(R0,Rd/Rd.getlenth()));
	if(dotans(fx,Rd)>0){
		fx=node(0,0,0)-fx;
	}
	Rd=Rd/Rd.getlenth();
	fx=fx/fx.getlenth();
	double cost=-dotans(fx,Rd)/(fx.getlenth()*Rd.getlenth());
	node Rf=node(2*cost*fx.x+Rd.x,2*cost*fx.y+Rd.y,2*cost*fx.z+Rd.z);
	return Rf;	
}

gline::gline(node R0,node R1,paramt intens){
	this->R0=R0;
	this->Rd=R1-R0;
	this->intens=intens;
}

camera::camera(node pos){
	this->pos=pos;
}

screen::screen(int row,int col,node pos,double d){
	this->row=row;
	this->col=col;
	this->pos=pos;
	this->d=d;
}

node screen::get_pos(int row,int col){
	return this->pos+node(0,(2*col-1)*this->d/2,-(2*row-1)*this->d/2);  //假设屏幕与yz平面平行
}

source::source(double xl,double xh,double yl,double yh,double height){
	this->xl=xl;
	this->xh=xh;
	this->yl=yl;
	this->yh=yh;
	this->height=height;
}

void camera::getpicture(screen sc){             
	srand((int)time(0));
	for(int i=1;i<=sc.row;i++){
		for(int j=1;j<=sc.col;j++){
			printf("%d %d\n",i,j);
			node mypos=sc.get_pos(i,j);
			node mycolor=findcolor(this->pos,mypos-(this->pos),0);
			paintcolor[i-1][j-1][0]=mymin(mycolor.x,255);
			paintcolor[i-1][j-1][1]=mymin(mycolor.y,255);
			paintcolor[i-1][j-1][2]=mymin(mycolor.z,255);
		}

	}
}

nodeplus::nodeplus(node mynode,int pos){
	this->mynode=mynode;
	this->pos=pos;
}



int islightok(node R0,node mysource){    //判断前往光源有无遮挡
	node Rd=mysource-R0;
	double t=(mysource.z-R0.z)/Rd.z;
	for(int i=0;i<envir.size();i++){
		for(int j=0;j<envir[i].num;j++){
			//printf("%d %d\n",i,j);
			triangle temp=envir[i].get_one(j);
			node crosspoint=temp.getjd(R0,Rd);   //求交点
			if(crosspoint.x<0){   //无交点就换
				continue;
			}
			double t2=(crosspoint.z-R0.z)/Rd.z;
			if(t2<t){
				return 0;
			}
		}
	}
	printf("veryhappy!\n");
	return 1;
}


node getcolor(paramt Kd, node R){
	node newcolor=node(0,0,0);
	newcolor.x+=Kd.r*R.x;
	newcolor.y+=Kd.g*R.y;
	newcolor.z+=Kd.b*R.z;
	return newcolor;
}

node findcolor(node R0,node Rd,int index){   
	double mymin=1000000000;
	node R0next=node(-1,-1,-1);
	node R1next=node(-1,-1,-1);
	node fx=node(-1,-1,-1),fx2,fx3;
	paramt Ka;   //反射折射系数
	paramt Kd;
	triangle temp;
	int ishave=0; 
	for(int i=0;i<envir.size();i++){
		for(int j=0;j<envir[i].num;j++){
			temp=envir[i].get_one(j);
			node crosspoint=temp.getjd(R0,Rd);   //求交点
			if(crosspoint.x<0){   //无交点就换
				continue;
			}
			double dis=(crosspoint-R0).getlenth();
			if(dis<mymin){
				ishave=0;
				fx=temp.getfx();
				fx=fx/fx.getlenth();
				mymin=dis;
				if(dotans(fx,Rd)>0){
					fx=node(0,0,0)-fx;   //得到法向
				}
				R0next=crosspoint;
				Ka=temp.Ka;
				Kd=temp.Kd;
			}
			else if(dis==mymin){
				ishave++;
				if(ishave==1){
					fx2=temp.getfx();
					fx2=fx2/fx2.getlenth();
					if(dotans(fx2,Rd)>0){
						fx2=node(0,0,0)-fx2;   //得到法向
					}
					Ka=paramt((Ka.r+temp.Ka.r)/2,(Ka.g+temp.Ka.g)/2,(Ka.b+temp.Ka.b)/2);
					Kd=paramt((Kd.r+temp.Kd.r)/2,(Kd.g+temp.Kd.g)/2,(Kd.b+temp.Kd.b)/2);
				}	
				else if(ishave==2){
					fx3=temp.getfx();
					fx3=fx3/fx3.getlenth();
					if(dotans(fx3,Rd)>0){
						fx3=node(0,0,0)-fx3;   //得到法向
					}
					Ka=paramt((Ka.r*2+temp.Ka.r)/3,(Ka.g*2+temp.Ka.g)/3,(Ka.b*2+temp.Ka.b)/3);
					Kd=paramt((Kd.r*2+temp.Kd.r)/3,(Kd.g*2+temp.Kd.g)/3,(Kd.b*2+temp.Kd.b)/3);
				}			
				
			}		
		}
	}
	if(mymin==1000000000){
		return node(0,0,0);
	}
	else{
		if(R0next.x>=mysource.xl && R0next.x<=mysource.xh && R0next.y>=mysource.yl && R0next.y<=mysource.yh && abs(R0next.z-mysource.height)<1){
			return node(18.9914*255,14.4266*255,3.8927*255);
		}
		if(index==2){
			return getcolor(Ka,node(0.5*255,0.5*255,0.5*255));
		}
		node color=node(0,0,0);
		node fxnew; 
		fx=fx/fx.getlenth();
		R0next=R0next+fx/1000;
		double N=600;
		int m;
		int jishu=1;
        double sum=0;
        for(int i=1;i<30;i++){
            sum+=cos(i*3.0/180.0*3.1416);
        }
		for(int i=1;i<30;i++){
			m=(int)(N*cos(i*3.0/180.0*3.1416)/sum+0.5);
			for(int j=1;j<=m;j++){
				if(fx.x!=0){
					fxnew=node(fx.x/abs(fx.x)*sin(i*3.0/180.0*3.1416),cos(i*3.0/180.0*3.1416)*cos(2.0/m*j*3.1416),cos(i*3.0/180.0*3.1416)*sin(2.0/m*j*3.1416));
				}
				else if(fx.y!=0){
					fxnew=node(cos(i*3.0/180.0*3.1416)*cos(2.0/m*j*3.1416),fx.y/abs(fx.y)*sin(i*3.0/180.0*3.1416),cos(i*3.0/180.0*3.1416)*sin(2.0/m*j*3.1416));
				}
				else if(fx.z!=0){
					fxnew=node(cos(i*3.0/180.0*3.1416)*cos(2.0/m*j*3.1416),cos(i*3.0/180.0*3.1416)*sin(2.0/m*j*3.1416),fx.z/abs(fx.z)*sin(i*3.0/180.0*3.1416));
				}
				if(ishave==0){
					color=color+getcolor(Kd,findcolor(R0next,fxnew,index+1));
					jishu++;
				}
				else if(ishave==1){
					if(dotans(fxnew,fx2)>=0){
						color=color+getcolor(Kd,findcolor(R0next,fxnew,index+1));
						jishu++;						
					}
				}
				else if(ishave==2){
					if(dotans(fxnew,fx2)>=0 && dotans(fxnew,fx3)>=0){
						color=color+getcolor(Kd,findcolor(R0next,fxnew,index+1));
						jishu++;						
					}					
				}
				
			}
		}
		color=color+getcolor(Kd,findcolor(R0next,fx,index+1));
		color=color/jishu;
        color=color+getcolor(Ka,node(0.5*255,0.5*255,0.5*255));
		return color;
	}
}


