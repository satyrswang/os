#include "FileSystem.h"
Content content;
Pointer ponter;                     
OFILE ofile;
OpenFile openfile;

//文件操作
/*查找路径名为name的文件或目录,返回该目录的起始盘块号 */
search(char name[],int flag,int *dnum,int *bnum)/*flag=8表示查找目录,否则为文件*//*返回找到文件或目录的目录项的位置：盘块dnum中第bnum项*/
{
	int k,i,s,j,last=0;
	char pna[3],type[3];
	if((strcmp(name,"")==0)||(strcmp(name,"/")==0))/*根目录*/
		return(2);
	k=0;
	if(name[0]=='/')k=1;
	i=2; /*i=根目录的起始盘块号*/
	while(last!=1)
	{
		/*pna=从name中分离出"/"后一个目录名（或文件名）*/
		for(s=0;name[k]!='.'&&name[k]!='/'&&s<3&&name[k]!='\0';s++,k++)
			pna[s]=name[k];
		for(;s<3;s++)/*用空格补全名字长度*/
			pna[s]=' ';
		while(name[k]!='.'&&name[k]!='\0'&&name[k]!='/')/*除去多余字符*/
			k++;
		type[0]=type[1]=' ';
		if(name[k]=='.')/*取文件类型名type*/
			if(flag==8)
			{
				printf("目录不应该有有类型名,查找失败\n");
				return(false);
			}
			else
			{/*文件遇到类型名认为结束,后面的字符作废*/
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
					printf("文件名错误\n");
					return(false);
				}
				last=1;
			}
			else
				if(name[k]!='\0')
					k++;
				if(name[k]=='\0')
					last=1;
				/*查找目录且名字等于pna的目录项*/
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
                        if(j<8)/*找到该目录或文件*/
							if(last==1)/*查找结束*/
							{
								*dnum=i;
								*bnum=j;
								return(buffer2[j].address);
							}
							else/*查找还未结束*/
								i=buffer2[j].address;/*读取下一个盘块*/
							else 
								return(false);
	}/*while 查找结束*/
	return 1;
}/*search()结束*/

/*分配一个磁盘块，返回块号*/
allocate( )
{
	int i;
	fseek(fc,0,SEEK_SET); /*将模拟磁盘的文件指针移至模拟磁盘FAT表*/
	fread(buffer1,64L,1,fc);/*将FAT表中第一个磁盘块读入模拟缓冲buffer1中*/
	for(i=3;i<63;i++)
		if(buffer1[i]==0)
		{ /*FAT中的第i项为0,分配第i块磁盘块,修改FAT表,并且写回磁盘*/
			buffer1[i]=255;
			fseek(fc,0,SEEK_SET);
			fwrite (buffer1,64L,1,fc);
			return(i); /*返回磁盘号*/
		}
		fread(buffer1,64L,1,fc);/*将FAT表中第二个磁盘块读入模拟缓冲buffer1中*/
		for(i=0;i<63;i++)
			if(buffer1[i]==0)
			{/*FAT中的第i项为0,分配第i＋64块磁盘块,修改FAT表,并且写回磁盘*/
				buffer1[i]=255;
				fseek(fc,-64L,SEEK_CUR);
				fwrite(buffer1,64L,1,fc);
				return(i+64); /*返回磁盘号*/
			}
			printf("已经没有磁盘空间\n");
			return(false);
}
/*分配磁盘块函数结束*/  

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


/*在已打开文件表中查找文件name*/
sopen(char name[])
{
	int i;
	i=0;
	while(i<openfile.length&&strcmp(openfile.file[i].name,name)!=0)/*依次查找已打开文件表*/
		i++;
	if(i>=openfile.length)
		return(-1);
	return(i);
}/*查找sopen函数结束*/


void dopen(char name[]) /*在已打开文件表中删除文件name*/
{
	int i;
	i=sopen(name);
	if(i==-1)
		printf("文件未打开\n");
	else
	{
		copen(&openfile.file[i],&openfile.file[openfile.length-1]);
		openfile.length--;
	}
}/*删除函数结束*/

/*在已打开文件表中插入文件name*/
iopen(OFILE *x)
{
	int i;
	i=sopen(x->name);
	if(i!=-1)
	{
		printf("文件已经打开\n");
		return(false);
	}
	else if(openfile.length==n)
	{
		printf("已打开文件表已满\n");
		return(false);
	}
	else
	{
		copen(&openfile.file[openfile.length],x);
		openfile.length++;
		return(true);
	}
}
/*填写已打开文件表函数结束*/


