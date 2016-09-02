fblock DM321;
attribute(autoscan);
uuid{72f3-ce40-0dfb-acf0};
var
	ADDR : word;in;
	T1 : byte;in;
	T2 : byte;in;
	T3 : byte;in;
	T4 : byte;in;
	T5 : byte;in;
	T6 : byte;in;
	T7 : byte;in;
	T8 : byte;in;
	
	FA1 : float;in;
	FA2 : float;in;
	FA3 : float;in;
	FA4 : float;in;
	FA5 : float;in;
	FA6 : float;in;
	FA7 : float;in;
	FA8 : float;in;
	Force: bool;in;
	Sel_CH : byte;in;
	
	VF : byte;out;
	A1 : float;out;
	A2 : float;out;
	A3 : float;out;
	A4 : float;out;
	A5 : float;out;
	A6 : float;out;
	A7 : float;out;
	A8 : float;out;
	Err1:bool; out;
	Err2:bool; out;
	Err3:bool; out;
	Err4:bool; out;
	Err5:bool; out;
	Err6:bool; out;
	Err7:bool; out;
	Err8:bool; out;
		
implementation

static char buffer[24];
static int check_buffer()
{
	int i;
	byte * pci;
	pci = (byte*)io_mem(ADDR);
	for(i=0; i<24; i++){
		if(pci[i]!=buffer[i]){
			return 1;
		}
	}
	
	return 0;
}

static void write_buffer()
{
	int i;
	byte * pco;
	pco = (byte*)io_mem(ADDR);
	
	pco[0]=T1;
	pco[1]=T2;
	pco[2]=T3;
	pco[3]=T4;
	pco[4]=T5;
	pco[5]=T6;
	pco[6]=T7;
	pco[7]=T8;
	
	for(i=8;i<24;i++){
		pco[i]=0;
	}
	
	for(i=0;i<24;i++){
		buffer[i]=pco[i];
	}
}

static void check_ch()
{
	if(VF & 0x01){
		Err1 = 1;
	}else{
		Err1 = 0;
	}

	if(VF & 0x02){
		Err2 = 1;
	}else{
		Err2 = 0;
	}

	if(VF & 0x04){
		Err3 = 1;
	}else{
		Err3 = 0;
	}

	if(VF & 0x08){
		Err4 = 1;
	}else{
		Err4 = 0;
	}

	if(VF & 0x10){
		Err5 = 1;
	}else{
		Err5 = 0;
	}

	if(VF & 0x20){
		Err6 = 1;
	}else{
		Err6 = 0;
	}

	if(VF & 0x40){
		Err7 = 1;
	}else{
		Err7 = 0;
	}

	if(VF & 0x80){
		Err8 = 1;
	}else{
		Err8 = 0;
	}
}

static float get_ad_val(float val_ch, byte t)
{
	float val;
	
	if(t==0){
		if(val_ch < 13107){
			val = 0;
		}else{
			val = (float)(val_ch-13107) / 52428;
		}
	}else{
		val = (float)val_ch  / 65535.;
	}
	
	return val;
}

static void get_value()
{
	word * pVal;
	pVal = (word*)io_mem(ADDR);

	VF=0;
	if(Force && Sel_CH & 0x0001){
		A1 = FA1;
	}else{ 
       		A1 = get_ad_val(pVal[0],T1);
       		if(pVal[0] < 4096 && T1 ==0){
			VF |= 1 ;
		}
       	}
	if(Force && Sel_CH & 0x0002){
		A2 = FA2;
	}else{ 
       		A2 = get_ad_val(pVal[1],T2);
       		if(pVal[1] < 4096 && T2 ==0){
			VF |= 1 << 1;
		}
       	}
	if(Force && Sel_CH & 0x0004){
		A3 = FA3;
	}else{ 
       		A3 = get_ad_val(pVal[2],T3);
       		if(pVal[2] < 4096 && T3 ==0){
			VF |= 1 << 2;
		}
       	}
	if(Force && Sel_CH & 0x0008){
		A4 = FA4;
	}else{ 
       		A4 = get_ad_val(pVal[3],T4);
       		if(pVal[3] < 4096 && T4 ==0){
			VF |= 1 << 3;
		}
       	}
	if(Force && Sel_CH & 0x0010){
		A5 = FA5;
	}else{ 
       		A5 = get_ad_val(pVal[4],T5);
       		if(pVal[4] < 4096 && T5 ==0){
			VF |= 1 << 4;
		}
       	}
	if(Force && Sel_CH & 0x0020){
		A6 = FA6;
	}else{ 
       		A6 = get_ad_val(pVal[5],T6);
       		if(pVal[5] < 4096 && T6 ==0){
			VF |= 1 << 5;
		}
       	}
	if(Force && Sel_CH & 0x0040){
		A7 = FA7;
	}else{ 
       		A7 = get_ad_val(pVal[6],T7);
       		if(pVal[6] < 4096 && T7 ==0){
			VF |= 1 << 6;
		}
       	}
	if(Force && Sel_CH & 0x0080){
		A8 = FA8;
	}else{ 
       		A8 = get_ad_val(pVal[7],T8);
       		if(pVal[7] < 4096 && T8 ==0){
			VF |= 1 << 7;
		}
       	}
	
	check_ch();
}

int main()
{	
	word * pVal;
	pVal = (word*)io_mem(ADDR);
	if(!pVal){
		VF=0xff;
		check_ch();
		return -1;
	}
	
	if(check_buffer()==1){
		get_value();
	}
	write_buffer();		
}