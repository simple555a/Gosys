fblock DBZ;            //死区限制块
uuid{1e75-6d42-ad9f-f987};

var
	X : float;in;     //值输入
	DB :float;in;	  //死区设置
	Y :float;out; 	  //值输出
        X_D:float;out;
implementation

void main()
{
	static bool first_time=true;
	float m;
	
	if(first_time)
           {X_D=X;
           first_time = false;
           return;}
        if(X-X_D>0)
          {m=X-X_D;}
        else
          {m=X_D-X;}  
        if(m>DB)
         {X_D=X;}
         else return;
        Y=X_D;
}