fblock TWO_SEL;		//二选器
uuid{fa55-7bc4-0028-459b};
var
	X1 :float;in;		//输入信号1
	X2 :float;in;		//输入信号2
	X1_BAD :bool;in;	//输入信号1坏
	X2_BAD :bool;in;	//输入信号2坏
	Mode :integer;in;	//选择方式：0=选均值，1=低选，2=高选；
	DB :float;in;		//偏差限值
	Y :float;out;		//选择后输出
	X_BAD : bool;out;	//输出坏
	Mode_ERR :bool;out;	//方式错误

//模块说明：
//1. 默认方式为0，	
//2. 通过设置X1_BAD或X2_BAD，可以屏蔽X1或X2（即只选X1或X2）；
//3. 初始化时，DB=0，建议使用时，DB>0;

implementation

void main()
{

//X_BAD和Mode_ERR都为1，说明DB限值设置错误；
	if(DB < 0){
		Mode_ERR = 1;
		X_BAD = 1;
		return;
	}

//信号均坏，保持原输出值	
	if(X1_BAD && X2_BAD ) {
		Mode_ERR = 0;
		X_BAD = 1;
		return;
	}else{
		Mode_ERR = 0;
		X_BAD = 0;
	}

//信号2坏，输出信号1		
	if(!X1_BAD && X2_BAD){
		Mode_ERR = 0;
		Y = X1;
	}
	
//信号1坏，输出信号2		
	if(X1_BAD && !X2_BAD){
		Mode_ERR = 0;
		Y = X2;
	}
	
//信号均正常；		
	if(!X1_BAD && !X2_BAD){		  
		if(fabs(X2 - X1) > DB){         //信号均正常，但偏差超限，输出保持；
			Mode_ERR = 0;
			X_BAD = 1;
			return;
		}else{                         //信号均正常，偏差未超限，输出决定于选择方式；
			X_BAD = 0;
			if(Mode == 0){         //方式0，选均值；
				Mode_ERR = 0;
				 Y = (X1 + X2)/2.0;
			}else if(Mode == 1){   //方式1，选低值；
				Mode_ERR = 0;
				if(X1 <= X2){
					Y = X1;	
				}else{
					Y = X2;	
				}
			}else if(Mode == 2){   //方式2，选高值；
				Mode_ERR = 0;
				if(X1 >= X2){
					Y = X1;	
				}else{
					Y = X2;	
				}
			}else{
				Mode_ERR = 1;  //否则，为选择方式错误，输出保持；
			}
		}
	}	
}







	
