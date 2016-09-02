fblock FUN12;		//12段函数变换，x应递增填写，不递增曲线取到递增的转折点为止
uuid{9780-ffa8-6266-121c};

var
     X:float;in;
     X1:float;in;
     Y1:float;in;
     X2:float;in;
     Y2:float;in;
     X3:float;in;
     Y3:float;in;
     X4:float;in;
     Y4:float;in;
     X5:float;in;
     Y5:float;in;
     X6:float;in;
     Y6:float;in;
     X7:float;in;
     Y7:float;in;
     X8:float;in;
     Y8:float;in;
     X9:float;in;
     Y9:float;in;
     X10:float;in;
     Y10:float;in;
     X11:float;in;
     Y11:float;in;
     X12:float;in;
     Y12:float;in;
	
     Y:float;out;

implementation

static float get_value(float x, float x1, float y1, float x2, float y2)
{
	if((x2-x1) < 1.e-10){
		return y1;
	}else{
		return y1 + (x-x1)/(x2-x1)*(y2-y1);
	}
}

void main()
{
	if(X < X1){
		Y = Y1;
	}else if(X < X2){
		Y = get_value(X, X1, Y1, X2, Y2);
	}else if(X < X3){
		Y = get_value(X, X2, Y2, X3, Y3);
	}else if(X < X4){
		Y = get_value(X, X3, Y3, X4, Y4);
	}else if(X < X5){
		Y = get_value(X, X4, Y4, X5, Y5);
	}else if(X < X6){
		Y = get_value(X, X5, Y5, X6, Y6);
	}else if(X < X7){
		Y = get_value(X, X6, Y6, X7, Y7);
	}else if(X < X8){
		Y = get_value(X, X7, Y7, X8, Y8);
	}else if(X < X9){
		Y = get_value(X, X8, Y8, X9, Y9);
	}else if(X < X10){
		Y = get_value(X, X9, Y9, X10, Y10);
	}else if(X < X11){
		Y = get_value(X, X10, Y10, X11, Y11);
	}else if(X < X12){
		Y = get_value(X, X11, Y11, X12, Y12);
	}else{
		Y = Y12;
	}
}