// Author: Dayuan Wang, Teng Xu
// Login id: wangdayu, xt 
// email: wangdayu@bu.edu, xt@bu.edu
// UID: U59515898, U95831644


#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


// initialize all the global variables
int s; // going to need them when I am going to constructe the cache.
int E;
int b; 
int hit_count; // going to need then for the counters. 
int miss_count;
int eviction_count;



// Defining the structure for each of the cache slot
// I will call it cache line
typedef struct{
  int valid;
  int tag;
  int activity; // Use to track the cache slot activity.
}Cache_line;

// Initialize the whole cache.
// Each of the cache slot is a struct that I defined above. 
Cache_line **cache; 

// Get the set bits from a certain address.
int getSetBits(long int address){
  return (address >> b) & (0x7fffffffffffffff >> (63-s)); 
}

// Get the tag bits from a certian address.
int getTagBits(long int address){
  return (address >> (s + b) & (0x7fffffffffffffff >> (63 - s - b))); 
}

// I am going to update the activity of the each cache slot.
// When a new data get put into a slot, I am going to set the activity to
// the highest value in the whole cache_line.
// Then I am going to go through the cache line, decrease the other cache activity by one,
// and make sure the activity value is between 0 to (E-1).
// The cache slot with the activity value 0 means the least resent used one.
void update_activity(int set_index, int way_index){
  int i;
  for(i = 0; i< E; i++){
    if(cache[set_index][i].valid == 1 && cache[set_index][i].activity > cache[set_index][way_index].activity){ // I 
      // just want to make sure the activity are between 0 and (E-1)
      cache[set_index][i].activity -= 1; }
  }
  cache[set_index][way_index].activity = E-1; // Set the MRU one to the highest activity value  in the set.
}

// Hit = 0; Miss = 1; Miss_then_Hit = 2; Miss_Evict_Hit = 3; Miss_Evict = 4 

// This is the function to find the data from the cache.
// Going to use the sscanf to process the input instruction.
// Then use the helper function to get the tag bits and the set bits from a address.
// Use the set bits to go the the cache set line,then use the for loop to go
// over the set.
// If it is found in the cache,update the cache activity, increment the hit_count by 1, then terminate
// the function. If the instruction type is 'M', that means we will have one more hit during accessing.
// If it is not a hit, then increment the miss_hit by 1. Now, we need to update the cache set activity,
// and terminal the function.
// If the instruction is a 'M', after a miss, it will be a hit after update the cache.
// If the whole set is full and we have not find the data in the cache, that means there is a eviction.
// The table needs to be update based on the LRU.
int find_cache(char instr[]){
  char ins_type;
  int size;
  long int address;
  sscanf(instr," %c %lx %d", &ins_type, &address, &size); // sscanf() uses after fgets();
  int set_index = getSetBits(address);
  int tag = getTagBits(address);
  
  // Checking for the hit
  // If hit happenes, the function will get returned. 
  int i;
  for(i = 0; i < E; ++i){
    if(cache[set_index][i].valid == 1 && cache[set_index][i].tag == tag){
      hit_count += 1;
      update_activity(set_index,i);
      if(ins_type == 'M'){
	hit_count += 1;
      }
      return 0;
    }
  }
  
  // when the function goes here, it means the miss is happened.
  // Check if there is no eviction happened. 
  // If there is no eviction, put it into the cache then return it. 
  miss_count += 1;
  for(i = 0; i < E; ++i){
    if(cache[set_index][i].valid == 0){
      cache[set_index][i].valid = 1;
      cache[set_index][i].tag = tag;

      update_activity(set_index, i);
      
      if(ins_type == 'M'){
	hit_count += 1;
	return 2;
      }
      else{
	return 1;
      }
    }
  }
  
  // When the function goes here, it means there is eviction happened.
  // put the data into the LRU slot, do the right things and return it. 
  eviction_count += 1;
  for(i = 0; i < E; ++i){
    if(cache[set_index][i].activity == 0){
      cache[set_index][i].tag = tag; // put the data in the cache
      // no need to update the valid bits, since it is one already.

      update_activity(set_index,i);
      
      if(ins_type == 'M'){
	hit_count += 1;
	return 3;
      }
      else{
	return 4;
      }

    }
  }
  return -1; 

}

