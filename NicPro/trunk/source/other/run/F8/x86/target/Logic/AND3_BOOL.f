fblock AND3_BOOL;
uuid{058c-8172-33d6-e191};
var
IN1:bool; in;
IN2:bool; in;
IN3:bool; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 && IN2 && IN3;
}