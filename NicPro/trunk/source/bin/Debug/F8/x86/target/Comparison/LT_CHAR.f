fblock LT_CHAR;
uuid{856c-472e-8780-be84};
var
IN1:char; in;
IN2:char; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 < IN2? 1 : 0;
}
