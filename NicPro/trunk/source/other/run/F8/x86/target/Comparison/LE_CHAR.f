fblock LE_CHAR;
uuid{db1a-49c6-7488-4f77};
var
IN1:char; in;
IN2:char; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 <= IN2? 1 : 0;
}
