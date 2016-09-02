fblock AND4_BOOL;
uuid{058c-8172-33d6-e192};
var
IN1:bool; in;
IN2:bool; in;
IN3:bool; in;
IN4:bool; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 && IN2 && IN3 && IN4;
}
