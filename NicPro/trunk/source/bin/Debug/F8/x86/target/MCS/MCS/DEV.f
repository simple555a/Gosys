fblock DEV;		//ƫ����㼰��ֵ���	
uuid{0800-7039-ecda-1ee1};
var
	PV:float;in;	//PV ����
	SP:float;in;	//SP����
	SPV:float;in;	//ƫ�����ֵ
	Y:float;in;	//ƫ�����
	MRJ:bool;out;	//ƫ���
implementation
static err;
void main()
{
	Y = PV - SP;
	err = fabs(PV - SP);
	if(Y > SPV){
		MRJ = 1;
	}else{
		MRJ = 0;
	}
}	