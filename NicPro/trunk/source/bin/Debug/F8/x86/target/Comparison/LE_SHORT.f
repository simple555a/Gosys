fblock LE_SHORT;
uuid{24f1-d9d9-fd17-1dc1};
var
IN1:short; in;
IN2:short; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 <= IN2? 1 : 0;
}
