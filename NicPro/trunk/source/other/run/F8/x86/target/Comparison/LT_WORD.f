fblock LT_WORD;
uuid{1727-0802-dfb4-3a65};
var
IN1:word; in;
IN2:word; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 < IN2? 1 : 0;
}
