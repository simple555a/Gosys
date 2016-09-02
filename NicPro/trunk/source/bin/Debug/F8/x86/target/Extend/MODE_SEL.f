fblock MODE_SEL;
uuid{b07e-a91e-63d0-24c0};
var
  	Auto:bool;in;		//切自动操作指令
	Prog:bool;in;		//切程控操作指令
	Man:bool;in;		//切手动操作指令
	
	Auto_S:bool;out;	//自动模式
	Prog_S:bool;out;	//程控模式
	Man_S:bool;out;		//手动模式
	
implementation

void main()
{
	//自动模式
	if(Prog || Man){
		Auto_S = 0;
	}else if(Auto && !Prog && !Man){
		Auto_S = 1;
	}
	//程控模式
	if(Auto || Man){
		Prog_S = 0;
	}else if(!Auto && Prog && !Man){
		Prog_S = 1;
	}
	//手动模式
	if(Auto || Prog){
		Man_S = 0;
	}else if(!Auto && !Prog && Man){
		Man_S = 1;
	}

}