/*建立文件函数，路径名name,文件属性attribute*/
create_file(char name[])
{
	int i,j,k,s,d,t,b,dd,dn,bn;
	int attribute=4;
	char dname[3],tname[2],pathname[20];
	OFILE x;
	if(attribute%2==1)
	{
		printf("只读文件,无法写,不能建立\n");
		return(false);
	}
	if(openfile.length==n)
	{
		printf("已打开表已满,不能建立\n");
		return(false);
	}
	/* 将name分成两部分，目录路径pathname和目录名dname*/
	for(j=0;name[j]!='\0';j++) /*查找最后一个“/”*/
		if(name[j]=='/')
			s=j;
		/*分离目录路径*/
		for(j=0;j<s;j++)
			pathname[j]=name[j];
		pathname[j]='\0';
		/*分离文件名*/
		for(k=0,j=s+1;name[j]!='\0'&&k<3&&name[j]!='.';j++,k++)
			dname[k]=name[j];
		if(k==0)
		{
			printf("错误文件名或目录名\n");
			return(false);
		}
		for(;k<3;k++)
			dname[k]=' ';
		k=0;
		
		if(name[j++]=='.')/*分离类型名*/
		{
			for(;name[j]!='\0'&&k<5&&name[j]!='.';j++,k++)
				tname[k]=name[j];
		}
		
		for(;k<3;k++) 
			tname[k]=' ';
		if((d=search(pathname,8,&dn,&bn))==false)/*找到目录路径，返回该目录所在块号dn和项数bn*/
		{
			printf("目录不存在，不能建立");
			return(false);
		}
		/*确认该目录不存在的同时查找空目录项*/
		b=-1;
		
		fseek(fc,d*64L,SEEK_SET);
		fread(buffer2,64L,1,fc); /*读出dnum盘块的内容*/
		for(t=0;t<8;t++)
		{
			if(buffer2[t].name[0]==dname[0]&&buffer2[t].name[1]==dname[1]&&buffer2[t].name[2]==dname[2]
				&&buffer2[t].type[0]==tname[0]&&buffer2[t].type[1]==tname[1]&&buffer2[t].type[2]==tname[2])
			{   /*找到名字dname的文件，建立失败*/
				printf("文件已经存在，不能建立\n");
				return(false);
			}
			if(buffer2[t].name[0]=='$'&&b==-1)
				b=t;
		}/*for*/
		
		if(b==-1)/*没有空目录项,建立失败*/
		{
			printf("目录无空间\n");
			return(false);
		}
		if((dd=allocate( ))==false)/*分配给建立目录的磁盘盘块dd*/
		{
			printf("建立文件失败\n");
			return(false);
		}
		/*填写目录项*/
		for(i=0;i<3;i++)
			buffer2[b].name[i]=dname[i];
		for(i=0;i<3;i++)
			buffer2[b].type[i]=tname[i];
		buffer2[b].attribute=attribute;
		buffer2[b].address=dd;
		buffer2[b].length=0;
		fseek(fc,d*64L,SEEK_SET);
		fwrite(buffer2,64L,1,fc);
		/*填写已打开文件表*/
		strcpy(x.name,name);
		x.attribute=attribute;
		x.number=dd;
		x.length=0;
		x.flag=1;
		x.read.dnum=x.write.dnum=dd;
		x.read.bnum=x.write.bnum=0;
		printf("文件创建成功 \n");
		iopen(&x);
		return 1;
		
}/*建立文件结束*/

/*打开文件函数*/
open_file(char name[],int attribute)
{
	OFILE x;
	int dnum,bnum,last,i,d;
	if((d=search(name,4,&dnum,&bnum))==false)
	{
		printf("文件不存在，打开操作失败\n");
		return(false);
	}
	fseek(fc,dnum*64L,SEEK_SET);/*读出对应目录项*/
	fread(buffer2,64,1,fc);
	if((buffer2[bnum].attribute%2==1)&& 
		attribute==1)/*对只读文件要求写*/
	{
		printf("文件不能写，打开失败");
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
		while(d!='\xff')/*寻找文件末尾*/
		{
			fseek(fc, d/64*64L, SEEK_SET);
			fread(buffer1,64L,1,fc);/*读出dnum项所在FAT*/
			last=d;
			d=buffer1[d%64];/*读出dnum块下一块内容赋给dnum*/
		}/*while*/
		x.write.dnum=last;/*填写写指针*/
		fseek(fc, last*64L, SEEK_SET);
		fread(buffer1,64L,1,fc);
		for(i=0;i<64&&buffer1[i]!='#';i++);
		x.write.bnum=i;
		x.length=(buffer2[bnum].length-1)*64+i;
	}
	iopen(&x);/*填写已打开文件表*/
	return 1;
}
/*文件打开结束*/


