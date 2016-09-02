fblock DM313FIX;
attribute(autoscan);
uuid{bde8-883b-134e-dcdc};
var
	ADDR : word;in;		//模块地址
	T1 : byte;in;		//T1~T8：对应通道热阻分度号
	T2 : byte;in;
	T3 : byte;in;
	T4 : byte;in;
	T5 : byte;in;
	T6 : byte;in;
	T7 : byte;in;
	T8 : byte;in;
	FA1 : float;in;		//FA1~FA8：对应通道强制输入
	FA2 : float;in;
	FA3 : float;in;
	FA4 : float;in;
	FA5 : float;in;
	FA6 : float;in;
	FA7 : float;in;
	FA8 : float;in;
	Force: bool;in;		//
	Sel_CH: byte;in;	//
	C1 : float;in;		//C1~C8：对应通道修正系数
	C2 : float;in;
	C3 : float;in;
	C4 : float;in;
	C5 : float;in;
	C6 : float;in;
	C7 : float;in;
	C8 : float;in;
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
static float get_rtd_val(word val_ch, word val_ref, byte t)
{
	float val;

	if(val_ch == 0xffff || val_ref < 100){
		return -999.;
	}

	if(val_ref == 0xffff){
		val = (float)val_ch * 4000 / 32768;
	}else{
		val = (float)val_ch / (float)val_ref * 500.;
	}

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

int main()
{
	int i;
	word * pVal;
	pVal = (word*)io_mem(ADDR);
	
	if(!pVal){
		VF=0xff;
		check_ch();
		return -1;
	}
	
	VF = 0;
	
	if(pVal[8] < 100){
		for(i = 0; i < 8; i++){
			VF |= 1 << i;
		}
	}else{
		for(i = 0; i < 8; i++){
			if(pVal[i] == 0xffff){
				VF |= 1 << i;
			}
		}
	}
	check_ch();
	
	if(Force && (Sel_CH & 0x01)){
		A1 = FA1;
	}else{
		A1 = get_rtd_val(pVal[0], pVal[8], T1) + C1;
	}
	if(Force && (Sel_CH & 0x02)){
		A2 = FA2;
	}else{
		A2 = get_rtd_val(pVal[1], pVal[8], T2) + C2;
	}
	if(Force && (Sel_CH & 0x04)){
		A3 = FA3;
	}else{
		A3 = get_rtd_val(pVal[2], pVal[8], T3) + C3;
	}
	if(Force && (Sel_CH & 0x08)){
		A4 = FA4;
	}else{
		A4 = get_rtd_val(pVal[3], pVal[8], T4) + C4;
	}
	if(Force && (Sel_CH & 0x10)){
		A5 = FA5;
	}else{
		A5 = get_rtd_val(pVal[4], pVal[8], T5) + C5;
	}
	if(Force && (Sel_CH & 0x20)){
		A6 = FA6;
	}else{
		A6 = get_rtd_val(pVal[5], pVal[8], T6) + C6;
	}
	if(Force && (Sel_CH & 0x40)){
		A7 = FA7;
	}else{
		A7 = get_rtd_val(pVal[6], pVal[8], T7) + C7;
	}
	if(Force && (Sel_CH & 0x80)){
		A8 = FA8;
	}else{
		A8 = get_rtd_val(pVal[7], pVal[8], T8) + C8;
	}
	
}