# Huffman compression/decompression

Simple compression/decompression tool, uses Huffman algorithm to compress
and to decompress files.

## The compression process:
* going over the the file and reads it byte by byte, calculating how many appearances there are
for each of the 2^(byte size) different bytes possible and saves it.
* after creating the histogram of bytes it builds the tree which will be used to convert each
byte to Huffman code.
* creates a new file which will store the compressed information.
* saves the histogram (sizeof(int) * 2^(byte size)) in the beginning of the file.
* reads one byte at a time from the input file, and translates each byte to unique code (0's and 1's)
to a buffer (size of buffer is 1 byte), only after the buffer is filled the program writes the
data from buffer to the output file.

## The decompression process:
* reads the first (sizeof(int) * 2^(byte size)) and restores the histogram of the original file.
* builds the translation tree from the histogram to translate the Huffman code to the correct bytes.
* reads one byte at a time from the input file and for each byte:
    * getting one bit from the byte
    * according to the bit value (0 or 1) going left or right in the translation tree.
    * if a leaf was reached then we found a translation from code to original byte, we write the
      corresponding byte and deduce 1 from our histogram.
    * if we reached a leaf and the corresponding byte in the histogram is 0, then we just read the
    0's we filled our last byte with in the compression and the process is ended.


## how to use
to compress:
>./Huffman.exe -c [file_path]

this will generate [file_name].huf file which is the compressed file.


to decompress: 
>./Huffman.exe -d [compressed_file_path]
	
this will generate [decompressed_file] file which is the decompressed file.
