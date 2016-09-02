fblock OR_DWORD;
uuid{deb1-f40d-6561-8804};
var
IN1:dword; in;
IN2:dword; in;
OUT:dword; out;
implementation
void main()
{
OUT = IN1 | IN2;
}
