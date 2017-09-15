#include "FileSystem.h"
Content content;
Pointer ponter;                     
OFILE ofile;
OpenFile openfile;

//�ļ�����
/*����·����Ϊname���ļ���Ŀ¼,���ظ�Ŀ¼����ʼ�̿�� */
search(char name[],int flag,int *dnum,int *bnum)/*flag=8��ʾ����Ŀ¼,����Ϊ�ļ�*//*�����ҵ��ļ���Ŀ¼��Ŀ¼���λ�ã��̿�dnum�е�bnum��*/
{
	int k,i,s,j,last=0;
	char pna[3],type[3];
	if((strcmp(name,"")==0)||(strcmp(name,"/")==0))/*��Ŀ¼*/
		return(2);
	k=0;
	if(name[0]=='/')k=1;
	i=2; /*i=��Ŀ¼����ʼ�̿��*/
	while(last!=1)
	{
		/*pna=��name�з����"/"��һ��Ŀ¼�������ļ�����*/
		for(s=0;name[k]!='.'&&name[k]!='/'&&s<3&&name[k]!='\0';s++,k++)
			pna[s]=name[k];
		for(;s<3;s++)/*�ÿո�ȫ���ֳ���*/
			pna[s]=' ';
		while(name[k]!='.'&&name[k]!='\0'&&name[k]!='/')/*��ȥ�����ַ�*/
			k++;
		type[0]=type[1]=' ';
		if(name[k]=='.')/*ȡ�ļ�������type*/
			if(flag==8)
			{
				printf("Ŀ¼��Ӧ������������,����ʧ��\n");
				return(false);
			}
			else
			{/*�ļ�������������Ϊ����,������ַ�����*/
				k++;
				if(name[k]!='\0')
					type[0]=name[k];
				k++;
				if(name[k]!='\0')
					type[1]=name[k];
				k++;
                if(name[k]!='\0')
					type[2]=name[k];
				
				if(name[k]!='\0'&&name[k+1]!='\0')
				{
					printf("�ļ�������\n");
					return(false);
				}
				last=1;
			}
			else
				if(name[k]!='\0')
					k++;
				if(name[k]=='\0')
					last=1;
				/*����Ŀ¼�����ֵ���pna��Ŀ¼��*/
				fseek(fc,i*64L,SEEK_SET);
				fread(buffer2,64L,1,fc);
				j=0;
				if(last==1&&flag!=8)
					while(j<8&&!(buffer2[j].attribute!=8&&buffer2[j].name[0]==pna[0]&&
                        buffer2[j].name[1]==pna[1]&&buffer2[j].name[2]==pna[2]&&
                        buffer2[j].type[0]==type[0]&&buffer2[j].type[1]==type[1]&&buffer2[j].type[2]==type[2]))
                        j++;
					else
                        while(j<8&&!(buffer2[j].attribute==8&&buffer2[j].name[0]==pna[0]&&buffer2[j].name[1]==pna[1]
							&&buffer2[j].name[2]==pna[2]))
							j++;
                        if(j<8)/*�ҵ���Ŀ¼���ļ�*/
							if(last==1)/*���ҽ���*/
							{
								*dnum=i;
								*bnum=j;
								return(buffer2[j].address);
							}
							else/*���һ�δ����*/
								i=buffer2[j].address;/*��ȡ��һ���̿�*/
							else 
								return(false);
	}/*while ���ҽ���*/
	return 1;
}/*search()����*/

