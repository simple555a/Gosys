fblock LT_FLOAT;
uuid{0c59-284f-1fbd-9a76};
var
IN1:float; in;
IN2:float; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 < IN2? 1 : 0;
}
