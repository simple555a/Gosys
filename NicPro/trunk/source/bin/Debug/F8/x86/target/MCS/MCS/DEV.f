fblock DEV;		//偏差计算及限值输出	
uuid{0800-7039-ecda-1ee1};
var
	PV:float;in;	//PV 输入
	SP:float;in;	//SP输入
	SPV:float;in;	//偏差报警限值
	Y:float;in;	//偏差输出
	MRJ:bool;out;	//偏差报警
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