/*����һ�����̿飬���ؿ��*/
allocate( )
{
	int i;
	fseek(fc,0,SEEK_SET); /*��ģ����̵��ļ�ָ������ģ�����FAT��*/
	fread(buffer1,64L,1,fc);/*��FAT���е�һ�����̿����ģ�⻺��buffer1��*/
	for(i=3;i<63;i++)
		if(buffer1[i]==0)
		{ /*FAT�еĵ�i��Ϊ0,�����i����̿�,�޸�FAT��,����д�ش���*/
			buffer1[i]=255;
			fseek(fc,0,SEEK_SET);
			fwrite (buffer1,64L,1,fc);
			return(i); /*���ش��̺�*/
		}
		fread(buffer1,64L,1,fc);/*��FAT���еڶ������̿����ģ�⻺��buffer1��*/
		for(i=0;i<63;i++)
			if(buffer1[i]==0)
			{/*FAT�еĵ�i��Ϊ0,�����i��64����̿�,�޸�FAT��,����д�ش���*/
				buffer1[i]=255;
				fseek(fc,-64L,SEEK_CUR);
				fwrite(buffer1,64L,1,fc);
				return(i+64); /*���ش��̺�*/
			}
			printf("�Ѿ�û�д��̿ռ�\n");
			return(false);
}
/*������̿麯������*/  

void copen(OFILE *x1,OFILE *x2)                     
{
    strcpy(x1->name,x2->name);
    x1->attribute=x2->attribute;
    x1->number=x2->number;
    x1->length=x2->length;
    x1->flag=x2->flag;
    x1->read.dnum=x2->read.dnum;
    x1->read.bnum=x2->read.bnum;
    x1->write.dnum=x2->write.dnum;
    x1->write.bnum=x2->write.bnum;
}


/*���Ѵ��ļ����в����ļ�name*/
sopen(char name[])
{
	int i;
	i=0;
	while(i<openfile.length&&strcmp(openfile.file[i].name,name)!=0)/*���β����Ѵ��ļ���*/
		i++;
	if(i>=openfile.length)
		return(-1);
	return(i);
}/*����sopen��������*/


void dopen(char name[]) /*���Ѵ��ļ�����ɾ���ļ�name*/
{
	int i;
	i=sopen(name);
	if(i==-1)
		printf("�ļ�δ��\n");
	else
	{
		copen(&openfile.file[i],&openfile.file[openfile.length-1]);
		openfile.length--;
	}
}/*ɾ����������*/

/*���Ѵ��ļ����в����ļ�name*/
iopen(OFILE *x)
{
	int i;
	i=sopen(x->name);
	if(i!=-1)
	{
		printf("�ļ��Ѿ���\n");
		return(false);
	}
	else if(openfile.length==n)
	{
		printf("�Ѵ��ļ�������\n");
		return(false);
	}
	else
	{
		copen(&openfile.file[openfile.length],x);
		openfile.length++;
		return(true);
	}
}
/*��д�Ѵ��ļ���������*/


