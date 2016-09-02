fblock EQ_DWORD;
uuid{9ec6-0da6-244c-b1ae};
var
IN1:dword; in;
IN2:dword; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 == IN2? 1 : 0;
}
