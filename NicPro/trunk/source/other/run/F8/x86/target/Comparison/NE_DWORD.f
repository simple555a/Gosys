fblock NE_DWORD;
uuid{c4b3-09f3-fce1-4d5c};
var
IN1:dword; in;
IN2:dword; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 != IN2? 1 : 0;
}