/*�����ļ�������·����name,�ļ�����attribute*/
create_file(char name[])
{
	int i,j,k,s,d,t,b,dd,dn,bn;
	int attribute=4;
	char dname[3],tname[2],pathname[20];
	OFILE x;
	if(attribute%2==1)
	{
		printf("ֻ���ļ�,�޷�д,���ܽ���\n");
		return(false);
	}
	if(openfile.length==n)
	{
		printf("�Ѵ򿪱�����,���ܽ���\n");
		return(false);
	}
	/* ��name�ֳ������֣�Ŀ¼·��pathname��Ŀ¼��dname*/
	for(j=0;name[j]!='\0';j++) /*�������һ����/��*/
		if(name[j]=='/')
			s=j;
		/*����Ŀ¼·��*/
		for(j=0;j<s;j++)
			pathname[j]=name[j];
		pathname[j]='\0';
		/*�����ļ���*/
		for(k=0,j=s+1;name[j]!='\0'&&k<3&&name[j]!='.';j++,k++)
			dname[k]=name[j];
		if(k==0)
		{
			printf("�����ļ�����Ŀ¼��\n");
			return(false);
		}
		for(;k<3;k++)
			dname[k]=' ';
		k=0;
		
		if(name[j++]=='.')/*����������*/
		{
			for(;name[j]!='\0'&&k<5&&name[j]!='.';j++,k++)
				tname[k]=name[j];
		}
		
		for(;k<3;k++) 
			tname[k]=' ';
		if((d=search(pathname,8,&dn,&bn))==false)/*�ҵ�Ŀ¼·�������ظ�Ŀ¼���ڿ��dn������bn*/
		{
			printf("Ŀ¼�����ڣ����ܽ���");
			return(false);
		}
		/*ȷ�ϸ�Ŀ¼�����ڵ�ͬʱ���ҿ�Ŀ¼��*/
		b=-1;
		
		fseek(fc,d*64L,SEEK_SET);
		fread(buffer2,64L,1,fc); /*����dnum�̿������*/
		for(t=0;t<8;t++)
		{
			if(buffer2[t].name[0]==dname[0]&&buffer2[t].name[1]==dname[1]&&buffer2[t].name[2]==dname[2]
				&&buffer2[t].type[0]==tname[0]&&buffer2[t].type[1]==tname[1]&&buffer2[t].type[2]==tname[2])
			{   /*�ҵ�����dname���ļ�������ʧ��*/
				printf("�ļ��Ѿ����ڣ����ܽ���\n");
				return(false);
			}
			if(buffer2[t].name[0]=='$'&&b==-1)
				b=t;
		}/*for*/
		
		if(b==-1)/*û�п�Ŀ¼��,����ʧ��*/
		{
			printf("Ŀ¼�޿ռ�\n");
			return(false);
		}
		if((dd=allocate( ))==false)/*���������Ŀ¼�Ĵ����̿�dd*/
		{
			printf("�����ļ�ʧ��\n");
			return(false);
		}
		/*��дĿ¼��*/
		for(i=0;i<3;i++)
			buffer2[b].name[i]=dname[i];
		for(i=0;i<3;i++)
			buffer2[b].type[i]=tname[i];
		buffer2[b].attribute=attribute;
		buffer2[b].address=dd;
		buffer2[b].length=0;
		fseek(fc,d*64L,SEEK_SET);
		fwrite(buffer2,64L,1,fc);
		/*��д�Ѵ��ļ���*/
		strcpy(x.name,name);
		x.attribute=attribute;
		x.number=dd;
		x.length=0;
		x.flag=1;
		x.read.dnum=x.write.dnum=dd;
		x.read.bnum=x.write.bnum=0;
		printf("�ļ������ɹ� \n");
		iopen(&x);
		return 1;
		
}/*�����ļ�����*/

/*���ļ�����*/
open_file(char name[],int attribute)
{
	OFILE x;
	int dnum,bnum,last,i,d;
	if((d=search(name,4,&dnum,&bnum))==false)
	{
		printf("�ļ������ڣ��򿪲���ʧ��\n");
		return(false);
	}
	fseek(fc,dnum*64L,SEEK_SET);/*������ӦĿ¼��*/
	fread(buffer2,64,1,fc);
	if((buffer2[bnum].attribute%2==1)&& 
		attribute==1)/*��ֻ���ļ�Ҫ��д*/
	{
		printf("�ļ�����д����ʧ��");
		return(false);
	}
	strcpy(x.name,name);
	x.attribute=buffer2[bnum].attribute;
	x.number=buffer2[bnum].address;
	x.read.dnum=x.write.dnum=buffer2[bnum].address;
	x.read.bnum=x.write.bnum=0;
	x.flag=attribute;
	if(attribute==1)
	{
		while(d!='\xff')/*Ѱ���ļ�ĩβ*/
		{
			fseek(fc, d/64*64L, SEEK_SET);
			fread(buffer1,64L,1,fc);/*����dnum������FAT*/
			last=d;
			d=buffer1[d%64];/*����dnum����һ�����ݸ���dnum*/
		}/*while*/
		x.write.dnum=last;/*��ддָ��*/
		fseek(fc, last*64L, SEEK_SET);
		fread(buffer1,64L,1,fc);
		for(i=0;i<64&&buffer1[i]!='#';i++);
		x.write.bnum=i;
		x.length=(buffer2[bnum].length-1)*64+i;
	}
	iopen(&x);/*��д�Ѵ��ļ���*/
	return 1;
}
/*�ļ��򿪽���*/


