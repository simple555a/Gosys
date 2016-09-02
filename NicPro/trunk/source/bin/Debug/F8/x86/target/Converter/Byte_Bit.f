fblock Byte_Bit;
uuid{1c5c-5913-5926-ceb1};
var
	IN:byte; in;
	
	Bit1:bool; out;
	Bit2:bool; out;
	Bit3:bool; out;
	Bit4:bool; out;
	Bit5:bool; out;
	Bit6:bool; out;
	Bit7:bool; out;
	Bit8:bool; out;

implementation

void main()
	{
	if(IN & 1){
		Bit1 = 1;
	}else{
		Bit1 = 0;
	}
	
	if(IN & 2){
		Bit2 = 1;
	}else{
		Bit2 = 0;
	}

	if(IN & 4){
		Bit3 = 1;
	}else{
		Bit3 = 0;
	}
	
	if(IN & 8){
		Bit4 = 1;
	}else{
		Bit4 = 0;
	}
	
	if(IN & 16){
		Bit5 = 1;
	}else{
		Bit5 = 0;
	}
	
	if(IN & 32){
		Bit6 = 1;
	}else{
		Bit6 = 0;
	}
	
	if(IN & 64){
		Bit7 = 1;
	}else{
		Bit7 = 0;
	}
	
	if(IN & 128){
		Bit8 = 1;
	}else{
		Bit8 = 0;
	}
}
