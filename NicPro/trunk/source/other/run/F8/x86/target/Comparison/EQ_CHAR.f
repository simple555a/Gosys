fblock EQ_CHAR;
uuid{b83c-34b4-9b64-dcc9};
var
IN1:char; in;
IN2:char; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 == IN2? 1 : 0;
}
