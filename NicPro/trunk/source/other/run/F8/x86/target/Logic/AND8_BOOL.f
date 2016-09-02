fblock AND8_BOOL;
uuid{058c-8172-33d6-e196};
var
IN1:bool; in;
IN2:bool; in;
IN3:bool; in;
IN4:bool; in;
IN5:bool; in;
IN6:bool; in;
IN7:bool; in;
IN8:bool; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 && IN2 && IN3 && IN4 && IN5 && IN6 && IN7 && IN8;
}
