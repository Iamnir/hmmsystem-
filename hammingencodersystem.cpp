// Program to convert any hexadecimal value to its hamming code
//   and to detect and correct single bit errors in a hamming code
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;

int sizearray(char array[]) {
  // this determines the size of a char array.
  // size is obviously used to keep track of the number of elements
  int size(0);
  while (array[size] != '\0')
    size++;
  return (size*4); /* I return size*4 because I send a char array with 
		      hexadecimal values in it, so the size of the larger
		      array with binary values will be four times as long*/
}

int num_parity(int size) {
  // this function determines the number of parity bits that will be needed
  // for a given number of bits
  int temp(0), ans(0); /* temp is used to check where the parity bits will be
			 positioned in the array, and if that value is larger
			 than the size plus the number of parity bits already
			 in the array (ans), then there will be no more*/
  for (int i=0; ; i++) {
    temp = (1 << i);
    if (temp < (size+ans))
      ans++;
    else break;
  }
  return ans; // return the number of parity bits
}

void binary_array(int arr[], int size, unsigned long long number) {
  // this function takes an empty array (arr) of size (size) and fills it with
  // binary values by bit shifting the decimal value of number.
  int a; //keeps track of how many bits need to be subtracted when shifting
  for (int i=0; i < size; i++) {
    arr[i] = number >> ((size-1)-i);
    a = i;
    while (a > 0) {
      arr[i] -= (arr[a-1]*(1 << (i-(a-1))));
      a--;
    }
  }
}

void parity(int bits[], int paritybits, int size, bool check) {
  // this function is used for both the normal hamming code generator and
  //   the checker to compute and check parity bits

  // the bits array is what will be used to insert or check the parity bits,
  //   paritybits is the number of parity bits in the array, size is the size
  //   of the array, and check is a bool (false is the hamming generator, true
  //   is the checker)
  
  int total; //this will store the sum of the bits
  int num_bits; //the i-th parity bit counts 2^(i-1) bits and then skips 2^(i-1) bits
  int n; //n denotes where the parity bit is located in the code array
  int count; /*count keeps track of how many bits the current iteration
	       has added*/
  int current; // current keeps track of the index that the parity bit is checking
  int incorrect(0); //keeps track of the sum of the incorrect parity bits
  
  for (int i=0; i < paritybits; i++) {
    total=0;
    num_bits = pow(2,i); //the i-th parity bit counts 2^(i-1) bits
    n = num_bits - 1; //n denotes where the parity bit is located in the code array
    count = num_bits; /*count keeps track of how many bits the current iteration
			has added*/
    if (!check) {
      if (i == 0)
	n+=2; //the first parity bit starts checking at the third bit
      else {
	count--; /*on the first check, the parity bit doesn't check itself, so the number
		   of bits that it checks is one less.*/
      }
    }
    
    //calculate parity bits
    while (n < size) {
      while (count > 0) {
	/*the current bit that it needs to check will be the position of the parity 
	  bit that it is currently checking (n) plus (the number of bits that parity bit 
	  counts minus how many it has already counted.*/
	current = n + (num_bits - count);
	if (current > (size-1)) 
	  break; /*make sure we don't try to access an element outside
		   of the array*/
	total += bits[current];
	count--;
      }
      count = num_bits;
      n = current + num_bits + 1;
    }
    if (check) {
      if ((total%2) == 1) {
	incorrect += pow(2,i); /* if the total is not even, then that parity
				  bit is wrong */
      }
    }
    else {
      if (total%2 == 1)
	bits[(1 << i)-1] = 1;
      else 
	bits[(1 << i)-1] = 0;
    }
  }
  if (check) {
    if (incorrect != 0) {
      // if the incorrect int is not zero, then one of the bits is wrong
      cout << "There is an error in the code. The corrected hexadecimal value is ";
      bits[incorrect-1] = !(bits[incorrect-1]);
      //calculate the answer in base 10 using the array
      unsigned long long ans(0);
      for (int i=(size-1); i >= 0; i--)
	ans += (bits[i]*(pow(2,((size-1)-i))));
      cout.setf(ios::hex,ios::basefield);
      cout << ans << endl;      
    }
    else {
      // if incorrect is zero, then nothing was wrong with the code
      cout << "The code is correct." << endl;
    }
  }
}

