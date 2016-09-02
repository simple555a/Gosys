fblock GE_BYTE;
uuid{9083-7c18-7666-8a35};
var
IN1:byte; in;
IN2:byte; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 >= IN2? 1 : 0;
}
