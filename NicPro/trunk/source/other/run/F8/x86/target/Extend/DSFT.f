fblock DSFT;
uuid{34e8-e701-7022-1e08};
var
	Z1: bool;in;
	Z2: bool;in;
	Z: bool;in;
	D: bool;out;
implementation

void main()
{
	if(Z){
		D = Z2;
	}else{
		D = Z1;
	}
}