fblock SCALING_X; //�����������̱任ģ��
uuid{51d9-993e-5f16-6666};
var	
IN:float; in;
	Min:float;in;
	Max:float;in;
	Kc:float;in;	//��������
	Kt:float;in;	//С�ź��г��ȣ�%��
	OUT:float; out;
implementation
void main()
{
	OUT = IN * (Max - Min) + Min + Kc;
	if(OUT < (Min + (Max - Min) * Kt)){
		OUT = Min;
	}
	if(OUT > Max){
		OUT = Max;
	}
}