fblock GE_DOUBLE;
uuid{4351-d93d-1178-7ab6};
var
IN1:double; in;
IN2:double; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 >= IN2? 1 : 0;
}
