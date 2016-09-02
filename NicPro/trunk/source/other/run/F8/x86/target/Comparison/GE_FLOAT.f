fblock GE_FLOAT;
uuid{c822-1271-02f5-992a};
var
IN1:float; in;
IN2:float; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 >= IN2? 1 : 0;
}
