fblock GT_SHORT;
uuid{df5e-640b-3afd-57f7};
var
IN1:short; in;
IN2:short; in;
OUT:bool; out;
implementation
void main()
{
OUT = IN1 > IN2? 1 : 0;
}
