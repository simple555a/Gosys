unit TagAttrs;

interface
uses
  SysUtils,
  librtdb;

type

TTagProps = record
  _type:string;
  Flags:string;
  alarmRank:string;
  alarmDeadBand:string;
  hihi:string;
  hi:string;
  lo:string;
  lolo:string;
  rate:string;
  minValue:string;
  maxValue:string;
  eu:string;
  onMsg:string;
  offMsg:string;
  device:string;
  address:string;
  description:string;
end;

TTagAttrs = class
  attrs:TRtkTag;
  discovered:boolean;
public
  constructor Create;
end;
PTagAttrs = ^TTagAttrs;

procedure Attrs2Props(var attrs:TStaticTagData; var props:TTagProps);

implementation

constructor TTagAttrs.Create;
begin
  discovered := false;
end;

procedure AddNewMask(var mask:string; newItem:string);
begin
  if mask = '' then
    mask := newItem
  else
    mask := mask + ' + ' + newItem;
end;

procedure Attrs2Props(var attrs : TStaticTagData; var props:TTagProps);
begin
  case get_tag_type(attrs.Flags) of
  dt_bool:
    props._type := 'b(开关量)';
  dt_real4:
    props._type := 'f(模拟量)';
  dt_real8:
    props._type := 'f(高精度模拟量)';
  dt_int16:
    props._type := '16位整数';
  dt_int32:
    props._type := '32位整数';
  dt_date:
    props._type := '日期';
  end;

  props.Flags := '';
  if (attrs.Flags and TF_HiHi) <> 0 then
    AddNewMask(props.Flags, 'h2(高2)');
  if (attrs.Flags and TF_Hi) <> 0 then
    AddNewMask(props.Flags, 'h(高)');
  if (attrs.Flags and TF_Lo) <> 0 then
    AddNewMask(props.Flags, 'l(低)');
  if (attrs.Flags and TF_LoLo) <> 0 then
    AddNewMask(props.Flags, 'l2(低2)');
  if (attrs.Flags and TF_Rate) <> 0 then
    AddNewMask(props.Flags, 'r(速率)');
  if (attrs.Flags and TF_On2Off) <> 0 then
    AddNewMask(props.Flags, 'off(关)');
  if (attrs.Flags and TF_Off2On) <> 0 then
    AddNewMask(props.Flags, 'on(开)');

  props.alarmRank := format('%d', [attrs.AlarmRank]);
  case get_tag_type(attrs.flags) of
  DT_Real4,DT_Real8:
    begin
      props.hihi := Format('%f', [attrs.u_hihi.flt]);
      props.hi := Format('%f', [attrs.u_hi.flt]);
      props.lo := Format('%f', [attrs.u_lo.flt]);
      props.lolo := Format('%f', [attrs.u_lolo.flt]);
      props.rate := Format('%f', [attrs.fltRate]);
      props.alarmDeadBand := Format('%f', [attrs.u_deadband.flt]);
      props.minValue := Format('%f', [attrs.u_min.flt]);
      props.maxValue := Format('%f', [attrs.u_max.flt]);
      props.eu := attrs.Msg.EU;
    end;
  dt_bool:
    begin
      props.onMsg := attrs.Msg.OnMsg;
      props.onMsg := attrs.Msg.OffMsg;
    end;
  dt_int16, dt_int32:
    begin
      props.hihi := Format('%f', [attrs.u_hihi.i]);
      props.hi := Format('%f', [attrs.u_hi.i]);
      props.lo := Format('%f', [attrs.u_lo.i]);
      props.lolo := Format('%f', [attrs.u_lolo.i]);
      props.rate := Format('%f', [attrs.fltRate]);
      props.alarmDeadBand := Format('%f', [attrs.u_deadband.i]);
      props.minValue := Format('%f', [attrs.u_min.i]);
      props.maxValue := Format('%f', [attrs.u_max.i]);
      props.eu := attrs.Msg.EU;
    end;
  end;
  
  props.device := attrs.Device;
  props.address := attrs.Address;
  props.description := attrs.Description;
end;

end.
