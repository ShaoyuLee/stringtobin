#include <stdio.h>

#include <stdlib.h>
#include <stdint.h>


#define MAXLEN (1024*1024)//1M
#ifdef _WIN32
#pragma warning (disable : 4996)
#endif
uint32_t stringtohex(uint8_t *source, uint32_t source_size, uint8_t *hex_buffer, uint32_t hex_buffer_size)
{
	uint32_t get_count = 0;
	uint32_t hex_count = 0;
	uint8_t half_byte = 0;

	//log("s=%s\r\n", s);
	//log("s_size=%d\r\n", s_size);
	for (uint32_t i = 0; i < source_size; i++)
	{
		//log("i=%d ",i);
		//log("s=%c ",s[i]);
		if ((source[i] == '0')
			&& ((source[i + 1] == 'x') || (source[i + 1] == 'X')))
		{
			i++;
			//log("s+1=%c\r\n ",s[1]);
			continue;
		}
		if (source[i] >= '0' && source[i] <= '9')
		{
			half_byte = (source[i] - '0');
		}
		else if (source[i] >= 'A'&&source[i] <= 'F')
		{
			half_byte = (source[i] - 'A') + 10;
		}
		else if (source[i] >= 'a'&&source[i] <= 'f')
		{
			half_byte = (source[i] - 'a') + 10;
		}
		else{
			continue;
		}

		//log("hf_byte=%x ", half_byte);
		if (0 == hex_count)
		{
			hex_buffer[get_count] = half_byte << 4;
			hex_count++;
		}
		else
		{
			hex_buffer[get_count] |= half_byte;
			//log("byte=%x ", hex[get_count]);
			hex_count = 0;
			get_count++;
			if (get_count>hex_buffer_size){
				break;
			}
		}

		//log("\r\n");
	}

	return get_count;
}

int main(int argc, char *argv[])

{

	uint32_t file_size = 0x12345678;
	uint32_t version = 0x0714;
	char tempfile[] ="temp.bin";
	if (argc != 3)

	{

		printf("\r\nInput file num is %d, Error file input\r\nexpmple£ºstr2bin  source.txt target.bin\r\n", argc);

		system("PAUSE");
		exit(1);

	}



	FILE * outfile, *infile, *temp;


	if (remove(tempfile) == 0)
	{
		printf("Removed %s.\r\n", tempfile);
	}
	else{
		//printf("Remove %s ", argv[2]);
		//perror("error");
	}
	temp = fopen(tempfile, "wb+");
	if (temp==NULL)
	{
		printf("temp.bin create error\r\n");
		system("PAUSE");
		return 3;
	}

	infile = fopen(argv[1], "rb");
	if (infile == NULL)

	{
		printf("%s, %s", argv[1], "no file found, exit\r\n");
		system("PAUSE");
		return 2;
	}

	if (remove(argv[2]) == 0)
	{
		printf("Removed %s.\r\n", argv[2]);
	}
	else{
		//printf("Remove %s ", argv[2]);
		//perror("error");
	}

    printf("create %s \r\n", argv[2]);
	outfile = fopen(argv[2], "wb+");
	if (infile == NULL)
	{
		printf("%s, %s", argv[2], "no file found, exit\r\n");
		system("PAUSE");
		return 2;
	}

	unsigned char *buf;
	unsigned char *hex_buf;

	buf=malloc(MAXLEN);
	hex_buf = malloc(MAXLEN);

	if (outfile == NULL || infile == NULL)

	{

		printf("%s, %s", argv[1], "no file exit\r\n");

		system("PAUSE");
		return 2;
	}



	int rc;
	int get_hex_len;
	uint8_t _0=0;
	fputs("Ginshell Co,Ltd", outfile);
	fwrite(&_0, sizeof(_0), sizeof(unsigned char), outfile);
	fputs("ble", outfile);
	fwrite(&_0, sizeof(_0), sizeof(unsigned char), outfile);
	
	while ((rc = fread(buf, sizeof(unsigned char), MAXLEN, infile)) != 0)

	{
		static count=0;
		count++;
		if (count > 100){
			printf("Too big file, abort convert!\r\n");
			break;
		}
		printf("get file size %d B\r\n", rc);
		get_hex_len=stringtohex(buf, rc, hex_buf, MAXLEN);
	    printf("get hex size %d B\r\n", get_hex_len);
		if (0 == get_hex_len)
		{
			printf("fail\r\n");
		}
		fwrite(hex_buf, sizeof(unsigned char), get_hex_len, temp);

	}

	fseek(temp, 0, SEEK_END); // seek to end of file
	file_size = ftell(temp); // get current file pointer
	printf("new file size is %d\r\n",file_size);
	fseek(temp, 0, SEEK_SET); // seek back to beginning of file

	unsigned char *buf_new;

	buf_new = malloc(file_size);
	rc = fread(buf_new, sizeof(unsigned char), file_size, temp);
	if (rc == file_size)
	{
		printf("get file ok\r\n");
	}
	else
	{
		printf("get wrong file\r\n");
	}
	fwrite(&version, sizeof(version), sizeof(unsigned char), outfile);
	fwrite(&file_size, sizeof(file_size), sizeof(unsigned char), outfile);
	fwrite(buf_new, sizeof(unsigned char), file_size, outfile);

	perror("fwrite");

	fclose(infile);
	fclose(outfile);
	fclose(temp);
	free(buf);
	free(hex_buf);
	
	system("PAUSE");

	return 0;

}
