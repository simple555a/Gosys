fblock HL;
uuid{fce2-a7d4-0a64-4572};
var
	PV:float;in;
	SP:float;in;
	SPV:float;in;
	MRJ:bool;out;
implementation
static err;
void main()
{
	err = abs(PV - SP);
	if(err > SPV){
		MRJ = 1;
	}else{
		MRJ = 0;
	}
}	