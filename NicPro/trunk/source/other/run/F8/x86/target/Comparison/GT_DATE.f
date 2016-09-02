fblock GT_DATE;
uuid{8326-eb1e-254e-a294};
var
IN1:date; in;
IN2:date; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 > IN2? 1 : 0;
}
