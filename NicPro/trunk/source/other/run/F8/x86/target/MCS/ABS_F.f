fblock ABS_F;			//绝对值模块
uuid{3b72-4380-461c-9228};

var
	IN:float; in;		//输入量，变量或立即数
	OUT:float; out;		//输出量

implementation
void main()
{
	OUT = fabs(IN)；
}