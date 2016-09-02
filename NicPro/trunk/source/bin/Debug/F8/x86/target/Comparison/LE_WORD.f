fblock LE_WORD;
uuid{52de-ed33-a796-5a5b};
var
IN1:word; in;
IN2:word; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 <= IN2? 1 : 0;
}
