#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <math.h>
#include <ap_int.h>
#include <iostream>
#include <ap_axi_sdata.h>

using namespace std;

typedef ap_axis<32,2,5,6> int_side_ch;

void gcd(hls:: stream<int_side_ch> &inStream , hls:: stream<int_side_ch> &outStream,int gain);

int main()
{
	hls:: stream<int_side_ch> inputStream;
	hls:: stream<int_side_ch> outputStream;

	int_side_ch val_in;
	val_in.data=17;
	val_in.keep=1;
	val_in.strb=1;
	val_in.last=0;
	val_in.user=1;
	val_in.id=0;
	val_in.dest=0;
	inputStream<<val_in;
	val_in.data=85;
	val_in.keep=1;
	val_in.strb=1;
	val_in.last=1;
	val_in.user=1;
	val_in.id=0;
	val_in.dest=0;
	inputStream<<val_in;

    gcd(inputStream, outputStream,1);
	int_side_ch val_out;
	outputStream.read(val_out);
    cout<<" output = "<< val_out.data;
}






