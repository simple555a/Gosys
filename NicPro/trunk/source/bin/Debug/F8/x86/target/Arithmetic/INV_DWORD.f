fblock INV_DWORD;
uuid{6c4e-b1ea-300c-e5ea};
var
IN:dword; in;
OUT:dword; out;
implementation
void main()
{
OUT = ~ IN;
}
