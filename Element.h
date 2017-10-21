#include<vector>

extern int paintcolor[512][512][3];
extern int count;
//extern std::vector<lightmatter> envir;

class node{
	public:
		double x,y,z;
		node();
		node(double x,double y,double z);
		node operator+(node that);
		node operator-(node that);
		node operator/(double that);
		double getlenth();
		void show();
};

class paramt{
	public:
		double r,g,b;
		paramt();
		paramt(double r,double g,double b);
};

class triangle{
	public:
		node n1,n2,n3;
		paramt Kd,Ks,Ka;  //漫反射系数，镜面反射系数，环境光反射系数
		triangle();
		triangle(node a,node b,node c,paramt Kd,paramt Ks,paramt Ka);
		node getfx();    //计算法向量 
		double getarea();   //计算面积 
		node getjd(node R0,node Rd);   //计算交点,直线为P=R0+t*Rd 
		node getfs(node R0,node Rd);    //计算反射线 
};

class lightmatter{    //物体
	public:
		int num;
		std::vector <triangle> c;
		lightmatter();
		void add_one(triangle x,int index);   //在index位置添加一个
		void delete_one(int index); //删除index位置的元素
		triangle get_one(int index);   //得到index位置的元素
};

class screen{            //屏幕
	public:
		int row,col;      //行和列
		node pos; //屏幕左上角的位置
		double d; //每个像素格的宽度
		screen(int row,int col,node pos,double d);
		node get_pos(int row,int col);
};

class gline{         //光线类
	public:
		node R0,Rd;
		paramt intens;  //三种色光分别的强度
		gline(node R0,node R1,paramt intens);   //R0是起点，R1是方向上第二个点
};

class sphere{
	public:
		double r;
		node center;
		sphere(node center,double r);
		node get_jd(node R0,node Rd);
		node get_fx(node R0);
		node get_fs(node R0,node Rd);
};

class source{          //面光源
	public:
		double xl,xh,yl,yh,height;
		source(double x1,double xh,double yl,double yh,double height); //构建函数

};

class camera{           //相机
	public:
		node pos; //相机位置
		camera(node pos);
		void getpicture(screen sc);   //测试用，第二个参数是物体而不是环境
		//void getpicture(screen sc,sphere temp, source mysource);  //球类专用
};

class nodeplus{
	public:
		node mynode;
		int pos;
		nodeplus(node mynode,int pos);
};


double dotans(node X,node Y);        //计算内积 
double detcalc(node X,node Y,node Z);   //计算行列式 

node findcolor(node R0,node Rd,int index);   //返回的是三色的情况

double mymin(double a,double b);

int islightok(node R0,node mysource);  //判断是否与光线相交

node getcolor(paramt Kd, node R);

extern std::vector<lightmatter> envir;
extern source mysource;  //得到面光源位置
