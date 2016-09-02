fblock NE_BYTE;
uuid{362d-01bb-5c3d-791c};
var
IN1:byte; in;
IN2:byte; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 != IN2? 1 : 0;
}
