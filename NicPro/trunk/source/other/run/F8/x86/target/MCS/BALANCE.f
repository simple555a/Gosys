fblock BALANCE;      //��·���ƽ��ģ��
uuid{1530-49ca-3818-a5bc};
var
     X:float;in;     //������
     DB:float;in;    //��ֵ����
     TR1:float;in;   //��һ·������
     TS1:bool;in;    //��һ·�����л�����
     TR2:float;in;   //�ڶ�·������
     TS2:bool;in;    //�ڶ�·�����л�����
     YH:float;in;    //Y1��Y2������
     YL:float;in;    //Y1��Y2������   
     	
     Y1:float;out;   //��һ·ƽ��ֵ���
     Y2:float;out;   //�ڶ�·ƽ��ֵ���
     ERR:bool;out;   //�����޴���
implementation

void main()
{	
	if(YH < YL){
		Y1 = 0;
		Y2 = 0;
		ERR = 1;
	}else{
		ERR = 0;
		if((YL < Y1 < YH) && (YL < Y2 < YH)){
			if((TS1 == 1)&&(TS2 == 1)){
				Y1 = TR1;
				Y2 = TR2;
			}
			if((TS1 == 0)&&(TS2 == 0)){
				Y1 = X + DB;
				Y2 = X - DB;
			}
			if((TS1 == 1)&&(TS2 == 0)){
				Y1 = TR1;
				Y2 = 2 * X - Y1;
			}
			if((TS1 == 0)&&(TS2 == 1)){
				Y2 = TR2;
				Y1 = 2 * X - Y2;
			}
		}
		if(Y1 >= YH){
			Y1 = YH;	
		}
		if(Y2 >= YH){
			Y2 = YH;	
		}
		if(Y1 <= YL){
			Y1 = YL;	
		}
		if(Y2 <= YL){
			Y2 = YL;	
		}
	}
}