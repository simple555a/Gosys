fblock GT_WORD;
uuid{2cf3-d288-4aad-f409};
var
IN1:word; in;
IN2:word; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 > IN2? 1 : 0;
}
