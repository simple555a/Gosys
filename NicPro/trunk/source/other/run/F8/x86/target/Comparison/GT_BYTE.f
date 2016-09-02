fblock GT_BYTE;
uuid{1142-0e4e-0258-4033};
var
IN1:byte; in;
IN2:byte; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 > IN2? 1 : 0;
}
