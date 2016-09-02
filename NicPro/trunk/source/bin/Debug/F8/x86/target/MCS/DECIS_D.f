fblock DECIS_D;			//表决器模块
uuid{05fd-ef4c-b8a3-14dd};
var
	C1V     :float;  in;	//测量通道1
	C2V     :float;  in;	//测量通道2
	C3V     :float;  in;	//测量通道3
	C1Bad   :bool;  in;	//测量通道1通道故障
	C2Bad   :bool;  in;	//测量通道2通道故障
	C3Bad   :bool;  in;	//测量通道3通道故障
	V	:float;  out;	//表决器输出
	CH_Bad  :bool;   out;	//两个以上测量通道故障

	
implementation
static float avge,aa,bb,cc;

void main() {
   if (C1Bad && C2Bad && C3Bad) {
   	CH_Bad = 1;
   	V = 0;
   	}else{
   	CH_Bad = 0;
	}	
   if(!C1Bad && C2Bad && C3Bad) {
   	V = C1V;
   	}
   if(C1Bad && !C2Bad && C3Bad) {
   	V = C2V;
   	}
   if(C1Bad && C2Bad && !C3Bad) {
   	V = C3V;
   	}
   if(!C1Bad && !C2Bad && C3Bad) {
   		V = (C1V + C2V)/2.0;
   		}
   if(!C1Bad && C2Bad && !C3Bad) {
   		V = (C1V + C3V)/2.0;
   		}
   if(C1Bad && !C2Bad && !C3Bad) {
   		V = (C2V + C3V)/2.0;
   		}
   if(!C1Bad && !C2Bad && !C3Bad) {
   		avge = (C1V + C2V + C3V)/3.0;
   		aa = fabs(avge - C1V);
   		bb = fabs(avge - C2V);
   		cc = fabs(avge - C3V);
   		if (aa >= bb) {
   			if (aa >= cc) {
   				V = (C2V + C3V)/2.0;
   				}
   			else { 
   				V = (C1V + C2V)/2.0;
   				}
   			}
   		else {
   		 	if(bb >= cc) {
   				V = (C1V + C3V)/2.0;
   				}
   			else {
   				V = (C1V + C2V)/2.0;
   				}
   			}
   		}
}

