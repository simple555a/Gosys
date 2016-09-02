fblock EQ_DATE;
uuid{517f-683f-b3cb-6318};
var
IN1:date; in;
IN2:date; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 == IN2? 1 : 0;
}
