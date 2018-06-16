#include<stdio.h>

int main(int argc,char *argv[])
{
	FILE *fp;
	char str[1000];
	int len;
	fp = fopen(argv[1],"r");
	fseek(fp,0L,SEEK_END);
	len = ftell(fp);
	fseek(fp,0L,SEEK_SET);
	fread(str,1,len+1,fp);
	fclose(fp);
	fp = fopen(argv[2],"w");
	fwrite(str,1,len+1,fp);
	return 0;
}
