fblock DBZ;            //�������ƿ�
uuid{1e75-6d42-ad9f-f987};

var
	X : float;in;     //ֵ����
	DB :float;in;	  //��������
	Y :float;out; 	  //ֵ���
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