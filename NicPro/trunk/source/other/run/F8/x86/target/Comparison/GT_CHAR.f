fblock GT_CHAR;
uuid{57a8-d063-b632-21c4};
var
IN1:char; in;
IN2:char; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 > IN2? 1 : 0;
}
