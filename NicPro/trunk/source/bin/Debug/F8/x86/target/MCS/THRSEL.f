fblock THRSEL;			//三选一选择器
uuid{757b-d37a-42a0-cae8};
var
	X1 :float;in;		//输入一
	X2 :float;in;		//输入二
	X3 :float;in;		//输入三
	X1_BAD :bool;in;	//输入一通道故障
	X2_BAD :bool;in;	//输入二通道故障
	X3_BAD :bool;in;	//输入三通道故障
	Mode :integer;in;	//方式选择
	DB :float;in;		//偏差限
	Y :float;out;		//输出
	X_BAD : bool;out;	//输出为坏点
	Mode_ERR :bool;out;	//模式输入错误
	
//X_BAD和Mode_ERR都为1说明DB限设置错误
implementation

void main()
{
	if(DB < 0){
		Mode_ERR = 1;
		X_BAD = 1;
		return;
	}
	
	if(X1_BAD && X2_BAD && X3_BAD) {
		Mode_ERR = 0;
		X_BAD = 1;
		return;
	}else{
		Mode_ERR = 0;
		X_BAD = 0;
	}
	
	if(!X1_BAD && X2_BAD && X3_BAD){
		Mode_ERR = 0;
		Y = X1;
	}
	if(X1_BAD && !X2_BAD && X3_BAD){
		Mode_ERR = 0;
		Y = X2;
	}
	if(X1_BAD && X2_BAD && !X3_BAD){
		Mode_ERR = 0;
		Y = X3;
	}

	if(!X1_BAD && !X2_BAD && X3_BAD){
		if(abs(X2 - X1) > DB){
			Mode_ERR = 0;
			X_BAD = 1;
			return;
		}else{
			X_BAD = 0;
			if(Mode == 0){
				Mode_ERR = 0;
				 Y = (X1 + X2)/2.0;
			}else if(Mode == 1){
				Mode_ERR = 0;
				if(X1 <= X2){
					Y = X1;	
				}else{
					Y = X2;	
				}
			}else if(Mode == 2){
				Mode_ERR = 0;
				if(X1 >= X2){
					Y = X1;	
				}else{
					Y = X2;	
				}
			}else{
				Mode_ERR = 1;
			}
		}
	}
	
	if(!X1_BAD && X2_BAD && !X3_BAD){
		if(abs(X3 - X1) > DB){
			Mode_ERR = 0;
			X_BAD = 1;
			return;
		}else{
			X_BAD = 0;
			if(Mode == 0){
				Mode_ERR = 0;
				Y = (X1 + X3)/2.0;
			}else if(Mode == 1){
				Mode_ERR = 0;
				if(X1 <= X3){
					Y = X1;	
				}else{
					Y = X3;	
				}
			}else if(Mode == 2){
				Mode_ERR = 0;
				if(X1 >= X3){
					Y = X1;	
				}else{
					Y = X3;	
				}
			}else{
				Mode_ERR = 1;
			}
		}
	}
	
	if(X1_BAD && !X2_BAD && !X3_BAD){
		if(abs(X3 - X2) > DB){
			Mode_ERR = 0;
			X_BAD = 1;
			return;
		}else{
			X_BAD = 0;
			if(Mode == 0){
				Mode_ERR = 0;
				Y = (X2 + X3)/2.0;
			}else if(Mode == 1){
				Mode_ERR = 0;
				if(X2 <= X3){
					Y = X2;	
				}else{
					Y = X3;	
				}
			}else if(Mode == 2){
				Mode_ERR = 0;
				if(X2 >= X3){
					Y = X2;	
				}else{
					Y = X3;	
				}
			}else{
				Mode_ERR = 1;
			}
		}
	}
	
	if(!X1_BAD && !X2_BAD && !X3_BAD) {
		if((abs(X1 - X2) > DB) && (abs(X3 - X2) > DB) && (abs(X3 - X1) > DB)){
			Mode_ERR = 0;
			X_BAD = 1;
			return;
		}else{
			X_BAD = 0;
		}
		
		if((abs(X1 - X2) <= DB) && (abs(X3 - X2) > DB) && (abs(X3 - X1) > DB)){
			Mode_ERR = 0;
			Y = (X1 + X2)/2.0;
		}
		if((abs(X1 - X2) > DB) && (abs(X3 - X2) > DB) && (abs(X3 - X1) <= DB)){
			Mode_ERR = 0;
			Y = (X1 + X3)/2.0;
		}
		if((abs(X1 - X2) > DB) && (abs(X3 - X2) <= DB) && (abs(X3 - X1) > DB)){
			Mode_ERR = 0;
			Y = (X2 + X3)/2.0;
		}
		
		if((abs(X1 - X2) <= DB) && (abs(X3 - X2) <= DB) && (abs(X3 - X1) > DB)){
			Mode_ERR = 0;
			Y = X2;
		}
		if((abs(X1 - X2) <= DB) && (abs(X3 - X2) > DB) && (abs(X3 - X1) <= DB)){
			Mode_ERR = 0;
			Y = X1;
		}
		if((abs(X1 - X2) > DB) && (abs(X3 - X2) <= DB) && (abs(X3 - X1) <= DB)){
			Mode_ERR = 0;
			Y = X3;
		}
		
		if((abs(X1 - X2) <= DB) && (abs(X3 - X2) <= DB) && (abs(X3 - X1) <= DB)){
			if(Mode == 0){
				Mode_ERR = 0;
				Y = (X1 + X2 + X3)/3.0;
			}else if(Mode == 1){
				Mode_ERR = 0;
				if((X1 <= X3) && (X1 <= X2)){
					Y = X1;	
				}else if((X2 <= X3) && (X2 <= X1)){
					Y = X2;	
				}else if((X3 <= X2) && (X3 <= X1)){
					Y = X3;	
				}
			}else if(Mode == 2){
				Mode_ERR = 0;
				if((X1 >= X3) && (X1 >= X2)){
					Y = X1;	
				}else if((X2 >= X3) && (X2 >= X1)){
					Y = X2;	
				}else if((X3 >= X2) && (X3 >= X1)){
					Y = X3;	
				}
			}else if(Mode == 3){
				Mode_ERR = 0;
				if(((X1 >= X3) && (X1 <= X2)) || ((X1 >= X2) && (X1 <= X3))){
					Y = X1;	
				}else if(((X2 >= X1) && (X2 <= X3)) || ((X2 >= X3) && (X2 <= X1))){
					Y = X2;	
				}else if(((X3 >= X1) && (X3 <= X2)) || ((X3 >= X2) && (X3 <= X1))){
					Y = X3;	
				}
			}else if(Mode == 4){
				Mode_ERR = 0;
				Y = X1;
			}else if(Mode == 5){
				Mode_ERR = 0;
				Y = X2;
			}else if(Mode == 6){
				Mode_ERR = 0;
				Y = X3;
			}else{
				Mode_ERR = 1;
			}		
		}
	}
}