/*关闭文件函数*/
close_file(char name[])
{
	int i,dnum,bnum;
	if((i=sopen(name))==-1)
	{
		printf("打开的文件中没有该文件,关闭失败\n");
		return(false);
	}
	if(openfile.file[i].flag==1)/*写文件的追加文件结束符*/
	{
		fseek(fc,openfile.file[i].write.dnum*64L, SEEK_SET);
		fread(buffer1,64,1,fc);
		buffer1[openfile.file[i].write.bnum]='#';
		fseek(fc,openfile.file[i].write.dnum*64L, SEEK_SET);
		fwrite(buffer1,64,1,fc);
		fputc('#',fc);
		search(name,4,&dnum,&bnum);/*查找该文件目录位置*/
		/*修改目录中的文件长度*/
		fseek(fc,dnum*64L, SEEK_SET);
		fread(buffer2,64,1,fc);
		buffer2[bnum].length=openfile.file[i].length/64+1;
		fseek(fc, dnum*64L, SEEK_SET);
		fwrite(buffer2,64,1,fc);
	}
	/*在已打开文件表中删除该文件的登记项*/
	if(openfile.length>1)
		copen(&openfile.file[i],&openfile.file[openfile.length-1]);
	openfile.length--;
	printf("文件关闭\n");
	return 1;
}

/*读文件函数，文件路径名name，读取长度length*/
read_file(char name[],int length)
{
	int i,t;
	if((i=sopen(name))==-1)
	{
		printf("文件没有打开或不存在\n");
		return(false);
	}
	if(openfile.file[i].flag==1)
	{
		printf("文件以写方式打开，不能读! 请先关闭文件，以读的方式打开!\n");
		return(false);
	}
	t=0;
	fseek(fc,openfile.file[i].read.dnum*64L,SEEK_SET);
	fread(buffer1,64,1,fc);
	while(t<length&&buffer1[openfile.file[i].read.bnum]!='#')
	{
		putchar(buffer1[openfile.file[i].read.bnum]);/*读出一个字符（这里是在屏幕上显示）*/
		if((t+1)%64==0)putchar('\n');
		/*修改读指针*/
		openfile.file[i].read.bnum++;
		if(openfile.file[i].read.bnum>=64)/*一块读完，读取下一个盘块*/
		{
			fseek(fc,openfile.file[i].read.dnum/64*64, SEEK_SET);
			fread(buffer1,64,1,fc);
			openfile.file[i].read.dnum=buffer1[openfile.file[i].read.dnum%64];/*修改读指针*/
			openfile.file[i].read.bnum=0;
			fseek(fc,openfile.file[i].read.dnum*64L,SEEK_SET);
			fread(buffer1,64,1,fc);/*读取下一个*/
		}
		t++;
	}
	return 1;
}
/*读函数结束*/

