unit unt_framDevice;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls, f8;

type

  TframDevice = class(TFrame)
    Image1: TImage;
    Panel1: TPanel;
    lblPos: TLabel;
    lblName: TLabel;
    procedure Image1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure Panel1DblClick(Sender: TObject);
  private
    { Private declarations }
    d : Pio_device;
    dd : Pdevlib_ent_t;
    
    di_offset, do_offset, ai_offset, ao_offset : Integer;

    procedure updateViewDdo;
    procedure updateViewDdi;
    procedure updateViewAco;
    procedure updateViewAci;

    procedure clickDdi(x, y : Integer);
    procedure clickDdo(x, y : Integer);
    procedure clickAci(x, y : Integer);
    procedure clickAco(x, y : Integer);
  public
    { Public declarations }
    addr : Integer;
    procedure updateView;
    procedure updateFirst;
  end;

implementation
{$R *.dfm}
uses
  libutils;
{ TframDdo161 }

procedure TframDevice.clickAci(x, y: Integer);
begin

end;

procedure TframDevice.clickAco(x, y: Integer);
begin

end;

procedure TframDevice.clickDdo(x, y: Integer);
begin

end;

procedure TframDevice.clickDdi(x, y: Integer);
var
  i : Integer;
  r : TRect;
  p : Pf8_u32;
begin
  r.Left := panel1.Left + panel1.Width + 5;
  r.Top := 5;
  r.Bottom := r.top + 16;
  r.Right := r.Left + 16;
  p := Pf8_u32(ke_get_io_memory(ke_get_default_kernel, d^.addr));
  for i:=0 to dd^.dev.n_di-1 do
  begin
    if ((x>r.Left) and (x<r.Right) and (y>r.Top) and(y<r.Bottom)) then
    begin
      p^ := p^ xor (1 shl i);
      break;
    end;
    Inc(r.left, 18);
    Inc(r.Right, 18);
    if (i mod 4)=3 then
    begin
      Inc(r.left, 5);
      Inc(r.Right, 5);
    end;
  end;
end;

procedure TframDevice.updateView;
begin
  d := ke_get_device_list(ke_get_default_kernel);
  Inc(d, addr);
  if (d^.features and IO_DEVICE_PRESENT)=0 then
  begin
    lblPos.Caption := 'Slot ' + IntToStr(addr);
    lblName.Caption := 'None';
    Hint := '空设备';
  end else begin
    dd := blklib_get_dev_by_id(@d^.clsid);
    if(assigned(dd)) then
    begin
      lblPos.Caption := 'Slot ' + IntToStr(addr);
      lblName.Caption := dd^.name;
      Hint := dd^.description;
      di_offset := 0;
      do_offset := di_offset + (dd^.dev.n_di shr 3);
      ai_offset := do_offset + (dd^.dev.n_do shr 3);
      ao_offset := ai_offset + (dd^.dev.n_ai * dd^.dev.ai_width);
      updateViewDdi;
      updateViewDdo;
      updateViewAci;
      updateViewAco;
    end else begin
      lblName.Caption := 'Slot ' + IntToStr(addr) + ' : Unknown';
      Hint := 'Unknown';
    end;
  end;
end;

procedure TframDevice.updateViewAci;
begin

end;

procedure TframDevice.updateViewAco;
begin
end;

procedure TframDevice.updateViewDdi;
var
  i : Integer;
  r : TRect;
  p : Pf8_u8;
  kk : f8_u32;
begin
  r.Left := panel1.Left + panel1.Width + 5;
  r.Top := 5;
  r.Bottom := r.top + 16;
  r.Right := r.Left + 16;
  p := ke_get_io_memory(ke_get_default_kernel, d^.addr);
  kk := Pf8_u32(p)^;
  for i:=0 to dd^.dev.n_di-1 do
  begin
    if (kk and 1)<>0 then
      image1.Canvas.Brush.Color := cllime
    else
      image1.Canvas.Brush.Color := clFuchsia;
    image1.canvas.FillRect(r);
    Inc(r.left, 18);
    Inc(r.Right, 18);
    if (i mod 4)=3 then
    begin
      Inc(r.left, 5);
      Inc(r.Right, 5);
    end;
    kk := kk shr 1;
  end;
end;

procedure TframDevice.updateViewDdo;
var
  i : Integer;
  r : TRect;
  p : Pf8_u8;
  kk : f8_u32;
begin
  r.Left := panel1.Left + panel1.Width + 5;
  r.Top := 25;
  r.Bottom := r.top + 16;
  r.Right := r.Left + 16;
  p := ke_get_io_memory(ke_get_default_kernel, d^.addr);
  Inc(p, do_offset);
  kk := (Pf8_u32(p))^;
  for i:=0 to dd^.dev.n_do-1 do
  begin
    if (kk and 1)<>0 then
      image1.Canvas.Brush.Color := clLime
    else
      image1.Canvas.Brush.Color := clFuchsia;
    image1.canvas.FillRect(r);
    Inc(r.left, 18);
    Inc(r.Right, 18);
    if (i mod 4)=3 then
    begin
      Inc(r.left, 5);
      Inc(r.Right, 5);
    end;
    kk := kk shr 1;
  end;
end;

procedure TframDevice.updateFirst;
var
  r : TRect;
begin
  if (addr and 1)<>0 then
    image1.Canvas.brush.Color := clGray
  else
    image1.Canvas.brush.Color := clTeal;
  r.Left := 0;
  r.Top := 0;
  r.Right := image1.Width;
  r.Bottom := image1.Height;
  image1.Canvas.FillRect(r);
  lblPos.Caption := 'Slot ' + IntToStr(addr);
  lblName.Caption := 'None';
end;

procedure TframDevice.Image1MouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  d := ke_get_device_list(ke_get_default_kernel);
  Inc(d, addr);
  if (d^.features and IO_DEVICE_PRESENT)<>0 then
  begin
    dd := blklib_get_dev_by_id(@d^.clsid);
    if(assigned(dd)) then
    begin
      if dd^.dev.n_di<>0 then
        clickDdi(x, y);
      if dd^.dev.n_do<>0 then
        clickDdo(x, y);
      if dd^.dev.n_ai<>0 then
        clickAci(x, y);
      if dd^.dev.n_ao<>0 then
        clickAco(x, y);
    end;
  end;
end;

procedure TframDevice.Panel1DblClick(Sender: TObject);
var
  ss : string;
  ad : integer;
begin
  ss := intToStr(addr);
  ss := Inputbox('卡件地址','输入新地址：',ss);
  ad := strToInt(ss);
  if ad<>addr then
    addr := ad;
end;

end.
