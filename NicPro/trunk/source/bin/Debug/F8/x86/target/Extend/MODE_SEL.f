fblock MODE_SEL;
uuid{b07e-a91e-63d0-24c0};
var
  	Auto:bool;in;		//���Զ�����ָ��
	Prog:bool;in;		//�г̿ز���ָ��
	Man:bool;in;		//���ֶ�����ָ��
	
	Auto_S:bool;out;	//�Զ�ģʽ
	Prog_S:bool;out;	//�̿�ģʽ
	Man_S:bool;out;		//�ֶ�ģʽ
	
implementation

void main()
{
	//�Զ�ģʽ
	if(Prog || Man){
		Auto_S = 0;
	}else if(Auto && !Prog && !Man){
		Auto_S = 1;
	}
	//�̿�ģʽ
	if(Auto || Man){
		Prog_S = 0;
	}else if(!Auto && Prog && !Man){
		Prog_S = 1;
	}
	//�ֶ�ģʽ
	if(Auto || Prog){
		Man_S = 0;
	}else if(!Auto && !Prog && Man){
		Man_S = 1;
	}

}

