#include <cstdio>
#include <cstdlib>
#include <getopt.h>

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

// save ourselves an include
int64_t min(int64_t a, int64_t b) { return a < b ? a : b; }

int main(int argc, char** argv)
{
	int const BUFFER_SIZE = 1024*1024;

	char const* pathIn = nullptr;
	char const* pathOut = nullptr;
	int64_t start = 0;
	int64_t length = 0;
	bool readToEnd = true;

	while (1)
	{
		static struct option long_options[] = {
			{ "input",  required_argument, 0, 'i' },
			{ "output", required_argument, 0, 'o' },
			{ "start",  required_argument, 0, 's' },
			{ "length", required_argument, 0, 'n' },
			{ 0, 0, 0, 0 }
		};

		int option_index = 0;
		int c = getopt_long(argc, argv, "i:o:s:n:", long_options, &option_index);
		
		if (c == -1) {
			break;
		}

		switch (c) {
			case 'i': {
				pathIn = optarg;
				break;
			}
			case 'o': {
				pathOut = optarg;
				break;
			}
			case 's': {
				start = strtol(optarg, nullptr, 10);
				if (start < 0) {
					eprintf("Start offset must be greater than or equal to zero.\n");
					return 1;
				}
				break;
			}
			case 'n': {
				length = strtol(optarg, nullptr, 10);
				if (length <= 0) {
					eprintf("Length must be greater than zero.\n");
					return 1;
				}
				readToEnd = false;
				break;
			}
		}
	}

	if (!pathIn || !pathOut)
	{
		eprintf("usage:\n");
		eprintf("slice -i <infile> -o <outfile> -s <start offset> -n <length>\n");
		return 1;
	}

	FILE* fhIn = fopen(pathIn, "rb");
	if (!fhIn)
	{
		eprintf("Failed to open '%s' for reading.\n", pathIn);
		return 1;
	}

	FILE* fhOut = fopen(pathOut, "wb");
	if (!fhOut)
	{
		eprintf("Failed to open '%s' for writing.\n", pathOut);
		return 1;
	}

	fseek(fhIn, start, SEEK_SET);

	unsigned char buffer[BUFFER_SIZE];

	int64_t totalBytesCopied = 0;
	if (readToEnd)
	{
		int64_t bytesRead;
		do {
			bytesRead = fread(buffer, 1, BUFFER_SIZE, fhIn);
			if (bytesRead > 0)
			{
				fwrite(buffer, 1, bytesRead, fhOut);
				totalBytesCopied += bytesRead;
			}
		} while (bytesRead != 0);

		printf(
			"Copied %lld bytes from '%s'+0x%llx to '%s'\n",
			totalBytesCopied,
			pathIn,
			start,
			pathOut
		);
	}
	else
	{
		while (totalBytesCopied < length)
		{
			int64_t const bytesRead = fread(buffer, 1, BUFFER_SIZE, fhIn);
			if (bytesRead > 0)
			{
				int64_t const bytesToWrite = min(length - totalBytesCopied, bytesRead);
				fwrite(buffer, 1, bytesToWrite, fhOut);
				totalBytesCopied += bytesToWrite;
			}
			else
			{
				break;
			}
		}

		if (totalBytesCopied == length)
		{
			printf(
				"Copied %lld bytes from '%s'+0x%llx to '%s'\n",
				totalBytesCopied,
				pathIn,
				start,
				pathOut
			);
		}
		else
		{
			if (totalBytesCopied == 0)
			{
				eprintf(
					"Attempted to copy %lld bytes from '%s'+0x%llx to '%s', but no data was available.\n",
					length,
					pathIn,
					start,
					pathOut
				);
			}
			else
			{
				eprintf(
					"Attempted to copy %lld bytes from '%s'+0x%llx to '%s', but only %lld %s available.\n",
					length,
					pathIn,
					start,
					pathOut,
					totalBytesCopied,
					totalBytesCopied == 1 ? "byte was" : "bytes were"
				);
			}
			fclose(fhIn);
			fclose(fhOut);
			return 1;
		}
	}

	fclose(fhIn);
	fclose(fhOut);
}
