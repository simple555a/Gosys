fblock DECIS_D;			//�����ģ��
uuid{05fd-ef4c-b8a3-14dd};
var
	C1V     :float;  in;	//����ͨ��1
	C2V     :float;  in;	//����ͨ��2
	C3V     :float;  in;	//����ͨ��3
	C1Bad   :bool;  in;	//����ͨ��1ͨ������
	C2Bad   :bool;  in;	//����ͨ��2ͨ������
	C3Bad   :bool;  in;	//����ͨ��3ͨ������
	V	:float;  out;	//��������
	CH_Bad  :bool;   out;	//�������ϲ���ͨ������

	
implementation
static float avge,aa,bb,cc;

void main() {
   if (C1Bad && C2Bad && C3Bad) {
   	CH_Bad = 1;
   	V = 0;
   	}else{
   	CH_Bad = 0;
	}	
   if(!C1Bad && C2Bad && C3Bad) {
   	V = C1V;
   	}
   if(C1Bad && !C2Bad && C3Bad) {
   	V = C2V;
   	}
   if(C1Bad && C2Bad && !C3Bad) {
   	V = C3V;
   	}
   if(!C1Bad && !C2Bad && C3Bad) {
   		V = (C1V + C2V)/2.0;
   		}
   if(!C1Bad && C2Bad && !C3Bad) {
   		V = (C1V + C3V)/2.0;
   		}
   if(C1Bad && !C2Bad && !C3Bad) {
   		V = (C2V + C3V)/2.0;
   		}
   if(!C1Bad && !C2Bad && !C3Bad) {
   		avge = (C1V + C2V + C3V)/3.0;
   		aa = fabs(avge - C1V);
   		bb = fabs(avge - C2V);
   		cc = fabs(avge - C3V);
   		if (aa >= bb) {
   			if (aa >= cc) {
   				V = (C2V + C3V)/2.0;
   				}
   			else { 
   				V = (C1V + C2V)/2.0;
   				}
   			}
   		else {
   		 	if(bb >= cc) {
   				V = (C1V + C3V)/2.0;
   				}
   			else {
   				V = (C1V + C2V)/2.0;
   				}
   			}
   		}
}

