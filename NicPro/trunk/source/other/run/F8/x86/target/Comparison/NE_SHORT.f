fblock NE_SHORT;
uuid{a92d-965e-15d6-607f};
var
IN1:short; in;
IN2:short; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 != IN2? 1 : 0;
}
