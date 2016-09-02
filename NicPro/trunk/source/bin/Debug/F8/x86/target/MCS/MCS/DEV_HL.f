fblock DEV_HL;
uuid{349c-29ae-7749-9316};
var
	PV:float;in;
	SP:float;in;
	SPV:float;in;
	
	ERR:float;out;
	D :bool;out;
	D1:bool;out;
	D2:bool;out;

implementation
static dev;
void main()
{
	ERR = SP - PV;
	dev = fabs(SP - PV);
	
	if(dev > SPV){
		D = 1;
	}else{
		D = 0;
	}
	
	if(ERR > 0){
		D1 = 1;
		D2 = 0;
	}else if(ERR < 0){
		D1 = 0;
		D2 = 1;
	}else{
		D1 = 0;
		D2 = 0;
	}
}	
