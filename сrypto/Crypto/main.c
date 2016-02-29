#include <stdlib.h>
#include <stdio.h>

void crypt(char * key, FILE * input, FILE * output)
{
	const int read_by = 1024;

	size_t key_size = 0;
	while (key[key_size] != '\0') key_size++;

	char * read = malloc(sizeof(char *) * read_by);
	size_t position = 0;
	size_t read_count = fread(read, 1, read_by, input);
	while (read_count != 0)
	{
		for (size_t i = 0; i < read_count; i++)
		{
			read[i] ^= key[position];
			position++;
			if (position >= key_size) position = 0;
		}

		fwrite(read, 1, read_count, output);

		read_count = fread(read, 1, read_by, input);
	}
}

void print_help(void)
{
	printf("key input output \t to encrypt or decrypt");
}

int main(int argc, char *argv[])
{
	if (argc < 4)
	{
		print_help();
	}
	else
	{
		FILE * input, *output;
		errno_t inErr = fopen_s(&input, argv[2], "rb");
		errno_t outErr = fopen_s(&output, argv[3], "wb");

		if (outErr || inErr)
		{
			printf("Error opening file\n");
			print_help();
		}
		else
		{
			crypt(argv[1], input, output);

			fclose(input);
			fclose(output);
		}
	}
}