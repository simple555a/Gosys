fblock GE_SHORT;
uuid{90d3-0cc0-a519-73f3};
var
IN1:short; in;
IN2:short; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 >= IN2? 1 : 0;
}
