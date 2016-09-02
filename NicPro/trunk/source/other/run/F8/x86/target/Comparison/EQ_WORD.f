fblock EQ_WORD;
uuid{ab8d-fdc2-08d5-188b};
var
IN1:word; in;
IN2:word; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 == IN2? 1 : 0;
}
