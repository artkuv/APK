int main()
{
	unsigned far *fp;
	//��������� ���������

	init();

	FP_SEG (fp) = _psp; // �������� �������

	FP_OFF (fp) = 0x2c; // � �������� �������� ������
	//� ����������� �����,

	_dos_freemem(*fp); //����� ���������� ������

	_dos_keep(0,(_DS -_CS)+(_SP/16)+1);

	//��������� �����������

	return 0;
}

void init()

{

intOld = getvect(0x__);//������ ����������� 08-0F, 70-77

...

setvect(0x__, intNew); // ���� �����������

...

_disable(); // ��������� ���������� (cli)

outp(0x20, 0x11); // ICW1

outp(0x21, 0x70); // ICW2

outp(0x21, 0x04); // ICW3

outp(0x21, 0x01); // ICW4

...

_enable(); // ��������� ���������� (sti)

}

void interrupt intNew(...) { get_reg(); intOld(); }

void get_reg() // �������� ��������� ���������

{

outp(0x20, 0x0B); //�� ��� ��������

print(inp(0x20));

...

}

void print(int val) // ����������� ��������� ���������

{
int i;

VIDEO far* screen = (VIDEO far *)MK_FP(0xB800,
0);

for(i=7;i>=0;i--) //�������� �� �����

{

screen->symb= val%2 +'0'; //��������� �����

screen->attr=0x5E;

screen++;

val/=2;

}

}