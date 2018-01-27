#include <iostream>
#include <cmath>
using namespace std;

int main() {
	int no_of_data_bits; 

	cout<<"Enter no_of_data_bits "<<endl; 
	cin>> no_of_data_bits;

	/*creating parity positions array as per no of data bits...here parity_pos[] array size 
	would be no_of_data_bits and indexing would be from 0 to (no_of_data_bits - 1)...*/
	int no_of_parity_bits = log2(no_of_data_bits) + 1;

	int parity_pos[no_of_parity_bits]; 

	for (int i = 0; i < no_of_parity_bits; ++i) {
        parity_pos[i] = pow(2,i);
    }
    cout<<"Calculated number of parity bits = "<<no_of_parity_bits; 

    /*creating a sending array of size (no_of_data_bits + no_of_parity_bits + 1)...here send_arr[] indexing would be 
	from 1 to ((no_of_data_bits + no_of_parity_bits + 1) - 1)...index '0' of send_arr[] is not used...*/
    int send_arr[no_of_data_bits + no_of_parity_bits + 1] = {};

     /*creating a variable 'k' which would be used to bypass the parity positions in send_arr[] by referring parity_pos[]...*/
    cout<<"\nEnter Data Bits one by one :\n";
    /*this loop-1  for send_arr[],  here I skipped the value of i =1, 2 because it is for parity bits. It is filling data_bits into the 
    corresponding send_arr[]   */
    for(int i = 3, j = 2; i < no_of_parity_bits + no_of_data_bits + 1; i++) {
        if (i == parity_pos[j]) {
            j++;
            continue;
        }
        cin>> send_arr[i]; 
    }
    // This loop-2 fills up parity bit of send_arr[]. 
    for (int i = 1, j = 0; i < no_of_parity_bits + no_of_data_bits + 1; ++i) {
        if (i != parity_pos[j]) {
            continue;
        }
        j++;
        /* inside this nested loop : i for parity position we  need to select i bits  from ith position of send_arr[] and then skip the next
         i bits from the same position ......so that means we  can skip 2*i bits together to get another bit.... this is what L is doing in this
		 loop and K is selecting the i bits ...............its like use and skip method to find parity value p1= use 1 and skip 1 , for p2 = use 2 skip 2 
		 for example : if i = 1 and l = i i.e. 1 and then skip 2*i      */
        for (int l = i; l < no_of_data_bits + no_of_parity_bits + 1; l += 2*i) {
            for (int k = 0; k < i && (l+k) < no_of_data_bits + no_of_parity_bits + 1; k++) {
                send_arr[i] ^= send_arr[l+k];
            }
        }
    }

    /*Displaying the data to be transmitted...*/
    cout<<"\nData to be transmitted: ";
    for(int i = 1; i < no_of_data_bits + no_of_parity_bits + 1; i++) {
        cout<< send_arr[i] ;
    }
    cout<<endl;

 return 0; 
}

