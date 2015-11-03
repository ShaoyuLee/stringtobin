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

	if (argc != 3)

	{

		printf("\r\nInput file num is %d, Error file input\r\nexpmple£ºstr2bin  source.txt target.bin\r\n", argc);

		system("PAUSE");
		exit(1);

	}



	FILE * outfile, *infile;

	outfile = fopen(argv[2], "wb");

	infile = fopen(argv[1], "rb");

	unsigned char *buf;
	unsigned char *hex_buf;

	buf=malloc(MAXLEN);
	hex_buf = malloc(MAXLEN);

	if (outfile == NULL || infile == NULL)

	{

		printf("%s, %s", argv[1], "not exit/n");

		system("PAUSE");
		return 2;
	}



	int rc;
	int get_hex_len;

	while ((rc = fread(buf, sizeof(unsigned char), MAXLEN, infile)) != 0)

	{
		printf("get file size %d B\r\n", rc);
		get_hex_len=stringtohex(buf, rc, hex_buf, MAXLEN);
	    printf("get hex size %d B\r\n", get_hex_len);
		if (0 == get_hex_len)
		{
			printf("fail\r\n");
		}
		fwrite(hex_buf, sizeof(unsigned char), get_hex_len, outfile);

	}

	fclose(infile);

	fclose(outfile);
	free(buf);
	free(hex_buf);
	system("PAUSE");

	return 0;

}
