fblock GT_DWORD;
uuid{2d69-2778-8a25-727a};
var
IN1:dword; in;
IN2:dword; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 > IN2? 1 : 0;
}
