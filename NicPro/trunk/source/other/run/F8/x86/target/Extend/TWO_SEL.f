fblock TWO_SEL;		//��ѡ��
uuid{fa55-7bc4-0028-459b};
var
	X1 :float;in;		//�����ź�1
	X2 :float;in;		//�����ź�2
	X1_BAD :bool;in;	//�����ź�1��
	X2_BAD :bool;in;	//�����ź�2��
	Mode :integer;in;	//ѡ��ʽ��0=ѡ��ֵ��1=��ѡ��2=��ѡ��
	DB :float;in;		//ƫ����ֵ
	Y :float;out;		//ѡ������
	X_BAD : bool;out;	//�����
	Mode_ERR :bool;out;	//��ʽ����

//ģ��˵����
//1. Ĭ�Ϸ�ʽΪ0��	
//2. ͨ������X1_BAD��X2_BAD����������X1��X2����ֻѡX1��X2����
//3. ��ʼ��ʱ��DB=0������ʹ��ʱ��DB>0;

implementation

void main()
{

//X_BAD��Mode_ERR��Ϊ1��˵��DB��ֵ���ô���
	if(DB < 0){
		Mode_ERR = 1;
		X_BAD = 1;
		return;
	}

//�źž���������ԭ���ֵ	
	if(X1_BAD && X2_BAD ) {
		Mode_ERR = 0;
		X_BAD = 1;
		return;
	}else{
		Mode_ERR = 0;
		X_BAD = 0;
	}

//�ź�2��������ź�1		
	if(!X1_BAD && X2_BAD){
		Mode_ERR = 0;
		Y = X1;
	}
	
//�ź�1��������ź�2		
	if(X1_BAD && !X2_BAD){
		Mode_ERR = 0;
		Y = X2;
	}
	
//�źž�������		
	if(!X1_BAD && !X2_BAD){		  
		if(fabs(X2 - X1) > DB){         //�źž���������ƫ��ޣ�������֣�
			Mode_ERR = 0;
			X_BAD = 1;
			return;
		}else{                         //�źž�������ƫ��δ���ޣ����������ѡ��ʽ��
			X_BAD = 0;
			if(Mode == 0){         //��ʽ0��ѡ��ֵ��
				Mode_ERR = 0;
				 Y = (X1 + X2)/2.0;
			}else if(Mode == 1){   //��ʽ1��ѡ��ֵ��
				Mode_ERR = 0;
				if(X1 <= X2){
					Y = X1;	
				}else{
					Y = X2;	
				}
			}else if(Mode == 2){   //��ʽ2��ѡ��ֵ��
				Mode_ERR = 0;
				if(X1 >= X2){
					Y = X1;	
				}else{
					Y = X2;	
				}
			}else{
				Mode_ERR = 1;  //����Ϊѡ��ʽ����������֣�
			}
		}
	}	
}







	
