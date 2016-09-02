fblock LE_BYTE;
uuid{fa35-4e64-4a7d-e71a};
var
IN1:byte; in;
IN2:byte; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 <= IN2? 1 : 0;
}
