fblock LT_SHORT;
uuid{7283-2413-a4bf-599d};
var
IN1:short; in;
IN2:short; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 < IN2? 1 : 0;
}
