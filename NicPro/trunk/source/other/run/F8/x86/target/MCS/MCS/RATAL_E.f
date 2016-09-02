fblock RATAL_E;            //模拟量变化率超速报警块
uuid{8316-d360-2bb9-c84a};

comments
    version="1.0";
    author="Ysl";
    
#include <f8_kprocs.h>
var
	X  : float;in;     //速率输入
	PL :float;in;	  //速率限值
	TL :float;in;	  //指定时间，必须小于5，单位秒
	D  :bool;out; 	  //越速率限指示
    TL_Err :bool;out;       //指定时间错误
	
implementation
static float t[128];
static float d[128];
static double s;
static integer a,i,j;
static date D1,D2;
static float D3;

void main()
{       
	static bool first_time = true;
	
		   
	if(first_time)
	{i=0;
	 first_time = false;
	 D2=gettime();
	 	}
	if(TL>5)
	   {TL_Err=1;
	    return;}
	else
	{
	TL_Err=0;		
	D1=gettime();
	D3=(float)(D1-D2);
	D2=D1;
	t[i]=D3;
	d[i]=X;
	s=0;
	for(a=0;a<=i;a++)
	 {s=s+t[a];
	 }
	if(s>=TL)
	{if(fabs(d[i]-d[0])>=PL)
		{D=1;}
	else
		{D=0;}
	for(j=0;j<i;j++)
		{t[j]=t[j+1];
		d[j]=d[j+1];
		}
	}
	else
	i++;
	}
}

