fblock NE_CHAR;
uuid{6d9b-05ca-8012-c594};
var
IN1:char; in;
IN2:char; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 != IN2? 1 : 0;
}
