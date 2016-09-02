fblock DM511;
attribute(autoscan);
uuid{0d52-e32e-1914-badf};
var
	ADDR : word;in;
	F1 : float;out;
	F2 : float;out;
implementation
int main()
{
	int * pVal;
	pVal = (int*)io_mem(ADDR);
	if(!pVal){
		return -1;
	}
	
	if(*pVal == 0){
		F1 = 0.;
	}else{
		F1 = 60. / *pVal * 10000000.;
	}	
	
	pVal++;
	if(*pVal == 0){
		F2 = 0.;
	}else{
		F2 = 60. / *pVal * 10000000.;
	}	

	return 0;
}	