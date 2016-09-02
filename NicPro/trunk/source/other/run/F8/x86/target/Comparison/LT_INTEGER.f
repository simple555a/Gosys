fblock LT_INTEGER;
uuid{7156-7d71-62f3-19f5};
var
IN1:integer; in;
IN2:integer; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 < IN2? 1 : 0;
}
