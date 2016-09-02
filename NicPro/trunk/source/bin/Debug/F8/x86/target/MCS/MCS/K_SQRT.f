fblock K_SQRT;             //��Ȩƽ����
uuid{d5d1-49ca-3818-a5b1};

var
	IN :float;in;	   //����ֵ
	K :float;in;	   //��Ȩϵ��
	CUTOFF :float;in;  //�ضϵ�
	OUT :float;out;    //������
	ERR :bool;out;	   //����С�����ضϵ�С����

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