/*�ر��ļ�����*/
close_file(char name[])
{
	int i,dnum,bnum;
	if((i=sopen(name))==-1)
	{
		printf("�򿪵��ļ���û�и��ļ�,�ر�ʧ��\n");
		return(false);
	}
	if(openfile.file[i].flag==1)/*д�ļ���׷���ļ�������*/
	{
		fseek(fc,openfile.file[i].write.dnum*64L, SEEK_SET);
		fread(buffer1,64,1,fc);
		buffer1[openfile.file[i].write.bnum]='#';
		fseek(fc,openfile.file[i].write.dnum*64L, SEEK_SET);
		fwrite(buffer1,64,1,fc);
		fputc('#',fc);
		search(name,4,&dnum,&bnum);/*���Ҹ��ļ�Ŀ¼λ��*/
		/*�޸�Ŀ¼�е��ļ�����*/
		fseek(fc,dnum*64L, SEEK_SET);
		fread(buffer2,64,1,fc);
		buffer2[bnum].length=openfile.file[i].length/64+1;
		fseek(fc, dnum*64L, SEEK_SET);
		fwrite(buffer2,64,1,fc);
	}
	/*���Ѵ��ļ�����ɾ�����ļ��ĵǼ���*/
	if(openfile.length>1)
		copen(&openfile.file[i],&openfile.file[openfile.length-1]);
	openfile.length--;
	printf("�ļ��ر�\n");
	return 1;
}

/*���ļ��������ļ�·����name����ȡ����length*/
read_file(char name[],int length)
{
	int i,t;
	if((i=sopen(name))==-1)
	{
		printf("�ļ�û�д򿪻򲻴���\n");
		return(false);
	}
	if(openfile.file[i].flag==1)
	{
		printf("�ļ���д��ʽ�򿪣����ܶ�! ���ȹر��ļ����Զ��ķ�ʽ��!\n");
		return(false);
	}
	t=0;
	fseek(fc,openfile.file[i].read.dnum*64L,SEEK_SET);
	fread(buffer1,64,1,fc);
	while(t<length&&buffer1[openfile.file[i].read.bnum]!='#')
	{
		putchar(buffer1[openfile.file[i].read.bnum]);/*����һ���ַ�������������Ļ����ʾ��*/
		if((t+1)%64==0)putchar('\n');
		/*�޸Ķ�ָ��*/
		openfile.file[i].read.bnum++;
		if(openfile.file[i].read.bnum>=64)/*һ����꣬��ȡ��һ���̿�*/
		{
			fseek(fc,openfile.file[i].read.dnum/64*64, SEEK_SET);
			fread(buffer1,64,1,fc);
			openfile.file[i].read.dnum=buffer1[openfile.file[i].read.dnum%64];/*�޸Ķ�ָ��*/
			openfile.file[i].read.bnum=0;
			fseek(fc,openfile.file[i].read.dnum*64L,SEEK_SET);
			fread(buffer1,64,1,fc);/*��ȡ��һ��*/
		}
		t++;
	}
	return 1;
}
/*����������*/

