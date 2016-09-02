fblock NE_DOUBLE;
uuid{65be-7f59-422b-b84c};
var
IN1:double; in;
IN2:double; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 != IN2? 1 : 0;
}
