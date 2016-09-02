fblock GE_DWORD;
uuid{0a3a-98e6-112e-7fb7};
var
IN1:dword; in;
IN2:dword; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 >= IN2? 1 : 0;
}
