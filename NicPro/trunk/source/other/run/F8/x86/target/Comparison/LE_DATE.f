fblock LE_DATE;
uuid{a661-8cea-bf0f-905c};
var
IN1:date; in;
IN2:date; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 <= IN2? 1 : 0;
}
