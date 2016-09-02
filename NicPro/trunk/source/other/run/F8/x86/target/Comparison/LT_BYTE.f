fblock LT_BYTE;
uuid{9cd5-42ad-cc93-18d7};
var
IN1:byte; in;
IN2:byte; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 < IN2? 1 : 0;
}
