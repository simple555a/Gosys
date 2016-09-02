fblock GE_DATE;
uuid{dbab-30ea-fcbb-548f};
var
IN1:date; in;
IN2:date; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 >= IN2? 1 : 0;
}