/*д�ļ�����*/
write_file(char name[],char buff[],int length)/*�ļ�·���� ���׼��д����̵����� д�����ݵĳ���*/
{
	int i,t,dd;
	int d,dn,bn;
	
	if((i=sopen(name))==-1)/*�ļ������ڣ��޷�д*/
	{
		printf("�ļ�û�д򿪻򲻴���\n");
		return(false);
	}
	if(openfile.file[i].flag==0)
	{
		printf("�ļ��Զ���ʽ�򿪣�����д\n");
		return(false);
	}
	t=0;
	fseek(fc,openfile.file[i].write.dnum*64L, SEEK_SET);
	fread(buffer1,64,1,fc);
	while(t<length)
	{
		buffer1[openfile.file[i].write.bnum]=buff[t];
		openfile.file[i].write.bnum++;
		openfile.file[i].length++;
		if(openfile.file[i].write.bnum>=64)
		{
			fseek(fc, openfile.file[i].write.dnum*64L, SEEK_SET);
			fwrite(buffer1,64,1,fc);/*һ��д�꣬д�ش���*/
			if((dd=allocate())==false)
			{
				openfile.file[i].write.bnum--;
				openfile.file[i].length--;
				printf("�޴��̿ռ�,������Ϣ��ʧ��дʧ��\n");
				return(false);
			}/*if*/
			fseek(fc,openfile.file[i].write.dnum/64*64L, SEEK_SET);
			fread(buffer1,64,1,fc);
			buffer1[openfile.file[i].write.dnum%64]=dd;
			fseek(fc,openfile.file[i].write.dnum/64*64L, SEEK_SET);
			fwrite(buffer1,64,1,fc);
			openfile.file[i].write.dnum=dd;
			openfile.file[i].write.bnum=0;
		}/*if*/
		t++;
	}/*while*/
	fseek(fc, openfile.file[i].write.dnum*64L, SEEK_SET);
	fwrite(buffer1,64,1,fc);/*һ��д�꣬д�ش���*/
	
    if((d=search(name,4,&dn,&bn))==false)/*�ҵ�Ŀ¼·�������ظ�Ŀ¼���ڿ��dn������bn*/
	{
		printf("Ŀ¼�����ڣ����ܽ���");
		return(false);
	}
	
	fseek(fc,dn*64L,SEEK_SET);
	fread(buffer2,64L,1,fc); /*����dnum�̿������*/
	
	buffer2[bn].length=length;
	fseek(fc,dn*64L,SEEK_SET);
	fwrite(buffer2,64L,1,fc);	
	   return 1;
}/*д��������*/



/*ɾ���ļ�*/
delete_file(char name[])
{
	int dnum,bnum,t;
	if((t=search(name,4,&dnum,&bnum))==false)
	{
		printf("�ļ�������\n");
		return(false);
	}
	if(sopen(name)!=-1)
	{
		printf("���ļ��򿪣�����ɾ��\n");
		return(false);
	}
	fseek(fc,dnum*64L, SEEK_SET);
	fread(buffer2,64,1,fc);
	buffer2[bnum].name[0]='$';/*�����ļ���Ŀ¼�óɿ�Ŀ¼*/
	fseek(fc,dnum*64L, SEEK_SET);
	fwrite(buffer2,64,1,fc);
	while(t!='\xff')/*ͨ��FAT����ÿһ���̿�ţ�������ɾ��*/
	{
		dnum=t;
		fseek(fc, dnum/64*64, SEEK_SET);
		fread(buffer1,64,1,fc);
		t=buffer1[dnum%64];
		buffer1[dnum%64]=0;
		fseek(fc, dnum/64*64L, SEEK_SET);
		fwrite(buffer1,64,1,fc);
	}
	printf("�ļ�ɾ���ɹ�!\n");
	return 1;
}/*�ļ�ɾ������*/