void bitarray(unsigned long long val, int code[], int size, int paritybits) {
  // bit array is used for the code generator. it takes a decimal number (val)
  //   and puts that number into the code of length (size+paritybits). 
  //   It must skip over the parity bits and shift the number a correct amount 
  //   then subtract previous values
  //   size is the length of (val) in binary
  int a_sm(0),a_lg(0),num1,index(0),pos(0);
  // a_sm keeps track of how many bits have been processed from the length
  //   of (val), a_lg keeps track of how many values have been filled from
  //   the large array with the parity bits. index keeps track of the index
  //   that the large array is currently calculating, and num1 and pos are 
  //   used to keep track of how many values have been subracted when 
  //   calculating a value after shifting the bits
  while ((a_sm < size) && (a_lg < (size+paritybits))) {
    if (code[a_lg] == -1) {
      a_lg++;
    }
    else {
      index = a_lg;
      code[index] = (val >> ((size-1)-a_sm));
      pos = index;
      num1 = 0;
      do {
	if (code[pos-1] == -1)
	  pos--;
	else {
	  code[index] -= (code[pos-1] * (1 << (num1+1)));
	  num1++;
	  pos--;
	}
      } while (num1 <= a_sm);
      a_sm++;
      a_lg++;
    }
  }
}

unsigned long long hamming(char num[]) {
  //find the size of the input
  int size = sizearray(num);
  
  //calculate number of parity bits
  int paritybits = num_parity(size);
  
  //the size of the whole array will be the number of original bits plus parity bits
  int total = size+paritybits;

  //make an array length (size+paritybits) for the final bits
  int code[total];
  //set all the bits equal to 0
  for (int i=0; i < total; i++)
    code[i] = 0;
  
  //denote the parity positions with -1
  for (int i=0; i < paritybits; i++){
    int index = pow(2,i)-1;
    code[index] = -1;
  }

  //convert the hexadecimal to decimal
  unsigned long long val = strtoul(num, 0, 16);

  //put the original bits (without the parity bits) into the big array 
  bitarray(val,code,size,paritybits);
  
  // variables for loop to calculate parity bits
  parity(code, paritybits, total, false);

  //calculate the answer in base 10 using the code array
  unsigned long long ans(0);
  for (int i=(total-1); i >= 0; i--)
    ans += (code[i]*(pow(2,((total-1)-i))));

  return ans; /* this returns the hamming code in decimal (to be couted in
		 base 16) */
}


void checker(char num[]) {
  //find the size
  int size = (sizearray(num))/4;

  // convert binary to decimal
  unsigned long long decimal = strtoul(num, 0, 2);

  cout.setf(ios::hex,ios::basefield);
  cout << "Your code in hexadecimal is " << decimal << "." << endl << endl;

  //calculate the number of parity bits
  int paritybits = num_parity(size);

  // put the binary bits into an array
  int bits[size];
  binary_array(bits,size,decimal);

  // check the bits
  parity(bits, paritybits, size, true);

}


int main() {
  cout << "This program has two functions: " << endl;
  cout << "1) Calculate the Hamming code of any given hexadecimal input" << endl;
  cout << "2) Detect and correct single bit errors in a Hamming code entered as input" << endl;
  int option;
  cout << endl;
  cout << "Which option would you like? Enter 1 or 2: ";
  cin >> option;
  cout << endl;

  if (option == 1) {
    char hex[15];
    // only 8 values can be used since that's all a unsigned long long can hold
    //  after being converted to a decimal if all the values are f's
    cout << "Enter a number in hexadecimal (up to 8 values): ";
    cin >> hex;
    cout << endl;

    unsigned long long ans;
    ans = hamming(hex);
  
    cout.setf(ios::hex, ios::basefield);
    cout << "The hamming code is " << ans << endl;
  }
  else if (option != 2) {
      cout << "You didn't enter a 1 or 2, try again." << endl << endl;
      main();
    }
  else {
    char hex[50];
    cout << "Enter the Hamming code in binary (up to 32 bits): ";
    cin >> hex;
    checker(hex);
  }

  return 0;
}
