fblock EQ_INTEGER;
uuid{e48a-f004-6a89-fe81};
var
IN1:integer; in;
IN2:integer; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 == IN2? 1 : 0;
}