/*����Ŀ¼������Ŀ¼·����name*/
md(char name[])
{
	int i,j,k,s,d,t,b,dd,dn,bn;
	char dname[3],pathname[20];
	i=2;/* i=��Ŀ¼����ʼ�̿��*/
	/* ��name�ֳ������֣�Ŀ¼·��pathname��Ŀ¼��dname*/
	for(j=0;name[j]!='\0';j++)/*�������һ����/��*/
		if(name[j]=='/')s=j;
		/*����Ŀ¼·��*/
		for(j=0;j<s;j++)
			pathname[j]=name[j];
		pathname[j]='\0';
		/*����Ŀ¼��*/
		for(k=0,j=s+1;name[j]!='\0'&&k<3&&name[j]!='.';j++,k++)
			dname[k]=name[j];
		if(k==0)
		{
			printf("�����ļ�����Ŀ¼��\n");
			return(false);
		}
		for(;k<3;k++)
			dname[k]=' ';
		if((d=search(pathname,8,&dn,&bn))==false)/*�ҵ�Ŀ¼·��*/
		{
			printf("Ŀ¼�����ڣ����ܽ���\n");
			return(false);
		}
		b=-1;
		/*ȷ�ϸ�Ŀ¼�����ڵ�ͬʱ���ҿ�Ŀ¼��*/
		fseek(fc,d*64L,SEEK_SET);
		fread(buffer2,64L,1,fc);/*����d�̿������*/
		for(t=0;t<8;t++)
		{
			if(buffer2[t].name[0]==dname[0]&&buffer2[t].name[1]==dname[1]
				&&buffer2[t].name[2]==dname[2]&&buffer2[t].attribute==8)
			{/*�ҵ�����dname��Ŀ¼������ʧ��*/
				printf("Ŀ¼�Ѿ����ڣ����ܽ���\n");
				return(false);
			}
			if(buffer2[t].name[0]=='$'&&b==-1)
				b=t;
		}/*for*/
		if(b==-1)/*û�п�Ŀ¼� ���ܽ���*/
		{
			printf("Ŀ¼�޿ռ�\n");
			return(false);
		}
		if((dd=allocate( ))==false)/*���������Ŀ¼�Ĵ����̿�dd*/
		{
			printf("Ŀ¼���ܽ���\n");
			return(false);
		}
		/*��дĿ¼��*/
		for(i=0;i<3;i++)
			buffer2[b].name[i]=dname[i];
		buffer2[b].type[0]='d'; 
		buffer2[b].type[1]='i';
		buffer2[b].type[2]='r'; 
		buffer2[b].attribute=8;
		buffer2[b].address=dd;
		buffer2[b].length=0;
		fseek(fc,d*64L,SEEK_SET);
		fwrite(buffer2,64L,1,fc);
		/*�ָ��½�Ŀ¼���̿��ʼ��*/
		for(t=0;t<8;t++)
			buffer2[t].name[0]='$';
		fseek(fc, dd*64L, SEEK_SET);
		fwrite(buffer2,64L,1,fc);
        printf("Ŀ¼�����ɹ�!\n");
		return 1;
}
/*����Ŀ¼����*/


/*ɾ��Ŀ¼������Ŀ¼·����name*/
rd(char name[])
{
	int i,j,k,s,d,t,b,dn,bn;
	char dname[3],pathname[20];
	i=2;/* i=��Ŀ¼����ʼ�̿��*/
	/* ��name�ֳ������֣�Ŀ¼·��pathname��Ŀ¼��dname*/
	for(j=0;name[j]!='\0';j++)/*�������һ����/��*/
		if(name[j]=='/')s=j;
		/*����Ŀ¼·��*/
		for(j=0;j<s;j++)
			pathname[j]=name[j];
		pathname[j]='\0';
		/*����Ŀ¼��*/
		for(k=0,j=s+1;name[j]!='\0'&&k<3&&name[j]!='.';j++,k++)
			dname[k]=name[j];
		if(k==0)
		{
			printf("�����ļ�����Ŀ¼��\n");
			return(false);
		}
		for(;k<3;k++)
			dname[k]=' ';
		if((d=search(pathname,8,&dn,&bn))==false)/*�ҵ�Ŀ¼·��*/
		{
			printf("Ŀ¼�����ڣ����ܽ���\n");
			return(false);
		}
		b=-1;
		/*ȷ�ϸ�Ŀ¼�����ڵ�ͬʱ���ҿ�Ŀ¼��*/
		fseek(fc,d*64L,SEEK_SET);
		fread(buffer2,64L,1,fc);/*����d�̿������*/
		for(t=0;t<8;t++)
		{
			if(buffer2[t].name[0]==dname[0]&&buffer2[t].name[1]==dname[1]
				&&buffer2[t].name[2]==dname[2]&&buffer2[t].attribute==8)
			{ 
				b=t;
			}
		}
		/*��дĿ¼��*/
		
		buffer2[b].name[0]='$';
		fseek(fc,d*64L,SEEK_SET);
		fwrite(buffer2,64L,1,fc);
		printf("ɾ��Ŀ¼�ɹ�!\n");
		return 1;
}
/*ɾ��Ŀ¼����*/



