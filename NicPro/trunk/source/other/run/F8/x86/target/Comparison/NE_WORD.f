fblock NE_WORD;
uuid{1765-95e5-dedf-6d79};
var
IN1:word; in;
IN2:word; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 != IN2? 1 : 0;
}
