fblock LIMIT;            //�޷����ģ��
attribute(autoscan);
uuid{147d-9f52-81a2-b8b0};

var
	MIN : float;in;  //�������
	IN : float;in;   //����ֵ
	MAX : float;in;	 //�������
	OUT : float;out; //���ֵ
	ERR : bool;out;  //�����޴���
	
implementation

void main()
{
	if(MIN > MAX){
		ERR = 1;
		OUT = 0;
	}else{	
		ERR = 0;
		if(IN <= MAX){
			if(IN >= MIN){
				OUT = IN;
			}else{
				OUT = MIN;
			}
		}else{
			OUT = MAX;
		}
	}
}