/*��ʾĿ¼����*/
dir(char name[])
{
	int dnum,t,dn,bn;
	if((dnum=search(name,8,&dn,&bn))==false)/*�ҵ�Ŀ¼·�������ظ�Ŀ¼���ڿ��dn���̿�������bn*/
	{
		printf("Ŀ¼������\n");
		return(false);
	}
	printf("���� ��չ�� ��ʼ�̿� ����\n");
	/*��ʾĿ¼����*/
	fseek(fc,dnum*64L,SEEK_SET);
	fread(buffer2,64L,1,fc);
	for(t=0;t<8;t++)/*��ʾ���̿���Ŀ¼�������*/
		if(buffer2[t].name[0]!='$')
			printf("%c%c%c   %c%c%c  %4d  %7d\n", buffer2[t].name[0], 
			buffer2[t].name[1],
			buffer2[t].name[2], buffer2[t].type[0], 
			buffer2[t].type[1],buffer2[t].type[2], buffer2[t].address, 
			buffer2[t].length);
		
		return 1;
}
/*��ʾĿ¼��������*/

/*��ʾ�ļ�����*/
typefile(char name[])
{
	int dnum,dn,bn,t;
	if((dnum=search(name,1,&dn,&bn))==false)
	{
		printf("�ļ�������\n");
		return(false);
	}
	if(sopen(name)!=-1)
	{
		printf("���ļ��򿪣�������ʾ\n");
		return(false);
	}
	while(dnum!='\xff')
	{
		fseek(fc,dnum*64L,SEEK_SET);
		fread(buffer1,64,1,fc);/*��һ���̿鵽����*/
		for(t=0;t<64&&buffer1[t]!='#';t++)/*��ʾ����������*/
			putchar(buffer1[t]);
		printf("\n");
		/*�����һ���̿�*/
		fseek(fc, dnum/64*64L, SEEK_SET);
		fread(buffer1,64,1,fc);
		dnum=buffer1[dnum%64];
	}
	return 1;
}
/*��ʾ�ļ���������*/


change(char name[],int attribute)
/*�ı��ļ�name������Ϊattribute*/
{
	int dnum,bnum;
	if(search(name,1,&dnum,&bnum)==false)/*�����ļ�Ŀ¼*/
	{
		printf("�ļ�������\n");
		return(false);
	}
	if(sopen(name)!=-1)
	{
		printf("���ļ��򿪣����ܸı��ļ�����\n");
		return(false);
	}
	fseek(fc,dnum*64L,SEEK_SET);
	fread(buffer2,64,1,fc);/*������Ŀ¼�����̿�*/
	buffer2[bnum].attribute=attribute;/*�޸�����*/
	fseek(fc,dnum*64L,SEEK_SET);
	fwrite(buffer2,64,1,fc);/*д�ش���*/
    return 1;
}/*�ı��ļ����Ժ�������*/



