fblock DM322;
attribute(autoscan);
uuid{cebc-b8de-b551-ac3a};
var
	ADDR: word; in;     	//ģ���ַ
	T1: byte; in;		//ͨ��1�ֶȺ�
	T2: byte; in;		//ͨ��2�ֶȺ�
	T3: byte; in;		//ͨ��3�ֶȺ�
	T4: byte; in;		//ͨ��4�ֶȺ�
	T5: byte; in;		//ͨ��5�ֶȺ�
	T6: byte; in;		//ͨ��6�ֶȺ�
	T7: byte; in;		//ͨ��7�ֶȺ�
	T8: byte; in;		//ͨ��8�ֶȺ�
	Ex_I:bool;in;		//1=�ⲿ��˲�����0=�ڲ���˲���
	Ref_In: float; in;	//�ⲿ��˲�������
	FA1: float; in;		//ͨ��1ǿ������
	FA2: float; in;		//ͨ��2ǿ������
	FA3: float; in;		//ͨ��3ǿ������
	FA4: float; in;		//ͨ��4ǿ������
	FA5: float; in;		//ͨ��5ǿ������
	FA6: float; in;		//ͨ��6ǿ������
	FA7: float; in;		//ͨ��7ǿ������
	FA8: float; in;		//ͨ��8ǿ������
	Force: bool; in;	//ǿ�����뿪�أ�1=ǿ�ƣ�0=����
	Sel_CH: byte; in;	//ǿ������ͨ��ѡ��
	VF: byte; out;		//״̬�����
	A1: float; out;		//ͨ��1���
	A2: float; out;		//ͨ��2���
	A3: float; out;		//ͨ��3���
	A4: float; out;		//ͨ��4���
	A5: float; out;		//ͨ��5���
	A6: float; out;		//ͨ��6���
	A7: float; out;		//ͨ��7���
	A8: float; out;		//ͨ��8���
	M9: float; out;		//ͨ��8���
	Err1: bool; out;	//ͨ��1�������
	Err2: bool; out;	//ͨ��2�������
	Err3: bool; out;	//ͨ��3�������
	Err4: bool; out;	//ͨ��4�������
	Err5: bool; out;	//ͨ��5�������
	Err6: bool; out;	//ͨ��6�������
	Err7: bool; out;	//ͨ��7�������
	Err8: bool; out;	//ͨ��8�������
	Ref_Out: float; out;	//��˲���ֵ���
	
implementation
static float val_k(float k)				//K�ֶȲ���
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
		REF = (float)pVal[8] * 4000 / 32768 / 8;
		M9 = REF;
		REF = val_pt100(REF);
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