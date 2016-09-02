fblock EQ_SHORT;
uuid{8fa9-9726-bf53-9797};
var
IN1:short; in;
IN2:short; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 == IN2? 1 : 0;
}
