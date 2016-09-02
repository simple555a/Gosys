fblock LE_FLOAT;
uuid{acb4-f732-a436-e389};
var
IN1:float; in;
IN2:float; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 <= IN2? 1 : 0;
}
