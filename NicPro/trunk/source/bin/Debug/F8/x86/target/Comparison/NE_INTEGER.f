fblock NE_INTEGER;
uuid{4371-a324-a58b-4833};
var
IN1:integer; in;
IN2:integer; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 != IN2? 1 : 0;
}
