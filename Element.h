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
		paramt Kd,Ks,Ka;  //������ϵ�������淴��ϵ���������ⷴ��ϵ��
		triangle();
		triangle(node a,node b,node c,paramt Kd,paramt Ks,paramt Ka);
		node getfx();    //���㷨���� 
		double getarea();   //������� 
		node getjd(node R0,node Rd);   //���㽻��,ֱ��ΪP=R0+t*Rd 
		node getfs(node R0,node Rd);    //���㷴���� 
};

class lightmatter{    //����
	public:
		int num;
		std::vector <triangle> c;
		lightmatter();
		void add_one(triangle x,int index);   //��indexλ�����һ��
		void delete_one(int index); //ɾ��indexλ�õ�Ԫ��
		triangle get_one(int index);   //�õ�indexλ�õ�Ԫ��
};

class screen{            //��Ļ
	public:
		int row,col;      //�к���
		node pos; //��Ļ���Ͻǵ�λ��
		double d; //ÿ�����ظ�Ŀ��
		screen(int row,int col,node pos,double d);
		node get_pos(int row,int col);
};

class gline{         //������
	public:
		node R0,Rd;
		paramt intens;  //����ɫ��ֱ��ǿ��
		gline(node R0,node R1,paramt intens);   //R0����㣬R1�Ƿ����ϵڶ�����
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

class source{          //���Դ
	public:
		double xl,xh,yl,yh,height;
		source(double x1,double xh,double yl,double yh,double height); //��������

};

class camera{           //���
	public:
		node pos; //���λ��
		camera(node pos);
		void getpicture(screen sc);   //�����ã��ڶ�����������������ǻ���
		//void getpicture(screen sc,sphere temp, source mysource);  //����ר��
};

class nodeplus{
	public:
		node mynode;
		int pos;
		nodeplus(node mynode,int pos);
};


double dotans(node X,node Y);        //�����ڻ� 
double detcalc(node X,node Y,node Z);   //��������ʽ 

node findcolor(node R0,node Rd,int index);   //���ص�����ɫ�����

double mymin(double a,double b);

int islightok(node R0,node mysource);  //�ж��Ƿ�������ཻ

node getcolor(paramt Kd, node R);

extern std::vector<lightmatter> envir;
extern source mysource;  //�õ����Դλ��
