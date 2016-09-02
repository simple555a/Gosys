fblock EQ_BYTE;
uuid{a6b5-bf39-6471-c5b7};
var
IN1:byte; in;
IN2:byte; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 == IN2? 1 : 0;
}
