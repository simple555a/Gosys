fblock MULDIV_W;            //�������Ȩ�˳�����
uuid{d5d1-49ca-3818-a5C1};

var
	IN1 :float;in;	   //����ֵ1
	IN2 :float;in;	   //����ֵ2
	IN3 :float;in;     //����ֵ3
	K :float;in;       //��Ȩϵ��
	c1 :float;in;	   //����ϵ��1
	c2 :float;in;      //����ϵ��2
	c3 :float;in;      //����ϵ��3
	c4 :float;in;      //����ϵ��4
	OUT :float;out;    //���ֵ
	ERR :bool;out;     //����Ϊ�㣬������ϵ��3������ֵ3֮��Ϊ��
	
implementation

void main()
{
	if(IN3 == (-1)*c3){
		ERR = 1;
	}else{
		ERR = 0;
		OUT =(K * (IN1 + c1) * (IN2 + c2))/(IN3 + c3) + c4;
	}
}