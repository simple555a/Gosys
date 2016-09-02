fblock THRSEL;			//��ѡһѡ����
uuid{757b-d37a-42a0-cae8};
var
	X1 :float;in;		//����һ
	X2 :float;in;		//�����
	X3 :float;in;		//������
	X1_BAD :bool;in;	//����һͨ������
	X2_BAD :bool;in;	//�����ͨ������
	X3_BAD :bool;in;	//������ͨ������
	Mode :integer;in;	//��ʽѡ��
	DB :float;in;		//ƫ����
	Y :float;out;		//���
	X_BAD : bool;out;	//���Ϊ����
	Mode_ERR :bool;out;	//ģʽ�������
	
//X_BAD��Mode_ERR��Ϊ1˵��DB�����ô���
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