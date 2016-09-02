fblock LT_DOUBLE;
uuid{7f8a-c520-a761-c577};
var
IN1:double; in;
IN2:double; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 < IN2? 1 : 0;
}
