fblock EQ_DOUBLE;
uuid{18ba-ae08-6f58-c6a4};
var
IN1:double; in;
IN2:double; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 == IN2? 1 : 0;
}
