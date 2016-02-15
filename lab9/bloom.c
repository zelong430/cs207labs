#include "bloom.h"
#include <math.h>

index_t hash1(bloom_filter_t *B, key_t k){
	key_t key = k;
	key = (~key) + (key << 21); // key = (key << 21) - key - 1;
  	key = key ^ (key >>24);
	key = (key + (key << 3)) + (key << 8); // key * 265
	key = key ^ (key >> 14);
	key = (key + (key << 2)) + (key << 4); // key * 21
	  key = key ^ (key >> 28);
	  key = key + (key << 31);
	  return key;
}

index_t hash2(bloom_filter_t *B, key_t k){
	key_t a = k;
	a = (a+0x7ed55d16) + (a<<12);
    a = (a^0xc761c23c) ^ (a>>19);
    a = (a+0x165667b1) + (a<<5);
    a = (a+0xd3a2646c) ^ (a<<9);
    a = (a+0xfd7046c5) + (a<<3);
    a = (a^0xb55a4f09) ^ (a>>16);
    return a;
    return a;
    
}

void set_bit(bloom_filter_t *B, index_t i){
	if (B->size <= i)
	{
		printf("Index Out of Range");
		return;
	}
	
	index_t * temp = B->table;
	index_t int_pos = i >> 6;
	index_t bit_pos = i % 64;
	index_t mask = 1 << bit_pos;
	B->table [int_pos] = B->table[int_pos] | mask;
}

index_t get_bit(bloom_filter_t *B, index_t i){
	if (B->size<=i)
	{
		printf("Index Out of Range");
		return 0;
	}
	index_t * temp = B->table;
	index_t int_pos = i >> 6;
	index_t bit_pos = i % 64;
	index_t mask = 1 << bit_pos;
	index_t result = B-> table[int_pos] & mask;
	result = result >> bit_pos;
	return result;
}

void bloom_init(bloom_filter_t *B, index_t size_in_bits)
{
	B->count = (uint64_t)0;
	index_t size_in_64 = (uint64_t)0;
	if (size_in_bits%(sizeof(uint64_t)*8) != 0)
		size_in_64 = size_in_bits/(8*sizeof(uint64_t))+1;
	else
		size_in_64 = size_in_bits/(8*sizeof(uint64_t));
	B->size = size_in_bits;
	B->table = calloc(size_in_bits/64,sizeof(uint64_t));
}

void bloom_destroy(bloom_filter_t *B)
{
	free(B);
}

int bloom_check(bloom_filter_t *B, key_t k){
	int num_of_one = 0;
	for(int i=0; i < N_HASHES; i++){
		int hash = hash1(B, k) + i * hash2(B, k);
		hash = hash % B->size;
		if (get_bit(B, hash) == 0){
			return 0;
		}

	}
	return 1;
}
void bloom_add(bloom_filter_t *B, key_t k){
	for(int i = 0; i < N_HASHES; i++){
		int hash = hash1(B, k) + i * hash2(B, k);
		hash = hash % B->size;
		set_bit(B, hash);
		B->count ++;
	}
}

int* rand100(void)
{
	int* r = calloc(100,sizeof(int));
	for(int i = 0; i<100; i++)
	{
		r[i] = rand() % 1000000;
	}
	return r;
}

void experiment(int * arry1, int * arry2){
	bloom_filter_t* B = (bloom_filter_t*) malloc(sizeof(bloom_filter_t));
	bloom_init(B,1000);
	for (int i=0; i< 100;i++){
		bloom_add(B, arry1[i]);
	}
	int num_of_occupy = 0;
	for(int i= 0; i< 1000; i++){
		if(get_bit(B, i) == 1){
			num_of_occupy ++;
		}
	}	
	int num_of_collsion = 0;
	for (int i=0; i< 100;i++){
		if (bloom_check(B, arry2[i]) == 1){
			num_of_collsion ++;
		}
	}
	printf("Experiment: num_of_occupy is %d and num_of_collsion is %d\n", num_of_occupy, num_of_collsion);
}

int main(int argc, char **argv)
{

	bloom_filter_t* B = (bloom_filter_t*) malloc(sizeof(bloom_filter_t));
	bloom_init(B,1000);


	for(int i=0;i<4;i++){
		printf("Hash TEST for key %d: hash1 is %llu hash2 is %llu \n", i,hash1(B,i),hash2(B,i));
	}
	printf("Hash TEST for key %d: hash1 is %llu hash2 is %llu \n", 13,hash1(B,13),hash2(B,97));
	printf("Hash TEST for key %d: hash1 is %llu hash2 is %llu \n", 97,hash1(B,13),hash2(B,97));
	
	int num_of_occupy = 0;
	for(int i = 1; i<=70;i++){
		bloom_add(B, i);
	}
	for(int i= 0; i< 1000; i++){
		if(get_bit(B, i) == 1){
			num_of_occupy ++;
		}
	}	
	printf("Smoke test. Number of bits in the table %d\n" , num_of_occupy);

	int * arry1 = rand100();
	int * arry2 = rand100();
	experiment(arry1, arry2);

}