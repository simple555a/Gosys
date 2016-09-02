fblock DM323;
attribute(autoscan);
uuid{ea60-3d7b-74c6-ad42};
var
	ADDR : word;in;
	M1 : byte;in;  //接线方式，默认三线制（0）
        M2 : byte;in;
        M3 : byte;in;
        M4 : byte;in;
        M5 : byte;in;
        M6 : byte;in;
        M7 : byte;in;
        M8 : byte;in;
	T1 : byte;in;    //分度号
	T2 : byte;in;
	T3 : byte;in;
	T4 : byte;in;
	T5 : byte;in;
	T6 : byte;in;
	T7 : byte;in;
	T8 : byte;in;
	FA1 : float;in;  //强制输入
	FA2 : float;in;
	FA3 : float;in;
	FA4 : float;in;
	FA5 : float;in;
	FA6 : float;in;
	FA7 : float;in;
	FA8 : float;in;
	Force: bool;in;
	Sel_CH: byte;in;
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
	
	pco[0]=M1;
	pco[1]=M2;
	pco[2]=M3;
	pco[3]=M4;
	pco[4]=M5;
	pco[5]=M6;
	pco[6]=M7;
	pco[7]=M8;
	
	for(i=8;i<24;i++){
		pco[i]=0;
	}
	
	for(i=0;i<24;i++){
		buffer[i]=pco[i];
	}
}

static float val_pt100(float r)
{
	if(r <= 39.71){
		return (r - 39.71) * 2.3563 - 150;
	}else if(r <= 60.25){
		return (r - 60.25) * 2.4343 - 100;
	}else if(r <= 80.31){
		return (r - 80.31) * 2.4925 - 50;
	}else if(r <= 100.0){
		return (r - 100.0) * 2.5394;
	}else if(r <= 119.40){
		return (r - 100.0) * 2.5773;
	}else if(r <= 138.50){
		return (r - 119.40) * 2.6178 + 50;
	}else if(r <= 157.31){
		return (r - 138.50) * 2.6582 + 100;
	}else if(r <= 175.84){
		return (r - 157.31) * 2.6983 + 150;
	}else if(r <= 194.07){
		return (r - 175.84) * 2.7427 + 200;
	}else if(r <= 212.02){
		return (r - 194.07) * 2.7855 + 250;
	}else if(r <= 229.67){
		return (r - 212.02) * 2.8409 + 300;
	}else if(r <= 247.04){
		return (r - 229.67) * 2.8785 + 350;
	}else if(r <= 264.11){
		return (r - 247.04) * 2.9291 + 400;
	}else if(r <= 280.90){
		return (r - 264.11) * 2.9780 + 450;
	}else if(r <= 297.39){
		return (r - 280.90) * 3.0321 + 500;
	}else {
		return (r - 297.39) * 3.0675 + 550;
	}
}

static float val_cu50(float r)
{
	if(r <= 50){
		return (r - 50) * 4.6468;
	}else if(r <= 71.4){
		return (r - 50) * 4.6729;
	}else{
		return (r - 71.4) * 4.6598 + 100;
	}
}

static float val_cu100(float r)
{
	if(r <= 100){
		return (r - 100) * 2.3191;
	}else if(r <= 142.80){
		return (r - 100) * 2.3364;
	}else{
		return (r - 142.80) * 2.3288 + 100.;
	}
}

// type 0 - pt100
// type 1 - cu50
// type 2 - cu100
static float get_rtd_val(word val_ch, byte t)
{
	float val;

	if(val_ch == 0xffff ){
		return -999.;
	}

	
	val = (float)val_ch * 4000 / 32768 / 8;
	

	switch(t){
	case 0:
		val = val_pt100(val);
		break;
	case 1:
		val = val_cu50(val);
		break;
	case 2:
		val = val_cu100(val);
		break;
	default:
		val = 0;
	}
	return val;
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

static void get_value()
{
	word * pVal;
	pVal = (word*)io_mem(ADDR);

	VF = 0;
		
	if(Force && (Sel_CH & 0x01)){
		A1 = FA1;
	}else{
		A1 = get_rtd_val(pVal[0], T1);
		if(pVal[0] == 0xffff ){
			VF |= 1 ;
		}
	}
	if(Force && (Sel_CH & 0x02)){
		A2 = FA2;
	}else{
		A2 = get_rtd_val(pVal[1], T2);
		if(pVal[1] == 0xffff ){
			VF |= 1 << 1;
		}
	}
	if(Force && (Sel_CH & 0x04)){
		A3 = FA3;
	}else{
		A3 = get_rtd_val(pVal[2], T3);
		if(pVal[2] == 0xffff ){
			VF |= 1 << 2;
		}
	}
	if(Force && (Sel_CH & 0x08)){
		A4 = FA4;
	}else{
		A4 = get_rtd_val(pVal[3], T4);
		if(pVal[3] == 0xffff ){
			VF |= 1 << 3;
		}
	}
	if(Force && (Sel_CH & 0x10)){
		A5 = FA5;
	}else{
		A5 = get_rtd_val(pVal[4], T5);
		if(pVal[4] == 0xffff ){
			VF |= 1 << 4;
		}
	}
	if(Force && (Sel_CH & 0x20)){
		A6 = FA6;
	}else{
		A6 = get_rtd_val(pVal[5], T6);
		if(pVal[5] == 0xffff ){
			VF |= 1 << 5;
		}
	}
	if(Force && (Sel_CH & 0x40)){
		A7 = FA7;
	}else{
		A7 = get_rtd_val(pVal[6], T7);
		if(pVal[6] == 0xffff ){
			VF |= 1 << 6;
		}
	}
	if(Force && (Sel_CH & 0x80)){
		A8 = FA8;
	}else{
		A8 = get_rtd_val(pVal[7], T8);
		if(pVal[7] == 0xffff ){
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