/*写文件函数*/
write_file(char name[],char buff[],int length)/*文件路径名 存放准备写入磁盘的内容 写入内容的长度*/
{
	int i,t,dd;
	int d,dn,bn;
	
	if((i=sopen(name))==-1)/*文件不存在，无法写*/
	{
		printf("文件没有打开或不存在\n");
		return(false);
	}
	if(openfile.file[i].flag==0)
	{
		printf("文件以读方式打开，不能写\n");
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
			fwrite(buffer1,64,1,fc);/*一块写完，写回磁盘*/
			if((dd=allocate())==false)
			{
				openfile.file[i].write.bnum--;
				openfile.file[i].length--;
				printf("无磁盘空间,部分信息丢失，写失败\n");
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
	fwrite(buffer1,64,1,fc);/*一块写完，写回磁盘*/
	
    if((d=search(name,4,&dn,&bn))==false)/*找到目录路径，返回该目录所在块号dn和项数bn*/
	{
		printf("目录不存在，不能建立");
		return(false);
	}
	
	fseek(fc,dn*64L,SEEK_SET);
	fread(buffer2,64L,1,fc); /*读出dnum盘块的内容*/
	
	buffer2[bn].length=length;
	fseek(fc,dn*64L,SEEK_SET);
	fwrite(buffer2,64L,1,fc);	
	   return 1;
}/*写函数结束*/



/*删除文件*/
delete_file(char name[])
{
	int dnum,bnum,t;
	if((t=search(name,4,&dnum,&bnum))==false)
	{
		printf("文件不存在\n");
		return(false);
	}
	if(sopen(name)!=-1)
	{
		printf("该文件打开，不能删除\n");
		return(false);
	}
	fseek(fc,dnum*64L, SEEK_SET);
	fread(buffer2,64,1,fc);
	buffer2[bnum].name[0]='$';/*将该文件的目录置成空目录*/
	fseek(fc,dnum*64L, SEEK_SET);
	fwrite(buffer2,64,1,fc);
	while(t!='\xff')/*通过FAT查找每一个盘块号，并依次删除*/
	{
		dnum=t;
		fseek(fc, dnum/64*64, SEEK_SET);
		fread(buffer1,64,1,fc);
		t=buffer1[dnum%64];
		buffer1[dnum%64]=0;
		fseek(fc, dnum/64*64L, SEEK_SET);
		fwrite(buffer1,64,1,fc);
	}
	printf("文件删除成功!\n");
	return 1;
}/*文件删除结束*/


/*建立目录函数，目录路径名name*/
md(char name[])
{
	int i,j,k,s,d,t,b,dd,dn,bn;
	char dname[3],pathname[20];
	i=2;/* i=根目录的起始盘块号*/
	/* 将name分成两部分，目录路径pathname和目录名dname*/
	for(j=0;name[j]!='\0';j++)/*查找最后一个“/”*/
		if(name[j]=='/')s=j;
		/*分离目录路径*/
		for(j=0;j<s;j++)
			pathname[j]=name[j];
		pathname[j]='\0';
		/*分离目录名*/
		for(k=0,j=s+1;name[j]!='\0'&&k<3&&name[j]!='.';j++,k++)
			dname[k]=name[j];
		if(k==0)
		{
			printf("错误文件名或目录名\n");
			return(false);
		}
		for(;k<3;k++)
			dname[k]=' ';
		if((d=search(pathname,8,&dn,&bn))==false)/*找到目录路径*/
		{
			printf("目录不存在，不能建立\n");
			return(false);
		}
		b=-1;
		/*确认该目录不存在的同时查找空目录项*/
		fseek(fc,d*64L,SEEK_SET);
		fread(buffer2,64L,1,fc);/*读出d盘块的内容*/
		for(t=0;t<8;t++)
		{
			if(buffer2[t].name[0]==dname[0]&&buffer2[t].name[1]==dname[1]
				&&buffer2[t].name[2]==dname[2]&&buffer2[t].attribute==8)
			{/*找到名字dname的目录，建立失败*/
				printf("目录已经存在，不能建立\n");
				return(false);
			}
			if(buffer2[t].name[0]=='$'&&b==-1)
				b=t;
		}/*for*/
		if(b==-1)/*没有空目录项， 不能建立*/
		{
			printf("目录无空间\n");
			return(false);
		}
		if((dd=allocate( ))==false)/*分配给建立目录的磁盘盘块dd*/
		{
			printf("目录不能建立\n");
			return(false);
		}
		/*填写目录项*/
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
		/*分给新建目录的盘块初始化*/
		for(t=0;t<8;t++)
			buffer2[t].name[0]='$';
		fseek(fc, dd*64L, SEEK_SET);
		fwrite(buffer2,64L,1,fc);
        printf("目录建立成功!\n");
		return 1;
}
/*建立目录结束*/


/*删除目录函数，目录路径名name*/
rd(char name[])
{
	int i,j,k,s,d,t,b,dn,bn;
	char dname[3],pathname[20];
	i=2;/* i=根目录的起始盘块号*/
	/* 将name分成两部分，目录路径pathname和目录名dname*/
	for(j=0;name[j]!='\0';j++)/*查找最后一个“/”*/
		if(name[j]=='/')s=j;
		/*分离目录路径*/
		for(j=0;j<s;j++)
			pathname[j]=name[j];
		pathname[j]='\0';
		/*分离目录名*/
		for(k=0,j=s+1;name[j]!='\0'&&k<3&&name[j]!='.';j++,k++)
			dname[k]=name[j];
		if(k==0)
		{
			printf("错误文件名或目录名\n");
			return(false);
		}
		for(;k<3;k++)
			dname[k]=' ';
		if((d=search(pathname,8,&dn,&bn))==false)/*找到目录路径*/
		{
			printf("目录不存在，不能建立\n");
			return(false);
		}
		b=-1;
		/*确认该目录不存在的同时查找空目录项*/
		fseek(fc,d*64L,SEEK_SET);
		fread(buffer2,64L,1,fc);/*读出d盘块的内容*/
		for(t=0;t<8;t++)
		{
			if(buffer2[t].name[0]==dname[0]&&buffer2[t].name[1]==dname[1]
				&&buffer2[t].name[2]==dname[2]&&buffer2[t].attribute==8)
			{ 
				b=t;
			}
		}
		/*填写目录项*/
		
		buffer2[b].name[0]='$';
		fseek(fc,d*64L,SEEK_SET);
		fwrite(buffer2,64L,1,fc);
		printf("删除目录成功!\n");
		return 1;
}
/*删除目录结束*/



/*显示目录内容*/
dir(char name[])
{
	int dnum,t,dn,bn;
	if((dnum=search(name,8,&dn,&bn))==false)/*找到目录路径，返回该目录所在块号dn和盘块内项数bn*/
	{
		printf("目录不存在\n");
		return(false);
	}
	printf("名称 扩展名 起始盘块 长度\n");
	/*显示目录内容*/
	fseek(fc,dnum*64L,SEEK_SET);
	fread(buffer2,64L,1,fc);
	for(t=0;t<8;t++)/*显示该盘块中目录项的内容*/
		if(buffer2[t].name[0]!='$')
			printf("%c%c%c   %c%c%c  %4d  %7d\n", buffer2[t].name[0], 
			buffer2[t].name[1],
			buffer2[t].name[2], buffer2[t].type[0], 
			buffer2[t].type[1],buffer2[t].type[2], buffer2[t].address, 
			buffer2[t].length);
		
		return 1;
}
/*显示目录函数结束*/

/*显示文件内容*/
typefile(char name[])
{
	int dnum,dn,bn,t;
	if((dnum=search(name,1,&dn,&bn))==false)
	{
		printf("文件不存在\n");
		return(false);
	}
	if(sopen(name)!=-1)
	{
		printf("该文件打开，不能显示\n");
		return(false);
	}
	while(dnum!='\xff')
	{
		fseek(fc,dnum*64L,SEEK_SET);
		fread(buffer1,64,1,fc);/*读一个盘块到缓冲*/
		for(t=0;t<64&&buffer1[t]!='#';t++)/*显示缓冲中内容*/
			putchar(buffer1[t]);
		printf("\n");
		/*获得下一个盘块*/
		fseek(fc, dnum/64*64L, SEEK_SET);
		fread(buffer1,64,1,fc);
		dnum=buffer1[dnum%64];
	}
	return 1;
}
/*显示文件函数结束*/


change(char name[],int attribute)
/*改变文件name的属性为attribute*/
{
	int dnum,bnum;
	if(search(name,1,&dnum,&bnum)==false)/*查找文件目录*/
	{
		printf("文件不存在\n");
		return(false);
	}
	if(sopen(name)!=-1)
	{
		printf("该文件打开，不能改变文件属性\n");
		return(false);
	}
	fseek(fc,dnum*64L,SEEK_SET);
	fread(buffer2,64,1,fc);/*读出该目录所在盘块*/
	buffer2[bnum].attribute=attribute;/*修改属性*/
	fseek(fc,dnum*64L,SEEK_SET);
	fwrite(buffer2,64,1,fc);/*写回磁盘*/
    return 1;
}/*改变文件属性函数结束*/



main( )
{
	char name[20];
	int attribute,type,length,i,a;
	char buffer[64];
	if((fc=fopen("c:\\c.txt","r+"))==NULL)
	{
		/*建立文件，模拟磁盘*/
		/*初始化已打开文件表*/
		/*初始化磁盘*/
		/*初始化文件分配表*/
		
		if((fc=fopen("c:\\c.txt","w+"))==NULL)
		{
			printf("无法打开文件\n");
			exit(0);
		}
		buffer1[0]=buffer1[1]=buffer1[2]=255;/*磁盘第0、1块存放FAT表，第2块存放根目录*/
		for(i=3;i<64;i++)
			buffer1[i]=0;
		fwrite(buffer1,64L,1,fc);
		
		for(i=0;i<64;i++)
			buffer1[i]=0;
		fwrite(buffer1,64L,1,fc);
		/*初始化根目录*/
		for(i=0;i<8;i++)
			buffer2[i].name[0]='$';/*若目录项的第一个字符为“$”表示该目录项为空*/
		fwrite(buffer2,64L,1,fc);
		/*初始化已打开文件表*/
	}
	while(1)
	{
		printf("文件系统模拟程序");
		printf("\n 1 - 建立文件\n");
		printf(" 2 - 打开文件\n");
		printf(" 3 - 读文件\n");
		printf(" 4 - 写文件\n");
		printf(" 5 - 关闭文件\n");
		printf(" 6 - 删除文件\n");
		printf(" 7 - 建立目录\n");
		printf(" 8 - 删除目录\n");
		printf(" 9 - 显示目录内容\n");
		printf(" 10 -显示文件内容\n");
		printf(" 11 -更改文件属性\n");
		printf(" 12 -格式化文件系统\n");
		printf(" 0 - 结束\n");
		printf(" 选择功能项（0~12）:");
		scanf("%d",&a);
		switch(a)
		{
		case 0: /*a=0程序结束*/
			fclose(fc);
			exit(0);
		case 1: /*a=1建立文件*/
			printf("输入文件路径名:");
			scanf("%s",name);
			create_file(name); /*建立文件*/
			break;
		case 2: /*a=2打开文件*/
			printf("输入文件路径名和操作类型（0-读文件，1-写文件）:");
			scanf("%s%d",name,&type);
			open_file(name,type); /*打开文件*/
			break;
		case 3: /*a=3读文件*/
			printf("输入文件路径名和读长度");
			scanf("%s%d",name,&length);
			read_file(name,length); /*读文件*/
			break;
		case 4: /*a=4写文件*/
			printf("输入文件路径名:");
			scanf("%s",name);
			printf("输入写的内容和和写长度");
			scanf("%s%d",buffer,&length);
			write_file(name,buffer,length); /*写文件*/
			break;
		case 5: /*a=5关闭文件*/
			printf("输入文件路径名");
			scanf("%s",name);
			close_file(name); /*关闭文件*/
			break;
		case 6: /*a=6删除文件*/
			printf("输入文件路径名");
			scanf("%s",name);
			delete_file(name); /*删除文件*/
			break;
		case 7: /*a=7建立目录*/
			printf("输入目录路径名");
			scanf("%s",name);
			md(name); /*建立目录*/
			break;
		case 8: /*a=8删除目录*/
			printf("输入删除目录路径名");
			scanf("%s",name);
			rd(name); /*显示目录*/
			break;
		case 9: /*a=9显示目录*/
			printf("输入目录路径名");
			scanf("%s",name);
			dir(name); /*显示目录*/
			break;
		case 10: /*a=10显示文件*/
			printf("输入文件路径名");
			scanf("%s",name);
			typefile(name); /*显示文件*/
			break;
		case 11:/* a=11改变文件属性 */
			printf("输入文件路径名和文件属性（1-只读文件，4-读写文件）：");
			scanf("%s%d",name,&attribute);
			change(name,attribute);
			break;
        case 12:/* a=12 格式化 */
			if((fc=fopen("c:\\c.txt","w+"))==NULL)
			{
				printf("无法打开文件\n");
				exit(0);
			}
			buffer1[0]=buffer1[1]=buffer1[2]=255;/*磁盘第0、1块存放FAT表，第2块存放根目录*/
			for(i=3;i<64;i++)
				buffer1[i]=0;
			fwrite(buffer1,64L,1,fc);
			for(i=0;i<64;i++)
				buffer1[i]=0;
			fwrite(buffer1,64L,1,fc);
			/*初始化根目录*/
			for(i=0;i<8;i++)
				buffer2[i].name[0]='$';/*若目录项的第一个字符为“$”表示该目录项为空*/
			fwrite(buffer2,64L,1,fc);
			printf("格式化成功! \n ");			
		}/* switch */
	}/* while */
	return 1;
}/*main( )结束*/
