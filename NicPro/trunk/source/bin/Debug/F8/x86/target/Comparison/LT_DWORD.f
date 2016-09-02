fblock LT_DWORD;
uuid{3afc-4260-50ac-377d};
var
IN1:dword; in;
IN2:dword; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 < IN2? 1 : 0;
}
