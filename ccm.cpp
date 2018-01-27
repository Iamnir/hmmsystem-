#include <iostream>
#include <climits>
#include <bitset>
#include <math.h>
#include <stdexcept>
using namespace std;


static const unsigned int bits_per_byte = CHAR_BIT; 
// number of bits per char (byte)
static const unsigned int max_hamming_bits = bits_per_byte * sizeof(long); 
// Here we get number of bits used to hold a long, or the maximum number of bits 
// we can use for our hamming code in this implementation

unsigned int getMinBits(long val) {	// Here  function getMinBits, We get the minimum number of bits need to 
                                       //express the integer val in binary (with no leading zeros)
		if (val == 0) {                  // val: the value we want to express in binary 
		return 1;                        // return: the minimum number of digits needed to express val in binary
	}else {
	    //find the index of the MSB in our long integer
		// loop: there are at most i + 1 bits needed to express val
		for(int i = max_hamming_bits - 1; i >= 0; i--) {
			if (((val >> i) & 1) == 1)
				return i + 1;
		}
	}
}  
// get the minimum number of parity bits needed for a hamming code with m data bits
// m: the number of data bits we will be using in our hamming code
// return: the minimum number of parity bits needed to create the hamming code
    unsigned int getMinParityBits(unsigned int m) {
	unsigned int r = 0;
	// if we have m data bits and r check bits, then the number of parity
	// bits we need is the smallest integer r that satisfies the condition
	// (m + r + 1) <= 2^r
	while((m + r + 1) > pow(2,r)) 
		r++;
	return r;
}

// given an integer val, create the corresponding hamming code with the bits of val
// as the 16 data bits. if val is not expressible as 1-14 digit hexadecimal
// (needs greater than 56 bits to be expressed as a binary)
// then we throw an error, because this is the most data bits that can be contained in a hamming code stored as a long
// val: an integer that needs be up to a 14-digit hexadecimal; the value expressed by the data bits
// return: the corresponding hamming code as an integer value
long hamming (long val) {		
	// get min number of bits needed to represent val in binary
	unsigned int num_data_bits = getMinBits(val);
	// get number of parity bits needed
	unsigned int num_parity_bits = getMinParityBits(num_data_bits);
	// Throw error if val is not expressible as 1-14 digit hexadecimal
	unsigned int max_data_bits = ((max_hamming_bits - num_parity_bits) / 4) * 4;
	if (num_data_bits > max_data_bits) {
		throw invalid_argument("greater than fourteen-digit hamming code");
	}
	
	// declare array bit enough for parity bits and data bits
	// initialize parity bits to -1, all others 0
	int total_bits = num_parity_bits + num_data_bits;
	int hamming_arr[total_bits];
	// loop : the first i elements of hamming_arr have been assigned to 0
	for ( int i = 0; i < total_bits; i++)
		hamming_arr[i] = 0;
	// loop : the first i parity bits have been initialized to -1 in hamming_arr
	for (int i = 0; i < num_parity_bits; i++) {
		int parity_indx = pow(2,i) - 1;
		hamming_arr[parity_indx] = -1;
	}

	// read in bits via bitwise operator, place in hamming array
	// loop: the first val_i data bits have been read in from val into hamming_arr
	int i = 0;
	int val_i = 0;
	while(i < total_bits) {
		if (hamming_arr[i] != -1){
			int bitval = (val >> (num_data_bits - val_i - 1)) & 1;
			hamming_arr[i] = bitval;
			val_i++;
		}
		i++;
	}

	// print out the data bits preceded by a tab, with parity bits filled in as spaces
	// This allows the user to directly compare the data bits alongside their final hamming code
	cout << "data bits:\t";
	for(int i = 0; i < total_bits; i++ ){
		if (hamming_arr[i] != -1) {
			cout << hamming_arr[i];
		} else {
			cout << " ";
		}
	}
	cout << endl;

	// for each parity bit, sum up the digits that it checks
	// then add 0 or 1 in the parity bit to make the sum even
	// loop :the first p parity bits have been calculated for even parity
	for(int p = 0; p < num_parity_bits; p++) {
		int curbit = pow(2,p);
		int p_sum = 0;
		for (int i = curbit; i <= total_bits; i++) {
			if ((((i-curbit)/curbit) % 2 )== 1){
				continue;
			} else {
				if (i != curbit) {
					p_sum += hamming_arr[i - 1];
				}

			}
		}
		hamming_arr[curbit-1] = (p_sum % 2 == 0) ? 0 : 1;
	}

	// convert array back into an integer
	// loop : the first i bits of hamming_arr have been added to the hamming code
	long hamming_code = 0;
	for(i = 0; i < total_bits; i++) {
		hamming_code += hamming_arr[i] * pow(2,total_bits - i - 1);
	}

	// print out the final hamming code in binary,
	// so that it lines up with the previously printed data bits
	// loop : the first i bits of the hamming code have been printed
	cout << "hamming code:\t";
	for (int i = 0 ; i < total_bits; i++){
		cout << hamming_arr[i];
	}
	cout << endl;
	return hamming_code;
}
// corresponding hamming code with the input value stored in the data bits
int main() {	
	cin.setf(ios::hex, ios::basefield);
	cout << "Enter up to a fourteen-digit-digit hexadecimal value: ";
	long input;
	cin >> input;	
	long output = hamming(input);	
	cout << "The hamming code is " << output << endl;
	return 0;
}