// This is the helper function to process the instruction line in the file.
// Becasuse there is always a '\n' character in the end of the instruction.
// This function will replace the '\n' character in the instruction line to a ' '. 
void edit_instruction(char ins[]){
  int q = 0;
  for(q = 0; q < 100; q++){
    if(ins[q] == '\n'){
      ins[q] = ' ';
    }
  }
}


// This is the mean function
int main(int argc, char *argv[])
{
  int opt;
  int vbit; // This is to process the command line v bits 
  int result = 0; // This is to store the cache_find result for each instruction
  // in the file. 
  FILE *f; // file pointer
  char trace[100]; // Used to store the file name.
  char instruction[100]; // Used to store in instruction from the file. 

  // Use the getopt function to get the input value of E, b, t from the input
  // We need to use these value to determine the cache table.
  while((opt = getopt(argc, argv, "vs:E:b:t:"))!=-1) 
     {
       switch(opt) 
	 {
	 case 'v':
	   vbit = 59; // check that if there is a v bit in the instruction line
	   break;
	 case 's':
	   s = atoi(optarg); 
	   break;
	 case 'E':
	   E = atoi(optarg);
	   break;
	 case 'b':
	   b = atoi(optarg);
	   break;
	 case 't':
	   strcpy(trace, optarg);
	   break;
	 default:
	   printf("Invalid argument!\n");
	   break;
	 }
     }
  
    // Use the value of s and E to calculate the size of cache table.
    int cache_size = pow(2,s);
    // Use the malloc to free the space and points to the cache table.
    cache = (Cache_line**)malloc(cache_size * sizeof(Cache_line*));
    int i;
    for(i = 0; i < cache_size; i++){
        cache[i] = (Cache_line*)malloc(E * sizeof(Cache_line));
        // For each cache, it is a struct, we will initialize the values
        // inside of all the struct.
        int j;
        for(j = 0; j < E; j++ ){
            cache[i][j].valid = 0;
            cache[i][j].tag = -1;
            cache[i][j].activity = 0;
        }
    }
    
    // Open the file.
    f = fopen(trace, "r");
    
    // Use the function fgets to get all the instructions.
    while(fgets(instruction, 100, f)){
        // We only care about the instructions other than the 'I'.
        if(instruction[0] == ' '){
            // For each line of the instructions, we are going to call
            // the helper function to find the cache, and count the miss, hit, and eviction.
	    // we will store the result in the local variable called result.
	    // If the v bit is triggered, we will print the result of each instruction.
	    // 0 means Hit. 1 means Miss. 2 means Miss then hit.
	    // 3 means Miss, eviction and hit. 4 means Miss and eviction. 
            result = find_cache(instruction);
	    edit_instruction(instruction); // Going to process the instruction line, remove the '\n' character. 
	    if(vbit == 59){
	      if(result == 0){
		printf("%s Hit\n",instruction );
	      }
	      else if(result == 1){
		printf("%s Miss\n", instruction);
	      }
	      else if(result == 2){
		printf("%s Miss then hit\n", instruction);
	      }
	      else if(result == 3){
		printf("%s Miss evict then hit\n",instruction);
	      }
	      else if(result == 4){
		printf("%s Miss and evict\n", instruction);
	      }
	      else{
		printf("what is this?\n");
	      }
	    }


        }
    }
    
    // Then close the file.
    fclose(f);
    
    // Print out the result.
    printSummary(hit_count, miss_count, eviction_count);
    
    // Free the space we created for the cache
    free(cache);
  
    return 0;
}
