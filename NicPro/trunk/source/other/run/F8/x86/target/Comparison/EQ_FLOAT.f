fblock EQ_FLOAT;
uuid{be26-e36a-1cd3-65a0};
var
IN1:float; in;
IN2:float; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 == IN2? 1 : 0;
}
