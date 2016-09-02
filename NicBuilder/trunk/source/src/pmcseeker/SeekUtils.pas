unit SeekUtils;

interface

procedure ZeroChrArray(var ca:Array of char; lb:integer; ub:integer);
procedure PChar2Array(pc:PChar; var ca:Array of char; lb:integer; ub:integer);

implementation

procedure ZeroChrArray(var ca:Array of char; lb:integer; ub:integer);
var
  i:integer;
begin
  for i := lb to ub do
      ca[i] := chr(0);
end;

procedure PChar2Array(pc:PChar; var ca:Array of char; lb:integer; ub:integer);
var
  i:integer;
begin
  for i := lb to ub do
    ca[i] := pc[i];
end;

end.
