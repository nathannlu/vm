#ifndef TOKENS_H 
#define TOKENS_H


struct tok_key_pair {
  char key[10];
  char value[10];
};

struct tok_map {
  const char*   data[100][2];
  int           size;
}



struct tok_map myMap = {
  // Initial key-value pairs
  {
    {"/^\\d+/", "NUMBER"}, 
    {"/^\"[^\"]*\"/", "STRING"}
    {"/^\'[^\']*\'/", "STRING"}
  }, 

  // Initial size
  2 
};




#endif
