fblock GE_INTEGER;
uuid{c67a-d674-3df9-76df};
var
IN1:integer; in;
IN2:integer; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 >= IN2? 1 : 0;
}
