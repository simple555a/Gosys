unit f8ui;

interface
uses
  windows,
  f8,
  rtkdefs,
  libcobj,
  classes,
  math,
  libutils,
  forms,
  blklayout,
  controls;

var
  g_Layout : TBlkLayout;
  g_bOnline : boolean;

procedure Unimplemented(h : hwnd);

implementation
uses
  sysutils;

procedure Unimplemented(h : hwnd);
begin
  messagebox(h, 'unimplemented', 'error', mb_ok);
end;

initialization

end.

