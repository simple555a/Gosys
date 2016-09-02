fblock LE_DWORD;
uuid{8b81-0335-9765-ff7f};
var
IN1:dword; in;
IN2:dword; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 <= IN2? 1 : 0;
}
