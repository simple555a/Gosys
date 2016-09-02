fblock LT_DATE;
uuid{452a-9be3-a3e1-d9cf};
var
IN1:date; in;
IN2:date; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 < IN2? 1 : 0;
}
