#include "Element.h"
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<vector>
#include<cstring>

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
}

triangle::triangle(node a,node b,node c){
	this->n1=a;
	this->n2=b;
	this->n3=c;
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
//	printf("%lf %lf %lf\n",t,beta,gamma);
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
	//printf("%lf %lf %lf %lf\n",t1,t2,b,c);
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
	return this->pos+node(-(2*col-1)*this->d/2,-(2*row-1)*this->d/2,0);  //假设屏幕与yz平面平行
}

source::source(double xl,double xh,double yl,double yh,double height){
	this->xl=xl;
	this->xh=xh;
	this->yl=yl;
	this->yh=yh;
	this->height=height;
}

void camera::getpicture(screen sc){             
	for(int i=1;i<=sc.row;i++){
		for(int j=1;j<=sc.col;j++){
			printf("%d %d\n",i,j);
			node mypos=sc.get_pos(i,j);
			//mypos.show();
			//node mycolor=findcolor(mypos,mypos-(this->pos));
			(mypos-(this->pos)).show();
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
	if(t<=0){
		return 0;
	}
	for(int i=0;i<envir.size();i++){
		for(int j=0;j<envir[i].num;j++){
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
	return 1;
}

node myrotate(node a,node p,double theta){
	theta=(theta/180.0)*3.1416;
	node fxsecond;
	fxsecond.x=p.x*cos(theta)+(a.y*p.z-a.z*p.y)*sin(theta)+a.x*(a.x*p.x+a.y*p.y+a.z*p.z)*(1-cos(theta));
	fxsecond.y=p.y*cos(theta)+(a.z*p.x-a.x*p.z)*sin(theta)+a.y*(a.x*p.x+a.y*p.y+a.z*p.z)*(1-cos(theta));
	fxsecond.z=p.z*cos(theta)+(a.x*p.y-a.y*p.x)*sin(theta)+a.z*(a.x*p.x+a.y*p.y+a.z*p.z)*(1-cos(theta));
	return fxsecond;
}


node getcolor(paramt Kd, node R){
	node newcolor=node(0,0,0);
	newcolor.x+=Kd.r*R.x;
	newcolor.y+=Kd.g*R.y;
	newcolor.z+=Kd.b*R.z;
	return newcolor;
}
		
node findcolor(node R0,node Rd,int index){   //应该乘以当前面的Ks，修改！
	/*printf("index= %d\n",index);
	R0.show();
	Rd.show();*/
	double mymin=1000000000;
	node R0next=node(-1,-1,-1);
	node R1next=node(-1,-1,-1);
	node fx=node(-1,-1,-1);
	paramt Ka;   //反射折射系数
	paramt Kd;
	triangle temp;
	int ishave=0;
	char name[20];
	for(int i=0;i<envir.size();i++){
		//printf("name= %s\n",envir[i].name);
		for(int j=0;j<envir[i].num;j++){
			//printf("%d %d\n",i,j);
			temp=envir[i].get_one(j);
			node crosspoint=temp.getjd(R0,Rd);   //求交点
			if(crosspoint.x<0){   //无交点就换
				continue;
			}
			double dis=(crosspoint-R0).getlenth();
			if(dis<mymin){
				strcpy(name,envir[i].name);
				fx=temp.getfx();
				mymin=dis;
				if(dotans(fx,Rd)>0){
					fx=node(0,0,0)-fx;   //得到法向
				}
				R0next=crosspoint;
				Ka=envir[i].Ka;
				Kd=envir[i].Kd;
			}
		}
	}
	if(mymin==1000000000){
		return node(0,0,0);
	}
	else{
		if(strcmp(name,"#light")==0){
			return node(18.9914*255,14.4266*255,3.8927*255);
		}
		if(index==2){
			return getcolor(Ka,node(0.5*255,0.5*255,0.5*255));
		}
		node color=node(0,0,0);
		node l1,l2;   //垂直第一第二分量l1,l2
		fx=fx/fx.getlenth();
		R0next=R0next+fx/1000;
		double N=600;
		int m;
		int jishu=1;
		double sum=0;
		for(int i=1;i<30;i++){
			sum+=cos(i*3.0/180.0*3.1416);
		}
		node fxnew;
		for(int i=1;i<=30;i++){
			m=(int)(N*cos(i*3.0/180.0*3.1416)/sum+0.5);
			jishu+=m;
			if((temp.n1-R0next).getlenth()<1){
				fxnew=fx/(1.0/sin(i*3.0/180.0*3.1416))+((temp.n2-R0next)/(temp.n2-R0next).getlenth())/(1.0/cos(i*3.0/180.0*3.1416));
			}
			else{
				fxnew=fx/(1.0/sin(i*3.0/180.0*3.1416))+((temp.n1-R0next)/(temp.n1-R0next).getlenth())/(1.0/cos(i*3.0/180.0*3.1416));
			}
			for(int j=1;j<=m;j++){
				fxnew=myrotate(fx,fxnew,(double)360/(double)m);
				color=color+getcolor(Kd,findcolor(R0next,fxnew,index+1));
			}
		}
		color=color+getcolor(Kd,findcolor(R0next,fx,index+1));
		color=color/jishu;
		color=color+getcolor(Ka,node(0.5*255,0.5*255,0.5*255));
		return color;
	}
}

void readmtl(){
	FILE *fpListFile=fopen("img.mtl","r"); 
	char mys[20];
	int mycount=0;
	int nownum=0;
	int myflag=0;   //1代表Ka,2代表Kd,3代表Ks
	while(fscanf(fpListFile,"%s",mys)!=EOF){    //读入材料表面信息 
		if(mys[0]=='#'){
			lightmatter temp=lightmatter();
			strcpy(temp.name,mys);
			envir.push_back(temp);
		}
		else if(mys[0]=='K'){
			if(mys[1]=='a') myflag=1;
			if(mys[1]=='d') myflag=2;
			if(mys[1]=='s') myflag=3;
			mycount=0;
		} 
		else if(myflag!=0){
			mycount++;
			if(mycount==3){
				mycount=0;
			}
			int pos=-1;
			double myans=0;
			int mylen=strlen(mys);
			for(int i=0;i<mylen;i++){
				if(mys[i]=='.'){
					pos=i;
				}
			}
			if(pos==-1){
				pos=mylen-1;
			}
			for(int i=mylen-1;i>=0;i--){
				if(mys[i]=='.'){
					pos=pos-1;
					continue;
				} 
				myans+=pow(10,pos-i)*(mys[i]-'0');
			}
			if(myflag==1){
				if(mycount==0){
					envir[nownum].Ka.b=myans;
				}
				else if(mycount==1){
					envir[nownum].Ka.r=myans;
				}
				else if(mycount==2){
					envir[nownum].Ka.g=myans;
				}
			}
			else if(myflag==2){
				if(mycount==0){
					envir[nownum].Kd.b=myans;
				}
				else if(mycount==1){
					envir[nownum].Kd.r=myans;
				}
				else if(mycount==2){
					envir[nownum].Kd.g=myans;
				}
			}
			else if(myflag==3){
				if(mycount==0){
					envir[nownum].Ks.b=myans;
					nownum++;
					myflag=0;
				}
				else if(mycount==1){
					envir[nownum].Ks.r=myans;
				}
				else if(mycount==2){
					envir[nownum].Ks.g=myans;
				}
			}		
		}
	} 
}

void readobj(){
	char mys[20];
	std::vector <node> myvec;
	FILE *fpListFile=fopen("img.obj","r"); 
	lightmatter temp;
	int vflag=0,fflag=0,f1,f2,f3,index,ii;
	double v1,v2,v3;
	while(fscanf(fpListFile,"%s",mys)!=EOF){    //读入node及triangle信息，需要建立一个node的vector来预存储 
		//printf("%s\n",mys);
		if(mys[0]=='#'){
			for(ii=0;ii<envir.size();ii++){
				if(strcmp(envir[ii].name,mys)==0){
					temp=envir[ii];
					index=0;
					break;
				}
			}
		}
		else if(mys[0]=='v'){
			vflag=1;
		}
		else if(mys[0]=='f'){
			fflag=1;
		}
		else if(mys[0]-'0'>=0 && mys[0]-'9'<=0){
			int pos=-1;
			double myans=0;
			int mylen=strlen(mys);
			for(int i=0;i<mylen;i++){
				if(mys[i]=='.'){
					pos=i;
				}
			}
			if(pos==-1){
				pos=mylen-1;
			}
			for(int i=mylen-1;i>=0;i--){
				if(mys[i]=='.'){
					pos=pos-1;
					continue;
				} 
				myans+=pow(10,pos-i)*(mys[i]-'0');
			}
			if(vflag!=0){
				if(vflag==1){
					v1=myans;
					vflag++;
				}
				else if(vflag==2){
					v2=myans;
					vflag++;
				}
				else if(vflag==3){
					v3=myans;
					vflag=0;
					myvec.push_back(node(v1,v2,v3));
				}
			}
			else if(fflag!=0){
				if(fflag==1){
					f1=myans;
					fflag++;
				}
				else if(fflag==2){
					f2=myans;
					fflag++;
				}
				else if(fflag==3){
					f3=myans;
					fflag=0;
					//printf("addone %d\n",index);
					envir[ii].add_one(triangle(myvec[f1-1],myvec[f2-1],myvec[f3-1]),index);
					index++;
				}
			}
		}
	}
}