main( )
{
	char name[20];
	int attribute,type,length,i,a;
	char buffer[64];
	if((fc=fopen("c:\\c.txt","r+"))==NULL)
	{
		/*�����ļ���ģ�����*/
		/*��ʼ���Ѵ��ļ���*/
		/*��ʼ������*/
		/*��ʼ���ļ������*/
		
		if((fc=fopen("c:\\c.txt","w+"))==NULL)
		{
			printf("�޷����ļ�\n");
			exit(0);
		}
		buffer1[0]=buffer1[1]=buffer1[2]=255;/*���̵�0��1����FAT����2���Ÿ�Ŀ¼*/
		for(i=3;i<64;i++)
			buffer1[i]=0;
		fwrite(buffer1,64L,1,fc);
		
		for(i=0;i<64;i++)
			buffer1[i]=0;
		fwrite(buffer1,64L,1,fc);
		/*��ʼ����Ŀ¼*/
		for(i=0;i<8;i++)
			buffer2[i].name[0]='$';/*��Ŀ¼��ĵ�һ���ַ�Ϊ��$����ʾ��Ŀ¼��Ϊ��*/
		fwrite(buffer2,64L,1,fc);
		/*��ʼ���Ѵ��ļ���*/
	}
	while(1)
	{
		printf("�ļ�ϵͳģ�����");
		printf("\n 1 - �����ļ�\n");
		printf(" 2 - ���ļ�\n");
		printf(" 3 - ���ļ�\n");
		printf(" 4 - д�ļ�\n");
		printf(" 5 - �ر��ļ�\n");
		printf(" 6 - ɾ���ļ�\n");
		printf(" 7 - ����Ŀ¼\n");
		printf(" 8 - ɾ��Ŀ¼\n");
		printf(" 9 - ��ʾĿ¼����\n");
		printf(" 10 -��ʾ�ļ�����\n");
		printf(" 11 -�����ļ�����\n");
		printf(" 12 -��ʽ���ļ�ϵͳ\n");
		printf(" 0 - ����\n");
		printf(" ѡ�����0~12��:");
		scanf("%d",&a);
		switch(a)
		{
		case 0: /*a=0�������*/
			fclose(fc);
			exit(0);
		case 1: /*a=1�����ļ�*/
			printf("�����ļ�·����:");
			scanf("%s",name);
			create_file(name); /*�����ļ�*/
			break;
		case 2: /*a=2���ļ�*/
			printf("�����ļ�·�����Ͳ������ͣ�0-���ļ���1-д�ļ���:");
			scanf("%s%d",name,&type);
			open_file(name,type); /*���ļ�*/
			break;
		case 3: /*a=3���ļ�*/
			printf("�����ļ�·�����Ͷ�����");
			scanf("%s%d",name,&length);
			read_file(name,length); /*���ļ�*/
			break;
		case 4: /*a=4д�ļ�*/
			printf("�����ļ�·����:");
			scanf("%s",name);
			printf("����д�����ݺͺ�д����");
			scanf("%s%d",buffer,&length);
			write_file(name,buffer,length); /*д�ļ�*/
			break;
		case 5: /*a=5�ر��ļ�*/
			printf("�����ļ�·����");
			scanf("%s",name);
			close_file(name); /*�ر��ļ�*/
			break;
		case 6: /*a=6ɾ���ļ�*/
			printf("�����ļ�·����");
			scanf("%s",name);
			delete_file(name); /*ɾ���ļ�*/
			break;
		case 7: /*a=7����Ŀ¼*/
			printf("����Ŀ¼·����");
			scanf("%s",name);
			md(name); /*����Ŀ¼*/
			break;
		case 8: /*a=8ɾ��Ŀ¼*/
			printf("����ɾ��Ŀ¼·����");
			scanf("%s",name);
			rd(name); /*��ʾĿ¼*/
			break;
		case 9: /*a=9��ʾĿ¼*/
			printf("����Ŀ¼·����");
			scanf("%s",name);
			dir(name); /*��ʾĿ¼*/
			break;
		case 10: /*a=10��ʾ�ļ�*/
			printf("�����ļ�·����");
			scanf("%s",name);
			typefile(name); /*��ʾ�ļ�*/
			break;
		case 11:/* a=11�ı��ļ����� */
			printf("�����ļ�·�������ļ����ԣ�1-ֻ���ļ���4-��д�ļ�����");
			scanf("%s%d",name,&attribute);
			change(name,attribute);
			break;
        case 12:/* a=12 ��ʽ�� */
			if((fc=fopen("c:\\c.txt","w+"))==NULL)
			{
				printf("�޷����ļ�\n");
				exit(0);
			}
			buffer1[0]=buffer1[1]=buffer1[2]=255;/*���̵�0��1����FAT����2���Ÿ�Ŀ¼*/
			for(i=3;i<64;i++)
				buffer1[i]=0;
			fwrite(buffer1,64L,1,fc);
			for(i=0;i<64;i++)
				buffer1[i]=0;
			fwrite(buffer1,64L,1,fc);
			/*��ʼ����Ŀ¼*/
			for(i=0;i<8;i++)
				buffer2[i].name[0]='$';/*��Ŀ¼��ĵ�һ���ַ�Ϊ��$����ʾ��Ŀ¼��Ϊ��*/
			fwrite(buffer2,64L,1,fc);
			printf("��ʽ���ɹ�! \n ");			
		}/* switch */
	}/* while */
	return 1;
}/*main( )����*/
