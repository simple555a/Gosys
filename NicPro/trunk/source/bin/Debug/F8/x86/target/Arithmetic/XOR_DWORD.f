fblock XOR_DWORD;
uuid{7366-c61c-c131-fd65};
var
IN1:dword; in;
IN2:dword; in;
OUT:dword; out;
implementation
void main()
{
OUT = IN1 ^ IN2;
}
