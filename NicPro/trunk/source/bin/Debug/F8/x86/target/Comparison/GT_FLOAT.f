fblock GT_FLOAT;
uuid{61de-0592-b54a-0887};
var
IN1:float; in;
IN2:float; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 > IN2? 1 : 0;
}
