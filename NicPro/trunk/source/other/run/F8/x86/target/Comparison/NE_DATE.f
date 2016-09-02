fblock NE_DATE;
uuid{c880-4083-75ca-efd7};
var
IN1:date; in;
IN2:date; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 != IN2? 1 : 0;
}
