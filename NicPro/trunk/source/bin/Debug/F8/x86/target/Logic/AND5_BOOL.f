fblock AND5_BOOL;
uuid{058c-8172-33d6-e193};
var
IN1:bool; in;
IN2:bool; in;
IN3:bool; in;
IN4:bool; in;
IN5:bool; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 && IN2 && IN3 && IN4 && IN5;
}