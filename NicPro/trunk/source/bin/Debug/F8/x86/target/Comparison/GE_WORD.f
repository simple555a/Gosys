fblock GE_WORD;
uuid{ca53-a52d-21b0-abc6};
var
IN1:word; in;
IN2:word; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 >= IN2? 1 : 0;
}
