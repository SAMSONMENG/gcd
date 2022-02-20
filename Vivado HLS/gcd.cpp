#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <math.h>
#include <ap_int.h>
#include <iostream>
#include <ap_axi_sdata.h>

using namespace std;
typedef ap_axis <32,2,5,6> int_side_ch;
void gcd(hls:: stream<int_side_ch> &inStream , hls:: stream<int_side_ch> &outStream,int gain)
{

#pragma HLS INTERFACE axis port=inStream
#pragma HLS INTERFACE axis port=outStream
#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS
#pragma HLS INTERFACE s_axilite port=gain bundle=CRTL_BUS

	int_side_ch val_in1;
	int_side_ch val_in2;
	int_side_ch val_out;

	val_in1= inStream.read();
	int x=(int)val_in1.data;
	val_in2= inStream.read();
	int y=(int)val_in2.data;
       if( x < y )
          std::swap( x, y );

       while( y > 0 )
       {
          int f = x % y;
          x = y;
          y = f;
       }
     val_out.data=x;
     outStream.write(val_out);
 }


