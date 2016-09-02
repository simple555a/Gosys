fblock GE_CHAR;
uuid{f1e7-1f65-0db1-2071};
var
IN1:char; in;
IN2:char; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 >= IN2? 1 : 0;
}
