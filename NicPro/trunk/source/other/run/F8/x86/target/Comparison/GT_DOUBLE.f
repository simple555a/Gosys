fblock GT_DOUBLE;
uuid{44c8-be94-1875-22fa};
var
IN1:double; in;
IN2:double; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 > IN2? 1 : 0;
}
