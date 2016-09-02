fblock DM322RAW;
attribute(autoscan);
uuid{f541-7ef3-8eda-8760};
var
	ADDR: word; in;
	RA1: integer; out;
	RA2: integer; out;
	RA3: integer; out;
	RA4: integer; out;
	RA5: integer; out;
	RA6: integer; out;
	RA7: integer; out;
	RA8: integer; out;
	M9: word; out;
	A1: float; out;
	A2: float; out;
	A3: float; out;
	A4: float; out;
	A5: float; out;
	A6: float; out;
	A7: float; out;
	A8: float; out;	
	T: float; out;
	
implementation

static float get_tc_val(integer val_ch)
{
	float val, ref;
	int s;	
	
	if(val_ch > 30000 || val < -30000){
		return -999.;
	}
	
	val = (float)val_ch * 2.5 / 32. / 32768.;	
	
	return  val;
	
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
		return -1;
	}
	
	RA1 = pVal[0];
	RA2 = pVal[1];
	RA3 = pVal[2];
	RA4 = pVal[3];
	RA5 = pVal[4];
	RA6 = pVal[5];
	RA7 = pVal[6];
	RA8 = pVal[7];
	REF = (float)pVal[8] * 4000 / 32768 / 8;
	M9 = REF;

	A1 = get_tc_val(RA1);
	A2 = get_tc_val(RA2);
	A3 = get_tc_val(RA3);
	A4 = get_tc_val(RA4);
	A5 = get_tc_val(RA5);
	A6 = get_tc_val(RA6);
	A7 = get_tc_val(RA7);
	A8 = get_tc_val(RA8);
	REF = val_pt100(REF);
	T = REF;
}