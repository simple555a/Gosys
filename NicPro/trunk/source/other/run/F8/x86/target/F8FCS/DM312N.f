fblock DM312N;
attribute(autoscan);
uuid{bde8-883b-134e-18dd};
var
	ADDR: word; in;     	//模块地址
	T1: byte; in;		//通道1分度号
	T2: byte; in;		//通道2分度号
	T3: byte; in;		//通道3分度号
	T4: byte; in;		//通道4分度号
	T5: byte; in;		//通道5分度号
	T6: byte; in;		//通道6分度号
	T7: byte; in;		//通道7分度号
	T8: byte; in;		//通道8分度号
	Ex_I:bool;in;		//1=外部冷端补偿；0=内部冷端补偿
	Ref_In: float; in;	//外部冷端补偿输入
	FA1: float; in;		//通道1强制输入
	FA2: float; in;		//通道2强制输入
	FA3: float; in;		//通道3强制输入
	FA4: float; in;		//通道4强制输入
	FA5: float; in;		//通道5强制输入
	FA6: float; in;		//通道6强制输入
	FA7: float; in;		//通道7强制输入
	FA8: float; in;		//通道8强制输入
	Force: bool; in;	//强制输入开关：1=强制；0=正常
	Sel_CH: byte; in;	//强制输入通道选择
	VF: byte; out;		//状态字输出
	A1: float; out;		//通道1输出
	A2: float; out;		//通道2输出
	A3: float; out;		//通道3输出
	A4: float; out;		//通道4输出
	A5: float; out;		//通道5输出
	A6: float; out;		//通道6输出
	A7: float; out;		//通道7输出
	A8: float; out;		//通道8输出
	Err1: bool; out;	//通道1故障输出
	Err2: bool; out;	//通道2故障输出
	Err3: bool; out;	//通道3故障输出
	Err4: bool; out;	//通道4故障输出
	Err5: bool; out;	//通道5故障输出
	Err6: bool; out;	//通道6故障输出
	Err7: bool; out;	//通道7故障输出
	Err8: bool; out;	//通道8故障输出
	Ref_Out: float; out;	//冷端补偿值输出
	
implementation
static float val_k(float k)				//K分度测量
{
	if(k <= 0){
		return k / 0.03553;
	}else if(k <= 4.095){
		return k / 0.04095;
	}else if(k <= 8.137){
		return (k - 4.095) / 0.04042 + 100.;
	}else if(k <= 12.207){
		return (k - 8.137) / 0.0407 + 200.;
	}else if(k <= 16.395){
		return (k - 12.207) / 0.04188 + 300.;
	}else if(k <= 20.64){
		return (k - 16.395) / 0.04245 + 400.;
	}else if(k <= 24.902){
		return (k - 20.64) / 0.04262 + 500.;
	}else if(k <= 29.128){
		return (k - 24.902) / 0.04226 + 600.;
	}else if(k <= 33.277){
		return (k - 29.128) / 0.04149 + 700.;
	}else if(k <= 37.325){
		return (k - 33.277) / 0.04148 + 800.;
	}else{
		return (k - 37.325) / 0.03985 + 900.;
	}
}

static float get_ref(float val_ref)
{
	return (float)val_ref * 0.0401;
}

// type 0 - K
static float get_tc_val(f8_i16 val_ch, float val_ref, byte t)
{
	float valr,ref,val;
	int s;	
	
	if(val_ch > 30000 || val_ch < -30000){
		return -999.;
	}
	
	if(val_ch < 0){
		s = -1;
		val_ch = - val_ch;
	}else{
		s = 1;
	}
	
	ref = get_ref(val_ref);
	valr = (float)val_ch * 2.5 / 32. / 32.768 + ref;	

	
	switch(t){
	case 0:
		val = val_k(valr);
		break;
	case 1:
		break;
	case 2:
		break;
	default:
		;
	}
	return val;
}

static void check_ch()
{
	Err1 = VF & 0x01;
	Err2 = VF & 0x02;
	Err3 = VF & 0x04;
	Err4 = VF & 0x08;
	Err5 = VF & 0x10;
	Err6 = VF & 0x20;
	Err7 = VF & 0x40;
	Err8 = VF & 0x80;
}

int main()
{
	int i;
	float REF;
	f8_i16 * pVal;
	pVal = (f8_i16*)io_mem(ADDR);
	
	if(!pVal){
		VF=0xff;
		check_ch();
		return -1;
	}
	
	VF = 0;
	for(i = 0; i < 8; i++){
		if(pVal[i] == 0x7fff){
			VF |= 1 << i;
		}
	}
	if(Ex_I){
		REF = Ref_In;
	}else{
		REF = (float)pVal[8] / 256.0;
	}
	
	Ref_Out = REF;	
	check_ch();
	
	if(Force && (Sel_CH & 0x01)){
		A1 = FA1;
	}else{
		A1 = get_tc_val(pVal[0], REF, T1);
	}
	if(Force && (Sel_CH & 0x02)){
		A2 = FA2;
	}else{
		A2 = get_tc_val(pVal[1], REF, T2);
	}
	if(Force && (Sel_CH & 0x04)){
		A3 = FA3;
	}else{
		A3 = get_tc_val(pVal[2], REF, T3);
	}
	if(Force && (Sel_CH & 0x08)){
		A4 = FA4;
	}else{
		A4 = get_tc_val(pVal[3], REF, T4);
	}
	if(Force && (Sel_CH & 0x10)){
		A5 = FA5;
	}else{
		A5 = get_tc_val(pVal[4], REF, T5);
	}
	if(Force && (Sel_CH & 0x20)){
		A6 = FA6;
	}else{
		A6 = get_tc_val(pVal[5], REF, T6);
	}
	if(Force && (Sel_CH & 0x40)){
		A7 = FA7;
	}else{
		A7 = get_tc_val(pVal[6], REF, T7);
	}
	if(Force && (Sel_CH & 0x80)){
		A8 = FA8;
	}else{
		A8 = get_tc_val(pVal[7], REF, T8);
	}

}