fblock NE_FLOAT;
uuid{62bc-d70c-df80-dbd8};
var
IN1:float; in;
IN2:float; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 != IN2? 1 : 0;
}
