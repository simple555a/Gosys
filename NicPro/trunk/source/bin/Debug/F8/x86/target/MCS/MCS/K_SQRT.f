fblock K_SQRT;             //加权平方根
uuid{d5d1-49ca-3818-a5b1};

var
	IN :float;in;	   //输入值
	K :float;in;	   //加权系数
	CUTOFF :float;in;  //截断点
	OUT :float;out;    //计算结果
	ERR :bool;out;	   //输入小于零或截断点小于零

implementation

void main()
{
	if((IN < 0) || (CUTOFF < 0)){
		OUT = 0.0;
		ERR = 1;
	}else{
		ERR = 0;
		if(IN < CUTOFF){
			OUT = 0.0;
		}else{
			OUT = K * sqrt(IN);
		}
	}
}